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
    int connectSocket(char *ipaddr);

    nlohmann::json receiveMessage();
    bool sendMessage(char *buf, size_t size);

    void Writen(int fd, void *ptr, size_t nbytes);
    int closeSocket();

private:
    int m_socketFd;
    ssize_t writen(int fd, const void *vptr, size_t n);

};

#endif // NETWORK_H
