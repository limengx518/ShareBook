#include "jottingnotification.h"
#include <iostream>

//JottingNotification::JottingNotification(std::string id, std::string senderId, std::string content, std::string time, std::string jottingId,bool expire):
//    m_id{id},
//    m_senderId{senderId},
//    m_content{content},
//    m_time{time},
//    m_jottingId{jottingId},
//    m_expire{expire}
//{

//}

JottingNotification::JottingNotification(std::string id,std::string senderId,std::unordered_map<std::string, NetizenProxy> recipients, std::string content, std::string time, std::string jottingId):
    m_id{id},
    m_senderId{senderId},
    _recipients{recipients},
    m_content{content},
    m_time{time},
    m_jottingId{jottingId}
{

}

void JottingNotification::registerSubscriber(const std::string recipientId)
{
    _recipients.insert({recipientId,NetizenProxy(recipientId)});
}

void JottingNotification::removeSubscriber(const std::string recipientId)
{
    _recipients.erase(recipientId);
}

void JottingNotification::notify(std::string netizenId)
{
//    if(_recipients.find(netizenId)!=_recipients.end()){
//        _recipients[netizenId].updateMessage(m_jottingId);
//    }
    for(auto& subscriber:_recipients){
        if(subscriber.first==netizenId){
            std::cout<<"与这个网民有关的消息进行更新"<<std::endl;
            subscriber.second.updateMessage(m_jottingId);
        }
    }
}

bool JottingNotification::isReadByAll()
{
    return _recipients.empty();
}

const std::string JottingNotification::id() const
{
    return m_id;
}

const std::string &JottingNotification::senderId() const
{
    return m_senderId;
}

const std::unordered_map<std::string, NetizenProxy> &JottingNotification::recipients() const
{
    return _recipients;
}

const std::string &JottingNotification::content() const
{
    return m_content;
}

const std::string &JottingNotification::time() const
{
    return m_time;
}

const std::string &JottingNotification::jottingId() const
{
    return m_jottingId;
}

bool JottingNotification::expire() const
{
    return m_expire;
}


