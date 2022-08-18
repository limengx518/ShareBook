#ifndef CLIENT_H
#define CLIENT_H

#include "nlohmann/json.hpp"
#include "network.h"


class Client
{
public:
    Client(char *ipaddr);
    void send(char *buf, size_t size);
    void sendPic(char *buf, size_t size);
    void sendFile(char *buf, size_t size, std::string filePath);
    std::string receive();
    ~Client();
private:
    Network  m_network;
};

#endif // CLIENT_H
