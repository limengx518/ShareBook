#include "messagecontroller.h"
#include "netizen.h"
#include "netizenbroker.h"
#include "messagesequence.h"
#include "jottingbroker.h"

MessageController* MessageController::m_instance = nullptr;
MessageController *MessageController::getInstance()
{
    if(m_instance==nullptr){
        m_instance = new MessageController();
    }
    return m_instance;
}

nlohmann::json MessageController::getJottingNotification(std::string netizenId)
{
    Netizen &netizen=NetizenBroker::getInstance()->findById(netizenId);
    //更新这个人所拥有的消息
    NotificationFlush(netizenId);
    nlohmann::json messages;
    std::vector<std::string> jottingId = netizen.getJottingNotification();
    for(auto &item:jottingId){
        Jotting &jot = JottingBroker::getInstance()->findById(item);
        messages.push_back(jot.getMessageAbstract());
    }
    return messages;
}

void MessageController::NotificationFlush(std::string netizenId)
{
//    std::cout<<"消息推送"<<std::endl;
    std::cout<<"更新这个人所拥有的消息"<<std::endl;
    MessageSequence::getInstance()->updateMessageQueue(netizenId);
}

MessageController::MessageController()
{
//    m_myTimer.start(30000,NotificationFlush);
}
