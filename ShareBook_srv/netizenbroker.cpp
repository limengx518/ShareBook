

#include "netizenbroker.h"
#include "commentbroker.h"
#include "jottingbroker.h"

NetizenBroker* NetizenBroker::m_netizenBroker=NULL;
std::unordered_map<std::string,Netizen> NetizenBroker::m_newClean={};
std::unordered_map<std::string,Netizen> NetizenBroker::m_oldClean={};
std::multimap<std::string,std::string>  NetizenBroker::m_newFollow={};
std::mutex NetizenBroker::m_mutex={};

NetizenBroker *NetizenBroker::getInstance()
{
    if(m_netizenBroker==nullptr){
        m_netizenBroker=new NetizenBroker();
    }
    return m_netizenBroker;
}

Netizen &NetizenBroker::findById(std::string id)
{
    Netizen *netizen=inCache(id);
    if(netizen==nullptr){
        return retrieveNetizen(id);
    }
    return *netizen;
}


std::vector<std::string> NetizenBroker::findJottings(std::string netizenId)
{
    //从数据库中将所有netizen发布的jotting的id拿出来
    std::string com="select J_id from Jotting where N_id="+netizenId;
    sql::ResultSet* res=RelationalBroker::query(com);
    std::vector<std::string> jottingIds;
    while (res->next()) {
        jottingIds.push_back(std::to_string(res->getUInt(1)));
    }

    //查看jotting的缓存中是否有该netizen新发布的笔记还未存入数据库
    std::vector<std::string> newCacheJotting=JottingBroker::getInstance()->findNewJottings(netizenId);
    jottingIds.insert(jottingIds.end(),newCacheJotting.begin(),newCacheJotting.end());

    return jottingIds;

}

std::vector<std::string> NetizenBroker::findFans(std::string netizenId)
{
    std::string com="select N_Fan_id from Relation where N_id="+netizenId;
    std::vector<std::string> fansIds;
    sql::ResultSet* res=RelationalBroker::query(com);
    while (res->next()) {
         fansIds.push_back(std::to_string(res->getUInt(1)));
    }

    std::vector<std::string> newCacheFans=findNewFans(netizenId);
    fansIds.insert(fansIds.end(),newCacheFans.begin(),newCacheFans.end());

    return fansIds;
}

std::vector<std::string> NetizenBroker::findConcereds(std::string netizenId)
{

    std::string com="select N_id from Relation where N_Fan_id="+netizenId;
    std::vector<std::string> concernedIds;
    sql::ResultSet* res=RelationalBroker::query(com);
    while (res->next()) {
         concernedIds.push_back(std::to_string(res->getUInt(1)));
    }

    std::vector<std::string> newCacheConcerneds=findNewFans(netizenId);
    concernedIds.insert(concernedIds.end(),newCacheConcerneds.begin(),newCacheConcerneds.end());

    return concernedIds;

}

std::vector<std::string> NetizenBroker::findComments(std::string netizenId)
{
    std::string com="select C_id from Comment where N_id="+netizenId;
    std::vector<std::string> commentIds;
    sql::ResultSet* res=RelationalBroker::query(com);
    while (res->next()) {
         commentIds.push_back(std::to_string(res->getUInt(1)));
    }

    std::vector<std::string> newCacheComments=CommentBroker::getInstance()->findNewComments(netizenId);
    commentIds.insert(commentIds.end(),newCacheComments.begin(),newCacheComments.end());

    return commentIds;
}

std::vector<std::string> NetizenBroker::findNewFans(std::string netizenId)
{
    std::vector<std::string> fansId;
    for(auto& item:m_newFollow){
        if(item.first==netizenId){
            fansId.push_back(item.second);
        }
    }

    return fansId;
}

std::vector<std::string> NetizenBroker::findNewConcerneds(std::string netizenId)
{
    std::vector<std::string> concernedsId;
    for(auto& item:m_newFollow){
        if(item.second==netizenId){
            concernedsId.push_back(item.first);
        }
    }

    return concernedsId;

}

Netizen &NetizenBroker::retrieveNetizen(std::string netizenId)
{
    std::string com="select * from Netizen where N_id="+netizenId;
    std::string id,nickName,signal,avater;
    sql::ResultSet* res=RelationalBroker::query(com);
    // Loop through and print results
    while (res->next()) {
        id=std::to_string(res->getUInt(1));
        nickName=res->getString(2);
        avater=res->getString(3);
        signal=res->getString(4);
    }
    Netizen netizen(id,nickName,signal,avater,findJottings(id),findFans(id),findConcereds(id),findComments(id));

    //将从数据库中拿出的数据放在缓存中(旧的净缓存)
    m_oldClean.insert({netizen.id(),netizen});

    return m_oldClean.at(netizen.id()) ;
}

void NetizenBroker::addFollowRelation(std::string nId, std::string fanId)
{
    m_newFollow.insert({nId,fanId});
}

Netizen *NetizenBroker::inCache(std::string id)
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



NetizenBroker::~NetizenBroker()
{
    //程序退出后，将缓存中的新数据存入数据库
    for(auto &netizen:m_newClean){
        std::string command="insert into Netizen values ("+netizen.first+",'"+netizen.second.nickName()+"')";
        RelationalBroker::insert(command);
    }

    for(auto iter = m_newFollow.begin(); iter != m_newFollow.end(); iter++){
        std::string command="insert into Relation values ("+iter->first+","+iter->second+")";
        RelationalBroker::insert(command);
    }
}


void NetizenBroker::newCleanFlush()
{
    for(auto iter = m_newClean.begin(); iter != m_newClean.end();){

        //应该保证当进行插入时，数据是不可以被其他线程所更改的
        std::lock_guard<std::mutex> lk(m_mutex);

        std::string command="insert into Netizen values ("+iter->first+",'"+iter->second.nickName()+"')";
        RelationalBroker::insert(command);

        //从缓存中删除相关数据
        std::cout<<"从缓存中删除"<<std::endl;
        //unordered_map这种链表型容器erase后返回的迭代器为当前的位置
        m_newClean.erase(iter++);
    }
}


void NetizenBroker::newFollowFlush()
{
    for(auto iter = m_newFollow.begin(); iter != m_newFollow.end();){

        //应该保证当进行插入时，数据是不可以被其他线程所更改的
        std::lock_guard<std::mutex> lk(m_mutex);

        std::string command="insert into Relation values ("+iter->first+","+iter->second+")";
        RelationalBroker::insert(command);

        //从缓存中删除相关数据
        std::cout<<"从缓存中删除"<<std::endl;
        //unordered_map这种链表型容器erase后返回的迭代器为当前的位置
        m_newFollow.erase(iter++);
    }
}


void NetizenBroker::flush()
{
    newCleanFlush();
    newFollowFlush();
}

NetizenBroker::NetizenBroker()
{

//    m_newFollow.insert({"9","1"});
//    m_newFollow.insert({"1","9"});

//    Netizen netizen("9","harward",{},{},{},{});
//    Netizen netizen1("10","李梦雪",{},{},{},{});
//    m_newClean.insert({netizen.id(),netizen});
//    m_newClean.insert({netizen1.id(),netizen1});

}
