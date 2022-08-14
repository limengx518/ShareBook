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

Server::Server()
{

}

void Server::start()
{
    //创建套接字
    m_network.Create();
    m_network.Bind();
    m_network.Listen();
    int nready;
    while(1){
        if(m_network.Poll()){
            int connfd = m_network.Accept();
            if(connfd<0) continue;
            m_threadPool.submit(std::bind(&Server::processClientRequest, this, connfd));
        };
    }

    m_network.Accept();
}

void Server::processClientRequest(int fd)
{
    nlohmann::json j = m_network.receiveMessage(fd);
    if(j.empty()){
        return ;
    }
//    ScanAndCheckJottingController *controller = ControllerFactory::getInstance()->createScanAndCheckJottingController();
//    json js = controller->pushJottings();
//    std::string s = js.dump();
//    std::cout<<s<<"  "<<s.length()<<"  "<<std::endl;
//    m_network.sendMessage(s.data(),s.length()*sizeof(char),fd);

    std::string request = j["request"];
    //Controller *controller;
    if(request == "ScanJottings"){
        ScanAndCheckJottingController *controller = ControllerFactory::getInstance()->createScanAndCheckJottingController();
        json j = controller->pushJottings();
        std::string s = j.dump();
        m_network.sendMessage(s.data(),s.length()*sizeof(char),fd);
    }else if(request == "publishJotting"){
        PublishJottingController *controller = ControllerFactory::getInstance()->createPublishJottingController();
    }
}
