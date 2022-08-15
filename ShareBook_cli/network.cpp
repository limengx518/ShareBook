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
#define INFTIM -1 //poll永远等待
#define MAXLINE 1000000
#define INVALID_SOCKET_FD -1

using json = nlohmann::json;

Network::Network()
{

}


int Network::createSocket()
{
    m_socketFd = socket(AF_INET,SOCK_STREAM,0);
    if(m_socketFd==INVALID_SOCKET_FD){
        std::cerr<<"Create socket error"<<std::endl;
    }
    return m_socketFd;
}

int Network::connectSocket(char* ipaddr)
{
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,ipaddr,&servaddr.sin_addr);

    if(connect(m_socketFd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        printf("Create socket failed. Errorn info: %d %s\n",errno,strerror(errno));
        return -1;
    }
    return 0;
}

nlohmann::json Network::receiveMessage()
{
    char buf[MAXLINE];
//    int n=read(m_socketFd,buf,MAXLINE);
    int n=recv(m_socketFd,buf,sizeof(buf),0);
    std::cout<<"receieve Message: "<<std::endl;
    if(n<0){
        if(errno == ECONNRESET){//connection reset by client
            printf("Connection reset by client！！\n");
        }else if(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN){
            printf("Read time out!!\n");
        }else{
            printf("NetWork::receiveMessage-> Read failed. Errorn info: %d %s\n",errno,strerror(errno));
        }
        return nullptr;
    }else if(n==0){
        printf("Client does not have input information ,and server does not have any information，waiting...\n");
        //connection closed by client
        return nullptr;
    }
    std::string s(buf);
    if(s.empty()){
        std::cerr<<"Network: Client receieve null"<<std::endl;
        json j;
        return j;
    }
    json j= json::parse(s);
    return j;
}

bool Network::sendMessage(char *buf, size_t size)
{
    if(m_socketFd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    ::send(m_socketFd,buf,size,0);
//    Writen(m_socketFd,buf,size);

    return true;
}

void Network::Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        std::cerr<<"Writen error"<<std::endl;
}

//连接半关闭函数，当前留在套接字发送缓冲区的数据将被发送到，后跟TCP的正常连接终止序列。
//进程不能对当前套接字调用任何写函数。
int Network::closeSocket()
{
    if(shutdown(m_socketFd,SHUT_WR)<0){
        printf("CloseSocket failed. Errorn info: %d %s\n",errno,strerror(errno));
        return -1;
    }
    return 0;
}

ssize_t Network::writen(int fd, const void *vptr, size_t n)
{
    size_t		nleft;
    ssize_t		nwritten;
    const char	*ptr;

    ptr = (char *)vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;		/* and call write() again */
            else
                return(-1);			/* error */
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
