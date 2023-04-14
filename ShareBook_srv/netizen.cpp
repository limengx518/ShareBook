#include "netizen.h"
#include "comment.h"
#include "time.h"
#include "jottingbroker.h"
#include "netizenbroker.h"
#include "jottingproxy.h"
#include "timeInfo.h"
#include <unordered_map>
#include <iostream>
#include "commentproxy.h"
#include "commentbroker.h"
#include <iostream>
#include <fstream>
#include "jottingnotification.h"
#include "messagesequence.h"
#include "snowflakeidworker.h"
#include "material.h"
#include "materialbroker.h"
#include "encodephoto.h"
#include "test.h"

using json = nlohmann::json;

Netizen::Netizen(const std::string &tid):
    NetizenInterface(tid)
{
    _messages.insert("1");
}

Netizen::Netizen(const std::string id, std::string nickName, std::string signal, std::string avatar, std::vector<std::string> jottingId, std::vector<std::string> fansId, std::vector<std::string> concernedsId, std::vector<std::string> commentdId):
    NetizenInterface(id),m_nickName(nickName),m_signal(signal),m_avatar(avatar)
{
    for(auto &jId:jottingId){
         _jottings.insert(std::pair<std::string,JottingProxy>(jId, JottingProxy(jId)));
    }
    for(auto &fId:fansId){
         _fans.insert(std::pair<std::string,NetizenProxy>(fId, NetizenProxy(fId)));
    }
    for(auto &cId:concernedsId){
        _concerneds.insert(std::pair<std::string,NetizenProxy>(cId, NetizenProxy(cId)));
    }
    for(auto &cId:commentdId){
        _comments.insert(std::pair<std::string,CommentProxy>(cId, CommentProxy(cId)));
    }
}

nlohmann::json Netizen::getInfo()
{
//    std::cout<<"getInfo\n";
    json netizenInfo;
    netizenInfo["name"]=m_nickName;
    netizenInfo["signal"]=m_signal;
    if(strcmp(TEST_TYPE,"PATH")){
        netizenInfo["avatar"]= encodePhoto(m_avatar);
    }else{
         netizenInfo["avatar"] = m_avatar;
    }

    netizenInfo["fanCnt"] = _fans.size();
    netizenInfo["interestCnt"] = _concerneds.size();

    json jottingInfo;
    for(auto &jp:_jottings){
        json jotting=jp.second.getOnePicAbstract();
        netizenInfo["jottings"].push_back(jotting);
    }

    netizenInfo["fansInfo"] = getFansInfo();
//    if(netizenInfo["fansInfo"].empty()){
//        netizenInfo["fansInfo"] = " ";
//    }
    netizenInfo["concernedInfo"] = getConcernedInfo();
//    if(netizenInfo["concernedInfo"].empty()){
//        netizenInfo["concernedInfo"]= " ";
//    }
//    netizenInfo["jottings"].push_back(jottingInfo);
//    for(auto &fp:_fans){
//       json netizenAbstract=fp.second.getAbstract();
//       netizenInfo["fans"][fp.first]["nickName"]=netizenAbstract["nickName"];
//    }
//    for(auto &cp:_concerneds){
//       json netizenAbstract=cp.second.getAbstract();
//       netizenInfo["concerneds"][cp.first]["nickName"]=netizenAbstract["nickName"];
//    }


//    std::cout<<netizenInfo.dump(4)<<std::endl;

    return netizenInfo;
}

nlohmann::json Netizen::getFansInfo()
{
    json fansInfo;
    for(auto &fp:_fans){
        fansInfo.push_back(fp.second.getAbstract());
    }
    return fansInfo;
}

nlohmann::json Netizen::getConcernedInfo()
{
    json concernedsInfo;
    for(auto &cp:_concerneds){
      concernedsInfo.push_back(cp.second.getAbstract());
    }
    return concernedsInfo;

}

nlohmann::json Netizen::getJottingNotification()
{
    json jottingNotification;
    for(auto &item:_messages){
        jottingNotification["messages"].push_back(item);
    }
    return jottingNotification;
}

