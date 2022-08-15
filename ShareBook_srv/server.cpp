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


#define MAXLINE 1000

Server::Server()
{

}

void Server::start()
{
    //创建套接字
    NetWork network;
    network.closeSocket();
    network.createSocket();
    network.bindSocket();
    network.listenSocket();
    network.acceptSocket();
    while(1){
        if(network.Poll()){
            int connfd = network.acceptSocket();
            if(connfd<0) continue;
            json j = network.receiveMessage(connfd);
            if(j.empty())  continue;
            m_threadPool.submit(std::bind(&Server::processClientRequest, this, connfd,j));
            processClientRequest(connfd,j);
            close(connfd);
        };
    }
}

void Server::processClientRequest(int fd,json j)
{
    NetWork network;
    std::string request = j["request"];
    if(request == "ScanJottings"){
        ScanAndCheckJottingController *controller = ControllerFactory::getInstance()->createScanAndCheckJottingController();
        json j = controller->pushJottings();
        std::string s = j.dump();
        network.sendMessage(s.data(),s.length()*sizeof(char),fd);
        close(fd);


    }else if(request == "publishJotting"){
        PublishJottingController *controller = ControllerFactory::getInstance()->createPublishJottingController();
    }
}
