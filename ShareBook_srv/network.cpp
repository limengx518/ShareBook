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

nlohmann::json NetWork::receiveMessage(int connfd)
{
    char buf[MAXLINE];
    int n;
    if((n=read(connfd,buf,MAXLINE))<0){
        if(errno == ECONNRESET){
            //connection reset by client
            close(connfd);
        }else std::cerr<<"Read error"<<std::endl;

    }else if(n==0){
        //connection closed by client
        close(connfd);
    }
    close(connfd);

    std::string s(buf);
    std::cout<<s<<std::endl;
    if(s.empty()){
        std::cerr<<"Network: Receieve null"<<std::endl;
        json j;
        return j;
    }
    json j= json::parse(s);
    return j;
}

bool NetWork::sendMessage(char *buf,size_t size,int connfd)
{
    if(connfd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    Writen(connfd,buf,size);
    close(connfd);
    return true;
}


ssize_t	NetWork::writen(int fd, const void *vptr, size_t n)/* Write "n" bytes to a descriptor. */
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
/* end writen */

void NetWork::Writen(int fd, void *ptr, size_t nbytes){
    if (writen(fd, ptr, nbytes) != nbytes){
         std::cerr<<"writen error"<<std::endl;
    }else{
        std::cerr<<"writen success"<<std::endl;
    }
}



