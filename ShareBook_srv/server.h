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

    bool receive(int &connfd, char *buf);
    void send(char *buf, size_t size, int &connfd);
    void sendFile(char *buf, size_t size, int &connfd, std::string filePath);

    void processClientRequest(int &fd);
    void processClientFileRequest(int &fd,std::string filePath);
    void processRTSPRequest(int &fd, std::string ipaddr);
private:
    ThreadPool m_threadPool;
    ScanAndCheckJottingController *m_scanAndCheckJottingController;
    PublishJottingController * m_publishJottingController;
};

#endif // SERVER_H
