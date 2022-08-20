/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       network.h
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @data       2022-08-19
 * @history
 *****************************************************************************/
#ifndef NETWORK_H
#define NETWORK_H

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

class Network {
public:
    Network();
    int createSocket();
    int connectSocket(const char *ipaddr);
    int closeSocket();

//    std::string receiveMessage();

//    bool sendMessage(char *buf, size_t size);
    bool receiveMessage(char* buffer);
    bool sendMessage(const char *buf, size_t size);
    bool sendFile(char *buf,size_t size,std::string filePath);
    ~Network();
private:
    int m_sockfd;

};

#endif // NETWORK_H
