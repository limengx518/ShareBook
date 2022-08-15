#ifndef CLIENT_H
#define CLIENT_H

#include "nlohmann/json.hpp"
#include "network.h"


class Client
{
public:
    Client();
    void send(char *buf, size_t size, char *ipaddr);
    nlohmann::json receive(char *ipaddr);
    ~Client();
private:
};

#endif // CLIENT_H
