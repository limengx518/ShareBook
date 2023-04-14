#include "commentbroker.h"
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>

CommentBroker* CommentBroker::m_CommentBroker=NULL;
std::unordered_map<std::string,Comment> CommentBroker::m_newClean={};
std::unordered_map<std::string,Comment> CommentBroker::m_oldClean={};
std::mutex CommentBroker::m_mutex={};

CommentBroker *CommentBroker::getInstance()
{
    if(m_CommentBroker==nullptr){
        m_CommentBroker=new CommentBroker();
    }
    return m_CommentBroker;
}

Comment &CommentBroker::findById(std::string id)
{
    Comment* comment=inCache(id);
    if(comment == nullptr){
        return retrieveComment(id);
    }
    return *comment;
}

std::vector<std::string> CommentBroker::findNewComments(std::string netizenId)
{
    std::vector<std::string> commentsId;
    for(auto& item:m_newClean){
        if(item.second.publisherId()==netizenId){
                commentsId.push_back(item.first);
        }
    }

    return commentsId;

}

std::vector<std::string> CommentBroker::findJottingNewComment(std::string jottingId)
{
    std::vector<std::string> commentsId;
    for(auto& item:m_newClean){
        if(item.second.jottingId()==jottingId){
                commentsId.push_back(item.first);
        }
    }

    return commentsId;

}

Comment &CommentBroker::retrieveComment(std::string commentId)
{
    std::string command="select * from Comment where C_id="+commentId;
    sql::ResultSet* res=RelationalBroker::query(command);
    std::string id,content,nid,jid,time;
     // Loop through and print results
    while (res->next()) {
        id=std::to_string(res->getUInt(1));
        content=res->getString(2);
        time=res->getString(3);
        nid=std::to_string(res->getUInt(4));
        jid=res->getString(5);
    }
    //retrieveComment(id)
    Comment comment(id,content,time,nid,jid);

    //将从数据库中拿出的数据放在缓存中(旧的净缓存)
    m_oldClean.insert({comment.id(),comment});

    return m_oldClean.at(comment.id());
}

void CommentBroker::addComment(const Comment &comment)
{
    m_newClean.insert({comment.id(),comment});
}



CommentBroker::~CommentBroker()
{
    //将缓存中的剩余的数据存入数据库
    for(auto &comment:m_newClean){
        std::string command="insert into Comment (C_id,C_content,C_time,N_id,J_id) values("+comment.second.id()+",'"+comment.second.content()+"','"+comment.second.time()+"',"+comment.second.publisherId()+","+comment.second.jottingId()+")";
        RelationalBroker::insert(command);
    }
}

CommentBroker::CommentBroker()
{
    // explicit Comment(id,content,publisherId,jottingId);
//    Comment comment1("12","bnag","2022-07-09 09:41:40","10","10");
//    m_newClean.insert({comment1.id(),comment1});
}

void CommentBroker::flush()
{
    newCleanFlush();
}

void CommentBroker::newCleanFlush()
{
    for(auto iter = m_newClean.begin(); iter != m_newClean.end();){

        //应该保证当进行插入时，数据是不可以被其他线程所更改的
        std::lock_guard<std::mutex> lk(m_mutex);
        std::cout<<"comment id:"<<iter->second.id()<<std::endl;
        std::string command="insert into Comment (C_id,C_content,C_time,N_id,J_id) values("+iter->second.id()+",'"+iter-> second.content()+"','"+iter->second.time()+"',"+iter->second.publisherId()+","+iter->second.jottingId()+")";

        std::cout<<command<<std::endl;
        RelationalBroker::insert(command);

        //从缓存中删除相关数据
        std::cout<<"从缓存中删除"<<std::endl;
        //unordered_map这种链表型容器erase后返回的迭代器为当前的位置
        m_newClean.erase(iter++);
    }
}

Comment *CommentBroker::inCache(std::string id)
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
