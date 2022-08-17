#ifndef BOOSTSERVER_H
#define BOOSTSERVER_H

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "boostnetwork.h"
#include "threadpool.h"

#define BOOSERV_PORT 9876

class BoostServer
{
public:
    BoostServer();
    void start();
    void processClientRequest(socket_ptr scoket);
private:
    ThreadPool m_threadPool;
};
#endif // BOOSTSERVER_H