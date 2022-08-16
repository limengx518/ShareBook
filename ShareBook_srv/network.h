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

class NetWork
{
public:
    NetWork();
    int createSocket();
    int bindSocket();
    int listenSocket();
    int acceptSocket();
    int Poll();
    void closeSocket();
    nlohmann::json receiveMessage(int &connfd);
    bool sendMessage(char *buf, size_t size, int &connfd);

    void Writen(int& fd, void *ptr, size_t nbytes);

private:
    int m_listenFd;

    ssize_t writen(int& fd, const void *vptr, size_t n);


};

#endif // NETWORK_H
