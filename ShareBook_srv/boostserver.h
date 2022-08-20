#ifndef BOOSTSERVER_H
#define BOOSTSERVER_H

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "boostnetwork.h"
#include "publishjottingcontroller.h"
#include "scanandcheckjottingcontroller.h"
#include "threadpool.h"

#define BOOSERV_PORT 9875

class BoostServer
{
public:
    BoostServer();
    void start();
    void processClientRequest(socket_ptr scoket);
private:
    ThreadPool m_threadPool;
    ScanAndCheckJottingController *m_scanAndCheckJottingController;
    PublishJottingController * m_publishJottingController;
};
#endif // BOOSTSERVER_H
