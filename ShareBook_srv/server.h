#ifndef SERVER_H
#define SERVER_H

#include "network.h"
#include "threadpool.h"
#include "controllerfactory.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Server
{
public:
    Server();
    void start();
    void processClientRequest(int &fd);
private:
    ThreadPool m_threadPool;
};

#endif // SERVER_H
