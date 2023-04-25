#include "messagesequence.h"

MessageSequence* MessageSequence::m_messageSequence=NULL;

MessageSequence *MessageSequence::getInstance()
{
    if(m_messageSequence==nullptr){
        m_messageSequence=new MessageSequence();
    }
    return m_messageSequence;
}

void MessageSequence::pushNotification(JottingNotification newNotification)
{
    std::cout<<"将消息加入到消息队列"<<std::endl;
    //加入消息给消息队列
    m_messageQueue.insert({newNotification.id(),newNotification});

//    //将消息存入数据库
//    std::string com="insert into JottingNotification (JN_id,JN_content,JN_time,JN_expire,N_id,J_id) values("+newNotification.id()+",'"+newNotification.content()+"','"+newNotification.time()+"',"+std::to_string(newNotification.expire())+","+newNotification.senderId()+","+newNotification.jottingId()+")";
//    RelationalBroker::insert(com);

//    //将消息与订阅者的关联存入数据库
//    for(auto& recipient:newNotification.recipients()){
//        std::string com_relation="insert into JottingNotificationRelation (JN_id,Recipient_id) values("+newNotification.id()+","+recipient.first+")";
//        RelationalBroker::insert(com_relation);
//    }
}

void MessageSequence::removeNotification(std::string id)
{
    m_messageQueue.erase(id);

    std::string com_relation="delete from JottingNotificationRelation where JN_id="+id;
    RelationalBroker::drop(com_relation);

    std::string com="delete from JottingNotification where JN_id="+id;
    RelationalBroker::drop(com);
}

JottingNotification *MessageSequence::findById(std::string id)
{
    return &m_messageQueue.at(id);
}

void MessageSequence::updateMessageQueue(std::string netizenId)
{
    //发送消息队列中所有与netizen有关的message
    for(auto& message:m_messageQueue){
        message.second.notify(netizenId);
    }

//    for(auto& message:m_messageQueue){
//        std::string com="select Recipient_id from JottingNotificationRelation where JN_id="+message.first;
//        sql::ResultSet* res=RelationalBroker::query(com);
//        while (res->next()) {
//            std::string id=std::to_string(res->getUInt(1));
//            if(message.second.recipients().count(id)==0){
//                std::string com_relation="delete from JottingNotificationRelation where JN_id="+message.first+" and Recipient_id="+id;
//                RelationalBroker::drop(com_relation);
//            }
//        }
//    }
}

void MessageSequence::removeMessageSubscriber(std::string messageId,std::string subscriberId)
{
    //删除队列中指定message的某个订阅者id（已查阅该message的netizen的id）
    m_messageQueue.at(messageId).removeSubscriber(subscriberId);

    //判断该消息是否全部订阅者查阅完毕
    if(m_messageQueue.at(messageId).isReadByAll()){
        std::cout<<"is empty"<<std::endl;
        //若全部查阅完毕，则删除该消息
        removeNotification(messageId);
    }
}

MessageSequence::~MessageSequence()
{

}

MessageSequence::MessageSequence()
{
    m_messageQueue.clear();

//    std::string com="select * from JottingNotification";
//    std::string id,senderId,jottingId,content,time;
//    bool expire;
//    sql::ResultSet* res=RelationalBroker::query(com);
//    while (res->next()) {
//        id=std::to_string(res->getUInt(1));
//        content=res->getString(2);
//        time=res->getString(3);
//        expire=res->getBoolean(4);
//        senderId=std::to_string(res->getUInt(5));
//        jottingId=std::to_string(res->getUInt(6));
//        m_messageQueue.insert({id,JottingNotification(id,senderId,content,time,jottingId,expire)});
//    }

//    for(auto& message:m_messageQueue){
//        std::string com_relation="select Recipient_id from JottingNotificationRelation where JN_id="+message.first;
//        std::string recipientId;
//        res=RelationalBroker::query(com_relation);
//        while (res->next()) {
//            recipientId=std::to_string(res->getUInt(1));
//            message.second.registerSubscriber(recipientId);
//        }
//    }
}
