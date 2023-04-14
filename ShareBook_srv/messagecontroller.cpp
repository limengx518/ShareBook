#include "messagecontroller.h"
#include "netizen.h"
#include "netizenbroker.h"
#include "messagesequence.h"

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
    return netizen.getJottingNotification();
}

void MessageController::NotificationFlush()
{
    std::cout<<"消息推送"<<std::endl;
    MessageSequence::getInstance()->updateMessageQueue();
}

MessageController::MessageController()
{
    m_myTimer.start(30000,NotificationFlush);
}
