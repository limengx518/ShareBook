/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       network.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#include "network.h"
#define SERV_PORT 9877
#define LISTENQ 1000
#define INFTIM -1 //poll永远等待
#define MAXLINE 10000


using json = nlohmann::json;


Network::Network()
{

}

Network::Network(int &fd)
    :m_listenFd(fd)
{

}

int Network::createSocket()
{
    m_listenFd = socket(AF_INET,SOCK_STREAM,0);
    if(m_listenFd==INVALID_SOCKET_FD){
         printf("Create socket failed. Errorn info: %d %s\n",errno,strerror(errno));
    }
    return m_listenFd;
}

int Network::bindSocket()
{
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    if(bind(m_listenFd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1){
        printf("Bind socket failed. Errorn info: %d %s\n",errno,strerror(errno));
        return -1;
    }
    return 0;
}

int Network::listenSocket()
{
    if(listen(m_listenFd,LISTENQ)<0){
        std::cerr<<"Listen socket error.Errorn info "<<errno<<","<<strerror(errno)<<std::endl;
        return -1;
    }
    return 0;
}

int Network::acceptSocket()
{
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd = accept(m_listenFd, (struct sockaddr *)&cliaddr,&clilen);
    if(connfd<0){
        printf("Accept socket failed. Errorn info: %d %s\n",errno,strerror(errno));
    }
    return connfd;
}

int Network::pollSocket()
{
    struct pollfd pd;
    pd.fd  = m_listenFd;
    pd.events = POLLRDNORM;
    if(poll(&pd,1,INFTIM)<=0){
        printf("Poll failed. Errorn info: %d %s\n",errno,strerror(errno));
    }
    return 1;

}

Network::~Network()
{
    closeSocket();
}

void Network::closeSocket()
{
    close(m_listenFd);
}

int Network::sendMessage(char *buf,size_t size,int &connfd)
{
    if(connfd<0){
        printf("Client socket error.Errorn info: %d %s\n",errno,strerror(errno));
        return false;
    }
    if(buf == NULL || size <= 0) return -1;
    int n = ::send(connfd,buf,size,0);
    if(n <0){
        if(errno == EWOULDBLOCK || errno == EINTR || errno == EWOULDBLOCK){
            printf("Server write error. Errorn info: %d %s\n",errno,strerror(errno));
        }
        return -1;
    }
    return n;
}

nlohmann::json Network::receiveMessage(int& connfd)
{
    char buf[MAXLINE];
    memset(buf,0,sizeof(buf));
    int n=recv(connfd,buf,sizeof(buf),0);
    if( n == -1){
        if(errno == ECONNRESET || errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN){
            printf("Server read error. Errorn info: %d %s\n",errno,strerror(errno));
        }
        return nullptr;
    }else if(n==0){
        printf("The opposite end has closed the socket.\n");
        return nullptr;
    }
    std::string s(buf);
    json j= json::parse(buf);
    return j;
}

int Network::sendFile(char *buf, size_t size,  int &connfd,std::string filePath)
{
    if(m_listenFd<0){
        printf("Client socket error.Errorn info: %d %s\n",errno,strerror(errno));
        return false;
    }
    FILE *fq;
    if( ( fq = fopen(filePath.c_str(),"rb") ) == NULL ){
        printf("File open.\n");
        close(connfd);
        exit(1);
    }
    int len;
    while(!feof(fq)){
        len = fread(buf, 1, sizeof(buf), fq);
        if(len != ::send(connfd, buf, len,0)){
            printf("Server file write error. Errorn info: %d %s\n",errno,strerror(errno));
            break;
        }
    }
    fclose(fq);
    return 1;
}

std::string Network::receiveFile(int &connfd,std::string filePath)
{
    FILE *fp;
    if((fp = fopen(filePath.c_str(),"ab")) == NULL ){
       printf("File.\n");
       close(m_listenFd);
       exit(1);
    }

    char buf[MAXLINE];
    memset(buf,0,sizeof(buf));
    int n;

    while(1){
        n = ::recv(connfd, buf, MAXLINE,0);
        if(n<0) printf("Server file read error. Errorn info: %d %s\n",errno,strerror(errno));
        if(n == 0) break;
        fwrite(buf, 1, n, fp);
    }
    buf[n] = '\0';
    fclose(fp);

    return "接收文件成功！！";
}

