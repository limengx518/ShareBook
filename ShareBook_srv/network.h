/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       network.h
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <nlohmann/json.hpp>

#define INVALID_SOCKET_FD -1

class Network
{
public:
    Network();
    Network(int &fd);
    int createSocket();
    int bindSocket();
    int listenSocket();
    int acceptSocket(char* ipaddr, int size);
    void closeSocket();
    int pollSocket();
    int sendn(void* buff,int dataSize);
    int receiven(char* buf,int bufSize);
    ~Network();


    int sendMessage(char *buf, size_t size);
    bool receiveMessage(char* buffer);

    int sendFile(char *buf, size_t size, std::string filePath);
    std::string receiveFile(std::string filePath);
private:
    int m_listenFd;
};

#endif // NETWORK_H
