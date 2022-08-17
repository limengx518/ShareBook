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
#include "string.h"
#include <stdio.h>


#define MAXLINE 10000

Server::Server()
{
    m_scanAndCheckJottingController = ControllerFactory::getInstance()->createScanAndCheckJottingController();
    m_publishJottingController = ControllerFactory::getInstance()->createPublishJottingController();
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
            json j = network.receiveMessage(connfd);
            m_threadPool.submit(std::bind(&Server::processClientRequest,this, connfd,j));
        };
    }

    network.closeSocket();
}

void Server::sendMessage(char *buf, size_t size, int &connfd)
{
    Network network(connfd);
    network.sendMessage(buf,size,connfd);
}

void Server::sendFile(char *buf, size_t size, int &connfd,std::string filePath)
{
    Network network(connfd);
    network.sendFile(buf,size,connfd,filePath);
}

void Server::processClientRequest(int& fd,json message)
{
    try {
        if(message.empty()){
            std::cout<<"Server receieve nullptr"<<std::endl;
            return;
        }
        std::string request = message["request"];
        std::cout<<"Server recieve request: "<<request<<std::endl;
        if(request == "ScanJottings"){
            //获得要推送的笔记 json信息
            json j = m_scanAndCheckJottingController->pushJottings();
            std::string s = j.dump();
            std::cout<<"pushJotting: "<<s<<std::endl;
            sendMessage(s.data(),s.length()*sizeof(char),fd);

        }else if(request == "PublishJottings"){
            std::string  isPub=m_publishJottingController->publishJottings(message);
            sendMessage(isPub.data(),isPub.length()*sizeof(char),fd);
        }
    }  catch (...) {
        std::cout<<"client error"<<std::endl;//确保当其中一个线程异常时不会影响另外的线程
    }
}

void Server::processClientFileRequest(int &fd, std::string filePath)
{
    Network network;
    std::string str = network.receiveFile(fd,filePath);

}
