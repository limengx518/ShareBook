#include "network.h"
#define SERV_PORT 9877
#define LISTENQ 1000
#define INFTIM -1 //poll永远等待

NetWork::NetWork()
{

}

int NetWork::Create()
{
    m_listenFd = socket(AF_INET,SOCK_STREAM,0);
    if(m_listenFd==INVALID_SOCKET_FD){
        std::cerr<<"Create socket error"<<std::endl;
    }
    return m_listenFd;
}

int NetWork::Bind()
{
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    if(bind(m_listenFd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        std::cerr<<"Bind socket error"<<std::endl;
        return -1;
    }
    return 0;
}

int NetWork::Listen()
{
    if(listen(m_listenFd,LISTENQ)<0){
        std::cerr<<"Listen socket error"<<std::endl;
        return -1;
    }
    return 0;
}

int NetWork::Accept()
{
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd = accept(m_listenFd, (struct sockaddr *)&cliaddr,&clilen);
    if(connfd<0){
        std::cerr<<"Accept  error"<<std::endl;
    }
    return connfd;
}

int NetWork::Poll()
{
    struct pollfd pd;
    pd.fd  = m_listenFd;
    pd.events = POLLRDNORM;
    if(poll(&pd,1,INFTIM)<=0){
        std::cerr<<"Poll error"<<std::endl;
    }
    return 1;
}

void NetWork::receiveMessage(nlohmann::json j, int connfd)
{

}







