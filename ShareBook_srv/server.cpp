#include "server.h"

Server::Server()
{

}

void Server::start()
{
    NetWork network;
    //创建套接字
    network.Create();
    network.Bind();
    network.Listen();
    int nready;
    while(1){
        if(network.Poll()){
            int connfd = network.Accept();
            if(connfd<0) continue;
            m_threadPool.submit(std::bind(&Server::processClient, this, connfd));
        };
    }

    network.Accept();
}

void Server::processClient(int fd)
{

}
