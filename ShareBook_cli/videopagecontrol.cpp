#include "videopagecontrol.h"


#include "client.h"
#include "clientInfo.h"
#include "base64.h"
#include "imageprovider.h"

#include <iostream>
#include <ostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
extern ImageProvider *imageProvider;
VideoPageControl::VideoPageControl(QObject *parent):
    QObject(parent)
{

}

void VideoPageControl::pushRecommendVideos()
{
    nlohmann::json message = {
        {"id",ID},
        {"request","ScanVideos"}
    };

    std::string sendData = message.dump();
    Client::getInstance()->reconnect();
    Client::getInstance()->send(sendData.c_str(),sendData.length());
    std::cout<<"Client"<<"客户端获取用户推送视频的信息......"<<std::endl;
    char *receiveData = new char[10000000];
    Client::getInstance()->receive(receiveData);

    //获取到的网民基本信息 以 json形式
    nlohmann::json scanInfo = nlohmann::json::parse(receiveData);
//    std::cout<<scanInfo.dump(4)<<std::endl;

     json receiveVideos;
     //浏览的笔记的信息是一个大数组
     QHash<QString,QPixmap> avatars;
     QHash<QString,QPixmap> allPics;
     for(auto &info:scanInfo){
         json videoInfo;

//         videoInfo["id"] = info["id"];
         videoInfo["nickName"] = info["name"];
         videoInfo["content"] = info["content"];
         videoInfo["path"] = info["path"];

         //笔记作者头像信息
         std::string avatarId = info["avatarId"];
         string avatarPath = "image://photos/"+avatarId;
         std::string avatarData=base64_decode(info["avatar"]);
         QPixmap pixmap;
         pixmap.loadFromData((unsigned char *)avatarData.data(),avatarData.length());
         avatars.insert(QString::fromStdString(avatarId),pixmap);
         videoInfo["avatar"] = avatarPath;
         avatars.insert(QString::fromStdString(avatarId),pixmap);

         receiveVideos["videos"].push_back(videoInfo);

    }
     imageProvider->setVideoAvatars(avatars);

     cout<<receiveVideos.dump(4)<<endl;
     m_videos = QString::fromStdString(receiveVideos.dump());
     cout<<"视频个数："<<receiveVideos["videos"].size()<<endl;
     Client::getInstance()->closeSocket();

}

void VideoPageControl::setNickName(const QString &nickName)
{
    m_nickName=nickName;
}

QString VideoPageControl::getNickName() const
{
    return m_nickName;
}


void VideoPageControl::setAvatar(const QString &avatar)
{
    m_avatar=avatar;
    emit avatarChanged(m_avatar);
}

QString VideoPageControl::getAvatar() const
{
    return m_avatar;
}

void VideoPageControl::setVideos(const QString &videos)
{
    m_videos = videos;
}

QString VideoPageControl::getVideos() const
{
    return m_videos;
}
