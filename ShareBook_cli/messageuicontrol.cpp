#include "messageuicontrol.h"


#include "base64.h"
#include "client.h"
#include "clientInfo.h"
#include "imageprovider.h"
#include "qpixmap.h"

#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
extern ImageProvider *imageProvider;


MessageUIControl::MessageUIControl(QObject *parent):QObject(parent)
{

}

void MessageUIControl::scanMessage()
{
    std::cout<<"进入函数scanMessage"<<std::endl;
    //发送浏览笔记请求 请求服务端推送笔记
    nlohmann::json message = {
        {"id",ID},
        {"request","GetMessage"}
    };

    std::string sendData = message.dump();
    Client::getInstance()->reconnect();
    Client::getInstance()->send(sendData.c_str(),sendData.length());
    std::cout<<"Client"<<"客户端获取消息通知......"<<std::endl;
    char *receiveData = new char[10000000];
    Client::getInstance()->receive(receiveData);


    //获取到的网民基本信息 以 json形式
    nlohmann::json mes = nlohmann::json::parse(receiveData);
    std::cout<<mes.dump(4)<<std::endl;

     json receiveNotifications;
    QHash<QString,QPixmap> avatars;
    for(auto &info:mes){
        json oneNotification;

        //笔记作者头像信息
        std::string avatarId = info["netizen"]["avatarId"];
        string avatarPath = "image://photos/"+avatarId;
        std::string avatarData=base64_decode(info["netizen"]["avatar"]);
        QPixmap pixmap;
        pixmap.loadFromData((unsigned char *)avatarData.data(),avatarData.length());
        avatars.insert(QString::fromStdString(avatarId),pixmap);
        oneNotification["avatar"] = avatarPath;

        oneNotification["nickName"] = info["netizen"]["nickName"];

        string str = info["netizen"]["nickName"];

        std::string content = " 您关注的 " +str + " 发布了新的笔记！";
        oneNotification["content"] = content;

        oneNotification["jottingId"] = info["jottingid"];
//        oneNotification["jottingId"] = "1";
        oneNotification["time"] = info["time"];

        receiveNotifications["notification"].push_back(oneNotification);
    }
    imageProvider->setMessageUIAvatars(avatars);

    cout<<receiveNotifications.dump(4)<<endl;

    m_notifications = QString::fromStdString(receiveNotifications.dump());
}

void MessageUIControl::setNotifications(const QString &notifications)
{
    m_notifications = notifications;
}

QString MessageUIControl::getNotifications() const
{
    return m_notifications;
}
