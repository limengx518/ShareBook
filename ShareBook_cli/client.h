#ifndef CLIENT_H
#define CLIENT_H

#include "nlohmann/json.hpp"
#include "network.h"


class Client
{
public:
    Client(const char *ipaddr);
    void send(const char *buf, size_t size);
    void sendFile(char *buf, size_t size, std::string filePath);
    bool receive(char * buf);
    ~Client();
private:
    Network  m_network;
};

#endif // CLIENT_H