const string Netizen::avatarPath() const
{
    return m_avatar;
}

const std::string Netizen::nickName()const
{
    return m_nickName;
}

const std::vector<std::string> Netizen::jottings() const
{
    std::vector<std::string> jottings;
    for(const auto& item:_jottings){
        jottings.push_back(item.first);
    }
    return jottings;
}

const std::vector<std::string> Netizen::fans() const
{
    std::vector<std::string> fans;
    for(const auto& item:_fans){
        fans.push_back(item.first);
    }
    return fans;
}

const std::vector<std::string> Netizen::concerneds() const
{
    std::vector<std::string> concerneds;
    for(const auto& item:_concerneds){
        concerneds.push_back(item.first);
    }
    return concerneds;
}

const std::vector<std::string> Netizen::commentsId() const
{
    std::vector<std::string> comments;
    for(const auto& item:_comments){
        comments.push_back(item.first);
    }
    return comments;
}

nlohmann::json Netizen::getAbstract()
{
    json j;
    j["nickName"]=m_nickName;
    j["signal"] = m_signal;
    if(strcmp(TEST_TYPE,"PATH")){
        j["avatar"] = encodePhoto(m_avatar);
    }else{
        j["avatar"] = m_avatar;
    }
//    j["avatar"] = encodePhoto(m_avatar);

    //得到一个独一无二的Id
//    std::string tmp = m_avatar.substr(23,m_avatar.size());
//    tmp=tmp.substr(0,tmp.size()-4);
//    j["avatarId"] = tmp;

    j["avatarId"] = m_avatar;
    return j;
}

const std::string Netizen::readLog()
{
    std::string line,last_line;
    int lineNumber=0,i=0;
    std::ifstream logFile;
    //打开log.txt文件
    logFile.open("/temp/log.txt");

    //得到所有的行数
    while(std::getline(logFile,line))
        lineNumber++;

    //修改状态标志
    logFile.clear();
    //设置输入位置指示器
    logFile.seekg(0);

    //得到倒数第二行中的内容
    while(std::getline(logFile,last_line) && i<lineNumber-2)
        i++;

    //关闭文件
    logFile.close();
//    std::cout<<"line="<<last_line<<std::endl;
    last_line = last_line.substr(4,last_line.length()-1);
    return last_line;
}

void Netizen::writeLog()
{
    std::string time_string=getTime();
    std::ofstream logFile;
    //打开log.txt文件，记录本次登陆时间
    logFile.open("/temp/log.txt",std::ios::app|std::ios::out);
    logFile<<"in:"<<time_string<<std::endl;
    logFile.close();
}

nlohmann::json Netizen::scanJottings()
{
    //根据时间差推送笔记
    std::vector<JottingProxy> jottingProxys=JottingBroker::getInstance()->pushJottings(id(),readLog(),getTime());
    json jottings;
    jottings["netizenId"] = id();
    jottings["nickName"] = m_nickName;
    if(strcmp(TEST_TYPE,"PATH")){
        jottings["avatar"] = encodePhoto(m_avatar);
    }else{
        jottings["avatar"] = m_avatar;
    }
    jottings["avatarId"] = m_avatar;
    for(auto &item:jottingProxys){
        jottings["jottings"].push_back(item.getAbstract());
    }
    return jottings;
}

nlohmann::json Netizen::checkOneJotting(std::string jottingId)
{
    Jotting jotting=JottingBroker::getInstance()->findById(jottingId);
    return jotting.getDetail();
}

nlohmann::json Netizen::checkInfoOneJotting(std::string jottingId)
{
    Jotting jotting=JottingBroker::getInstance()->findById(jottingId);
    return jotting.getInfoDetail();
}

