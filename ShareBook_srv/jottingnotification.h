#ifndef JOTTINGNOTIFICATION_H
#define JOTTINGNOTIFICATION_H

#include <iostream>
#include <unordered_map>
#include "netizenproxy.h"

class JottingNotification
{
public:
//    explicit JottingNotification(std::string id,std::string senderId,std::string content,std::string time,std::string jottingId,bool expire);
    explicit JottingNotification(std::string id,std::string senderId,std::unordered_map<std::string, NetizenProxy> recipients,std::string content,std::string time,std::string jottingId);

    //注册订阅者
    void registerSubscriber(const std::string recipientId);
    //删除订阅者
    void removeSubscriber(const std::string recipientId);
    //通知具体的订阅者
    void notify(std::string netizenId);

    bool isReadByAll();

    const std::string id() const;
    const std::string &senderId() const;
    const std::unordered_map<std::string, NetizenProxy> &recipients() const;
    const std::string &content() const;
    const std::string &time() const;
    const std::string &jottingId() const;
    bool expire() const;

private:
    std::string m_id;
    std::string m_deadline;
    std::string m_senderId;
    std::unordered_map<std::string, NetizenProxy> _recipients;
    std::string m_content;
    std::string m_time;
    std::string m_jottingId;
    bool m_expire=false;
};

#endif // JOTTINGNOTIFICATION_H
