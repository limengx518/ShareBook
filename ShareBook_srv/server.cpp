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

#define MAXLINE 1024

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
        char buf[1000000];
        network.receiveMessage(buf);
        json message = json::parse(buf);
        std::cout<<"Server<< 消息长度："<<strlen(buf)<<std::endl;

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
            std::cout<<s.size()<<std::endl;
            send(s.data(),s.size(),fd);

        }else if(request == "PublishJottings"){
            std::string  isPub=m_publishJottingController->publishJottings(message);
            std::cout<<isPub<<std::endl;
            send(isPub.data(),isPub.size(),fd);

            std::cout<<"Server << 接收笔记信息 发布成功!"<<std::endl;
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
