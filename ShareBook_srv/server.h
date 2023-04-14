#ifndef SERVER_H
#define SERVER_H

#include "network.h"
#include "threadpool.h"
#include "controllerfactory.h"
#include "cacheflush.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Server
{
public:
    Server();
    void start();

    bool receive(int &connfd, char *buf);
    void send(char *buf, size_t size, int &connfd);
    void sendFile(char *buf, size_t size, int &connfd, std::string filePath);

    void processClientRequest(int &fd);
    void processClientFileRequest(int &fd,std::string filePath);
private:
    ThreadPool m_threadPool;
    ScanAndCheckJottingController *m_scanAndCheckJottingController;
    PublishJottingController * m_publishJottingController;
    InitController *m_initController;
    CacheFlush *m_cacheFlush;
    MessageController *m_messageController;
};

#endif // SERVER_H