bool Netizen::comment(const std::string content, const std::string jottingId)
{
    //获取创建时间
    std::string time=getTime();

    //创建comment的id
    unsigned int comment_id=Singleton<IdWorker>::instance().nextId();

    //创建笔记对象
    Comment *comment=new Comment(std::to_string(comment_id),content,time,id(),jottingId);

    std::cout<<"comment id:"<<comment->id()<<std::endl;
    //将笔记对象存入newCleanCache缓存
    CommentBroker::getInstance()->addComment(*comment);

    //建立netizen和comment的联系
    _comments.insert(std::pair<std::string,CommentProxy>(comment->id(), CommentProxy(comment->id())));

    //建立jotting和comment的联系
    Jotting& jotting=JottingBroker::getInstance()->findById(jottingId);
    jotting.comment(comment->id());

//    delete comment;
    return true;
}

bool Netizen::publishJotting(nlohmann::json jotting_json)
{

    std::vector<std::string> comments;
    std::vector<Material> materials;
    std::vector<std::string> materialsId;
    //获取创建时间
    std::string time=getTime();
    //创建jotting的id
    std::string jotting_id=std::to_string((unsigned int)Singleton<IdWorker>::instance().nextId());

    //这里还需要根据所创建的文件类型进行再次的修改
    auto content=jotting_json["content"].get<std::string>();
    for(auto& material:jotting_json["materials"]){
        std::string path=material.get<std::string>();
        std::string id=std::to_string((unsigned int)Singleton<IdWorker>::instance().nextId());
        int type=1;
        materialsId.push_back(id);
        materials.push_back(Material(id,jotting_id,path,type));
    }

    //创建笔记对象
    Jotting jotting(jotting_id,content,time,id(),materialsId,comments);

    //建立netizen和jotting的联系
    _jottings.insert(std::pair<std::string,JottingProxy>(jotting.id(),JottingProxy(jotting.id())));

    //将笔记对象和material对象存入newCleanCache缓存
    JottingBroker::getInstance()->addJotting(jotting);
    for(auto &material:materials){
        MaterialBroker::getInstance()->addMaterial(material);
    }


    //发送给所有粉丝“发布笔记”的消息
    std::string message_content="你关注的人有新的笔记";
    //创建jotting的id
    unsigned int message_id=Singleton<IdWorker>::instance().nextId();
    //将JottingNotification加入消息队列中
    MessageSequence::getInstance()->pushNotification(JottingNotification(std::to_string(message_id),id(),_fans,message_content,time,jotting.id()));

    return true;
}

void Netizen::updateMessage(std::string messageId)
{
    _messages.insert(messageId);
}

bool Netizen::isOnline()
{
    return m_online;
}

void Netizen::setOnline(bool online)
{
    m_online=online;
}

void Netizen::followNetizen(std::string concernedId)
{
    //建立关联
    _concerneds.insert(std::pair<std::string,NetizenProxy>(concernedId,NetizenProxy(concernedId)));

    //找到所关注的人
    Netizen concerned=NetizenBroker::getInstance()->findById(concernedId);
    //建立关联(涨粉)
    concerned.growFan(id());

    //存入newCleanCache
    NetizenBroker::getInstance()->addFollowRelation(concernedId,id());
}

nlohmann::json Netizen::scanMessages()
{
    json messages;
    for(auto &messageId:_messages){
        JottingNotification *notification=MessageSequence::getInstance()->findById(messageId);
        json message;
        message["id"]=messageId;
        message["senderName"]=NetizenBroker::getInstance()->findById(notification->senderId()).nickName();
        message["content"]=notification->content();
        message["time"]=notification->time();
        messages.push_back(message);
    }
    return messages;
}

nlohmann::json Netizen::checkMessage(std::string messageId)
{
    //删除查看的消息关联
    _messages.erase(messageId);

    //查找message所指定的笔记id
    JottingNotification *notification=MessageSequence::getInstance()->findById(messageId);
    std::string jottingId=notification->jottingId();

    //删除此消息里订阅者中该netizen的id,防止下次再次发送相同消息给此netizen
    MessageSequence::getInstance()->removeMessageSubscriber(messageId,id());

    //返回笔记的详情内容
    return checkOneJotting(jottingId);
}

void Netizen::growFan(std::string id)
{
    _fans.insert({id,NetizenProxy(id)});
}

