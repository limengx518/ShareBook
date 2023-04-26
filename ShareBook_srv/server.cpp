/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       server.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#include "server.h"
#include "encodephoto.h"
#include "string.h"
#include <stdio.h>

#define MAXLINE 9999999

Server::Server()
{
    m_scanAndCheckJottingController = ControllerFactory::getInstance()->createScanAndCheckJottingController();
    m_publishJottingController = ControllerFactory::getInstance()->createPublishJottingController();
    m_initController = ControllerFactory::getInstance()->createInitController();
    m_cacheFlush = ControllerFactory::getInstance()->createCacheFlush();
    m_messageController = ControllerFactory::getInstance()->createMessageController();
}

void Server::start()
{
    //创建套接字
    Network network;
    network.createSocket();
    network.bindSocket();
    network.listenSocket();
    while(1){
        if(network.pollSocket()){
            int connfd = network.acceptSocket();
            if(connfd<0) continue;
            m_threadPool.submit(std::bind(&Server::processClientRequest,this,connfd));
        };
    }

    network.closeSocket();
}

bool Server::receive(int &connfd,char * buf)
{
    Network network(connfd);
    return network.receiveMessage(buf);
}

void Server::send(char *buf, size_t size, int &connfd)
{
    Network network(connfd);
    network.sendMessage(buf,size);
}

void Server::sendFile(char *buf, size_t size, int &connfd,std::string filePath)
{
    Network network(connfd);
    network.sendFile(buf,size,filePath);
}

void Server::processClientRequest(int& fd)
{
    try {
        Network network(fd);
        char buf[MAXLINE];

        network.receiveMessage(buf);

        json message = json::parse(buf);
        std::cout<<"Server<< 消息长度："<<strlen(buf)<<std::endl;
        if(message.empty()){
            std::cout<<"Server receieve nullptr"<<std::endl;
            return;
        }
        std::string request = message["request"];
        std::string netizenId = message["id"];
        std::cout<<"Server recieve request: "<<request<<std::endl;

        if(request == "ScanJottings"){
            std::cout<<"    ----Netizen "<<netizenId<<" : ScanJottings----    "<<std::endl;
            //获得要推送的笔记 json信息
            json j = m_scanAndCheckJottingController->pushJottings(netizenId);
            std::string s = j.dump(4);
            std::cout<<s<<std::endl;
            network.sendMessage(s.data(),s.size());
//            send(s.data(),s.size(),fd);
            std::cout<<" ----ScanJottings ends----     "<<std::endl;

        }else if(request == "PublishJotting"){
            std::cout<<"    ----Netizen "<<netizenId<<" : PublishJotting----    "<<std::endl;
            std::string  isPub=m_publishJottingController->publishJottings(netizenId,message["jotting"]);
            std::cout<<isPub<<std::endl;
            network.sendMessage(isPub.data(),isPub.size());
//            send(isPub.data(),isPub.size(),fd);
            std::cout<<"Server << 接收笔记信息 发布成功!"<<std::endl;

        }else if(request == "InitPersonalInfo"){
            std::cout<<"    ----Netizen "<<netizenId<<" : InitPersonalInfo----    "<<std::endl;
            json j = m_initController->getInfo(netizenId);
            std::string s = j.dump(4);
            std::cout<<s<<std::endl;
//            send(s.data(),s.size(),fd);
            network.sendMessage(s.data(),s.size());
            std::cout<<"    ----Init final----  "<<std::endl;
        }else if(request == "GetJottingDetail"){
            std::string jottingId = message["jottingId"];
            std::cout<<"    ----Netizen "<<netizenId<<" GetJottingDetail "<<jottingId<<"----    "<<std::endl;
            json j = m_scanAndCheckJottingController->pushJottingDetial(netizenId,jottingId);
            std::string s = j.dump(4);
            std::cout<<s<<std::endl;
//            send(s.data(),s.size(),fd);
            network.sendMessage(s.data(),s.size());
            std::cout<<"   ---- GetJottingDetail end----    "<<std::endl;

        }else if(request == "GetPicture"){
//            std::string picPath = message["picPath"];
//            std::cout<<"    ----Netizen "<<netizenId<<" : GetPicture,Path:"<<picPath<<"----    "<<std::endl;
////            network.sendFile(picPath);
//            std::string pic = encodePhoto(picPath);
////            std::cout<<pic<<std::endl;
//            std::cout<<pic.size()<<std::endl;
//            network.sendMessage(pic.data(),pic.size());
//            std::cout<<"Send Picture end"<<std::endl;
        }else if(request == "Comment"){
            std::cout<<"    ----Netizen "<<netizenId<<" : Receive Comment---- "<<std::endl;
            std::string  comment = message["text"];
            std::string jottingId = message["jottingId"];
            std::cout<<"jottingId:"<<jottingId<<"\n";
            std::cout<<"comment:"<<comment<<"\n";
            m_publishJottingController->comment(netizenId,comment,jottingId);
            std::cout<<"   ----Receive end----"<<std::endl;
        }else if(request == "GetInfoJottingDetail"){ //个人主页中的笔记详情
            std::string jottingId = message["jottingId"];
            std::cout<<"    ----Netizen "<<netizenId<<" : GetInfoJottingDetail----    "<<jottingId<<std::endl;
            json j = m_scanAndCheckJottingController->pushInfoJottingDetail(netizenId,jottingId);
            std::string s = j.dump(4);
            std::cout<<s<<std::endl;
            network.sendMessage(s.data(),s.size());
//            send(s.data(),s.size(),fd);
            std::cout<<"   ---- GetInfoJottingDetail end----    "<<std::endl;
        }else if(request =="GetMessage"){
            std::cout<<"    ----Netizen "<<netizenId<<" : GetMessage----    "<<std::endl;
            json j = m_messageController->getJottingNotification(netizenId);
            std::string s = j.dump(4);
            std::cout<<s<<std::endl;
            network.sendMessage(s.data(),s.size());
//            send(s.data(),s.size(),fd);
            std::cout<<"   ---- GetMessage end----    "<<std::endl;
        }else if(request == "ScanVideos"){
            std::cout<<"    ----Netizen "<<netizenId<<" : ScanVideos----    "<<std::endl;
            json j = m_scanAndCheckJottingController->scanVideos(netizenId);
            std::string s = j.dump(4);
            std::cout<<s<<std::endl;
            network.sendMessage(s.data(),s.size());
            std::cout<<"   ---- ScanVideos end----    "<<std::endl;
        }
    }  catch (...) {
        std::cout<<"client error"<<std::endl;//确保当其中一个线程异常时不会影响另外的线程
    }
}

void Server::processClientFileRequest(int &fd, std::string filePath)
{
    Network network(fd);
    std::string str = network.receiveFile(filePath);

}
