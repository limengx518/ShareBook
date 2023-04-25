#ifndef MESSAGESEQUENCE_H
#define MESSAGESEQUENCE_H

#include <iostream>
#include <unordered_map>
#include "jottingnotification.h"
#include "relationalbroker.h"

class MessageSequence:public RelationalBroker
{
public:
    static MessageSequence* getInstance();

    //加入指定message
    void pushNotification(JottingNotification newNotification);
    //删除指定message
    void removeNotification(std::string id);
    //获取指定message
    JottingNotification* findById(std::string id);

    //更新消息队列
    void updateMessageQueue(std::string netizenId);
    //移除指定message的某个订阅者
    void removeMessageSubscriber(std::string messageId,std::string subscriberId);

    virtual ~MessageSequence();
private:
    MessageSequence();
    static MessageSequence* m_messageSequence;

    std::unordered_map<std::string,JottingNotification> m_messageQueue;

};

#endif // MESSAGESEQUENCE_H
