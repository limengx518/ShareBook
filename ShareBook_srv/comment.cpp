#include "comment.h"

#include <nlohmann/json.hpp>
#include <iostream>
#include <functional>
#include <utility>

using json = nlohmann::json;

//Comment::Comment(const std::string &tid, std::string netizenId):
//    CommentInterface(tid),_netizenProxy(netizenId),m_content{}
//{
//}

Comment::Comment(const std::string id, std::string content,std::string time, std::string netizenId, std::string jottingId):
    CommentInterface(id),
    _netizenProxy(netizenId),
    _jottingProxy(jottingId),
    m_content{content},
    m_time{time}
{

}


nlohmann::json Comment::getInfo()
{
    json commentInfo;
    commentInfo["content"]=m_content;
    commentInfo["time"]=m_time;
    commentInfo["netizen"]=_netizenProxy.getAbstract();
    return commentInfo;
}


const std::string Comment::content() const
{
    return m_content;
}

const std::string Comment::publisherId() const
{
    return _netizenProxy.id();
}

const std::string Comment::jottingId() const
{
    return _jottingProxy.id();
}

const std::string Comment::time()
{
    return m_time;
}
