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
    int closeSocket();

    nlohmann::json receiveMessage();
    bool sendMessage(char *buf, size_t size);
    bool sendFile(char *buf,size_t size,std::string filePath);
    ~Network();
private:
    int m_socketFd;

};

#endif // NETWORK_H
