#ifndef SERVER_H
#define SERVER_H

#include "network.h"
#include "threadpool.h"

class Server
{
public:
    Server();
    void start();
    void processClient(int fd);

private:
    ThreadPool m_threadPool;
};

#endif // SERVER_H
