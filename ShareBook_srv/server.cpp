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
    network.createSocket();
    network.bindSocket();
    network.listenSocket();
    int nready;
    while(1){
        if(network.Poll()){
            int connfd = network.acceptSocket();
            if(connfd<0) continue;
            m_threadPool.submit(std::bind(&Server::processClientRequest,this, connfd));
        };
    }

    network.closeSocket();
}

void Server::processClientRequest(int& fd)
{
    try {
        NetWork network;
        json j = network.receiveMessage(fd);

        if(j.empty()){
            std::cout<<"receieve nullptr"<<std::endl;
            return;
        }
//        std::cout<<j.dump()<<std::endl;
        std::string request = j["request"];
        if(request == "ScanJottings"){
            ScanAndCheckJottingController *controller = ControllerFactory::getInstance()->createScanAndCheckJottingController();
            json j = controller->pushJottings();
            std::string s = j.dump();
            network.sendMessage(s.data(),s.length()*sizeof(char),fd);
            close(fd);
        }else if(request == "PublishJottings"){
            PublishJottingController *controller = ControllerFactory::getInstance()->createPublishJottingController();
            std::string  isPub=controller->publishJottings(j);
            network.sendMessage(isPub.data(),isPub.length()*sizeof(char),fd);
            close(fd);
        }
    }  catch (...) {
        std::cout<<"client error"<<std::endl;//确保当其中一个线程异常时不会影响另外的线程
    }
}
