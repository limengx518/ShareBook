#include "jottingbroker.h"
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>
#include "materialbroker.h"
#include "commentbroker.h"

JottingBroker* JottingBroker::m_jottingBroker=NULL;
std::unordered_map<std::string,Jotting> JottingBroker::m_newClean={};
std::unordered_map<std::string,Jotting> JottingBroker::m_oldClean={};
std::mutex JottingBroker::m_mutex={};


JottingBroker *JottingBroker::getInstance()
{
    if(m_jottingBroker==nullptr){
        m_jottingBroker=new JottingBroker();
    }
    return m_jottingBroker;
}

Jotting &JottingBroker::findById(std::string id)
{
    Jotting* jotting=inCache(id);

    if(jotting == nullptr){
        return this->retrieveJotting(id);
    }

    return *jotting;
}

Jotting &JottingBroker::retrieveJotting(std::string jottingId)
{
    std::string command="select * from Jotting where J_id="+jottingId;
    sql::ResultSet* res=RelationalBroker::query(command);
    std::string id,content,nid,time;
     // Loop through and print results
    while (res->next()) {
        id=std::to_string(res->getUInt(1));
        content=res->getString(2);
        time=res->getString(3);
        nid=std::to_string(res->getUInt(4));
    }
    //retrieveJotting(id)
    Jotting jotting(id,content,time,nid,findMaterials(id),findComments(id));

    //将从数据库中拿出的数据放在缓存中(旧的净缓存)
    m_oldClean.insert({jotting.id(),jotting});

    return m_oldClean.at(jotting.id());

//    return nullptr;
}

std::vector<JottingProxy> JottingBroker::pushJottings(std::string netizenId, std::string lastTime, std::string thisTime)
{
    std::cout<<"lastTime:"<<lastTime<<std::endl;
    std::cout<<"thisTime:"<<thisTime<<std::endl;
//    thisTime="2022-09-25 21:27:59";
    std::string command="select * from Jotting where J_time BETWEEN '"+lastTime + "' AND '"+thisTime+"'";
    sql::ResultSet* res=RelationalBroker::query(command);
    std::string id;
    std::vector<JottingProxy> ids;
     // Loop through and print results
    while (res->next()) {
        id=std::to_string(res->getUInt(1));
        ids.push_back(JottingProxy(id));
    }
    return ids;
}


std::vector<std::string> JottingBroker::findMaterials(std::string jottingId)
{
    std::vector<std::string> materialIds;
std::string com="select M_id from Material where J_id="+jottingId;
    sql::ResultSet* res=RelationalBroker::query(com);
    while (res->next()) {
        materialIds.push_back(std::to_string(res->getUInt(1)));
    }

    std::vector<std::string> newMaterials=MaterialBroker::getInstance()->findJottingNewMaterial(jottingId);
    materialIds.insert(materialIds.begin(),newMaterials.begin(),newMaterials.end());
    return materialIds;
}

std::vector<std::string> JottingBroker::findComments(std::string jottingId)
{
    std::vector<std::string> commentIds;
std::string com="select C_id from Comment where J_id="+jottingId;
    sql::ResultSet* res=RelationalBroker::query(com);
    while (res->next()) {
        commentIds.push_back(std::to_string(res->getUInt(1)));
    }

    std::vector<std::string> newComments=CommentBroker::getInstance()->findJottingNewComment(jottingId);
    commentIds.insert(commentIds.begin(),newComments.begin(),newComments.end());
    return commentIds;
}

std::vector<std::string> JottingBroker::findNewJottings(std::string netizenId)
{
    //网民新发布的笔记应该是存在于newCleanCache中
    std::vector<std::string> res;
    for(auto& item:m_newClean){
        if(item.second.netizenId()==netizenId){
            res.push_back(item.first);
        }
    }

    return res;
}

Jotting* JottingBroker::inCache(std::string id)
{
    //判断是否在缓存中
    if(m_oldClean.count(id)){
        return &m_oldClean.at(id);
    }

    if(m_newClean.count(id)){
        return &m_newClean.at(id);
    }

    return nullptr;
}

void JottingBroker::newCleanFlush()
{
    for(auto iter = m_newClean.begin(); iter != m_newClean.end();){

        std::cout<<"将笔记内容存入数据库"<<std::endl;
           //保证当进行插入时，数据是不可以被更改的？？？？
           std::unique_lock<std::mutex> lk(m_mutex);

           //将数据插入数据库
           std::string command="insert into Jotting (J_id,J_content,J_time,N_id) values("+iter->first+","+"'"+iter->second.note()+"','"+iter->second.time()+"','"+iter->second.netizenId()+"')";
           RelationalBroker::insert(command);

           //从缓存中删除相关数据
           std::cout<<"笔记内容从缓存中删除"<<std::endl;
           //unordered_map这种链表型容器erase后返回的迭代器为当前的位置
           m_newClean.erase(iter++);
           lk.unlock();
       }
}


void JottingBroker::flush()
{
    newCleanFlush();
}


void JottingBroker::addJotting(const Jotting &jotting)
{
    //将从数据库中拿出的数据放在缓存中(旧的净缓存)
    m_newClean.insert({jotting.id(),jotting});
}

JottingBroker::~JottingBroker()
{
    //程序退出后，将缓存中的新数据存入数据库
    for(auto &jotting:m_newClean){
        std::string command="insert into Jotting (J_id,J_content,J_time,N_id) values("+jotting.second.id()+","+jotting.second.note()+","+jotting.second.time()+","+jotting.second.netizenId()+")";
        RelationalBroker::insert(command);
    }
}

JottingBroker::JottingBroker()
{
//    Jotting jotting("8","你好","2022-05-22 10:00:01.00","10",{},{});
//    m_newClean.insert({jotting.id(),jotting});
//    Jotting jotting1("10","你好","2022-05-22 10:00:01.00","10",{},{});
//    m_newClean.insert({jotting1.id(),jotting1});
}
