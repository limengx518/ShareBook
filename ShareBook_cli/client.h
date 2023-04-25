#ifndef CLIENT_H
#define CLIENT_H

#include "nlohmann/json.hpp"
#include "network.h"


class Client
{
public:
    static Client *getInstance();
     ~Client();

    void send(const char *buf, size_t size);
    void sendFile(char *buf, size_t size, std::string filePath);
    bool receive(char * buf);
    void start();
    void reconnect();
    std::string receiveFile();
    void closeSocket();
private:
    Client();
    static Client* m_instance;
    Network  m_network;
};

#endif // CLIENT_H
