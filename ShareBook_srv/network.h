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
#define SERV_PORT 9878
#define RTP_PORT 9888
#define SERV_IP "192.168.205.249"

class Network
{
public:
    Network();
    Network(int &fd);
    int createSocket();
    int createUdpSocket();
    int bindSocket(char* ip=SERV_IP, int port=SERV_PORT);
    int listenSocket();
    int acceptSocket(char* ipaddr, int &port);
    void closeSocket();
    int pollSocket();
    int sendn(void* buff,int dataSize);
    int receiven(char* buf,int bufSize);
    int sendUdp(const void *buf, size_t n, char *ip, int port);
    int receiveUdp(void *buf, size_t n, std::string ip, int port);
    ~Network();


    int sendMessage(char *buf, size_t size);
    bool receiveMessage(char* buffer);

    int sendFile(char *buf, size_t size, std::string filePath);
    std::string receiveFile(std::string filePath);
private:
    int m_listenFd;
};

#endif // NETWORK_H
