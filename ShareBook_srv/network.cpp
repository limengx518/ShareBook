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


NetWork::NetWork()
{

}

int NetWork::createSocket()
{
    m_listenFd = socket(AF_INET,SOCK_STREAM,0);
    if(m_listenFd==INVALID_SOCKET_FD){
         printf("Create socket failed. Errorn info: %d %s\n",errno,strerror(errno));
    }
    return m_listenFd;
}

int NetWork::bindSocket()
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

int NetWork::listenSocket()
{
    if(listen(m_listenFd,LISTENQ)<0){
        std::cerr<<"Listen socket error.Errorn info "<<errno<<","<<strerror(errno)<<std::endl;
        return -1;
    }
    return 0;
}

int NetWork::acceptSocket()
{
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd = accept(m_listenFd, (struct sockaddr *)&cliaddr,&clilen);
    if(connfd<0){
        printf("Accept socket failed. Errorn info: %d %s\n",errno,strerror(errno));
    }
    return connfd;
}

int NetWork::Poll()
{
    struct pollfd pd;
    pd.fd  = m_listenFd;
    pd.events = POLLRDNORM;
    if(poll(&pd,1,INFTIM)<=0){
        printf("Poll failed. Errorn info: %d %s\n",errno,strerror(errno));
    }
    return 1;

}

void NetWork::closeSocket()
{
    close(m_listenFd);
}

nlohmann::json NetWork::receiveMessage(int& connfd)
{
    char buf[MAXLINE];
    memset(buf,0,sizeof(buf));
    int n;
    if((n=read(connfd,buf,sizeof(buf))) == -1){
        if(errno == ECONNRESET){//connection reset by client
            printf("Connection reset by client！！\n");
        }else if(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN){
            printf("Read time out!!\n");
        }else{
            printf("NetWork::receiveMessage-> Read failed. Errorn info: %d %s\n",errno,strerror(errno));
        }
        return nullptr;
    }else if(n==0){
        printf("The opposite end has closed the socket\n");
        return nullptr;
    }
    std::string s(buf);
    std::cout<<"s="<<s<<std::endl;
    json j= json::parse(buf);
    return j;
}

bool NetWork::sendMessage(char *buf,size_t size,int &connfd)
{
    if(connfd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    Writen(connfd,buf,size);
    return true;
}


ssize_t	NetWork::writen(int& fd, const void *vptr, size_t n)/* Write "n" bytes to a descriptor. */
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
            else{
                 printf("error == %d\n",errno);
                 return(-1);			/* error */
            }
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}
/* end writen */

void NetWork::Writen(int &fd, void *ptr, size_t nbytes){
    if (writen(fd, ptr, nbytes) != nbytes){
          printf("Writen failed. Errorn info: %d %s\n",errno,strerror(errno));
    }else{
        std::cerr<<"Writen success"<<std::endl;
    }
}



