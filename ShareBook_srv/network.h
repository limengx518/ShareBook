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
    int Create();
    int Bind();
    int Listen();
    int Accept();
    int Poll();
    void receiveMessage(nlohmann::json j,int connfd);

private:
    int m_listenFd;

};

#endif // NETWORK_H
