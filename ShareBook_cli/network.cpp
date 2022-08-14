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


int Network::Create()
{
    m_socketFd = socket(AF_INET,SOCK_STREAM,0);
    if(m_socketFd==INVALID_SOCKET_FD){
        std::cerr<<"Create socket error"<<std::endl;
    }
    return m_socketFd;
}

int Network::Connect(char* ipaddr)
{
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,ipaddr,&servaddr.sin_addr);

    if(connect(m_socketFd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        std::cerr<<"Connect socket error"<<std::endl;
        return -1;
    }
    return 0;
}

nlohmann::json Network::receiveMessage()
{
    char buf[MAXLINE];
    int n;
    fd_set rset;
    FD_SET(m_socketFd,&rset);
    select(m_socketFd+1,&rset,NULL,NULL,NULL);
    if(FD_ISSET(m_socketFd,&rset)){
        if((n=read(m_socketFd,buf,MAXLINE))<0){
            if(errno == ECONNRESET){
                //connection reset by client
                Close();
            }else std::cerr<<"Read error"<<std::endl;

            return NULL;
        }else if(n==0){
            std::cout<<"server message is null"<<std::endl;
            //connection closed by client
            Close();
            return NULL;
        }
        json j = json::parse(buf);
        return j;
    }
}

bool Network::sendMessage(char *buf, size_t size)
{
    if(m_socketFd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    Writen(m_socketFd,buf,size);
    Close();
    return true;
}

void Network::Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        std::cerr<<"Writen error"<<std::endl;
}

//连接半关闭函数，当前留在套接字发送缓冲区的数据将被发送到，后跟TCP的正常连接终止序列。
//进程不能对当前套接字调用任何写函数。
int Network::Close()
{
    if(shutdown(m_socketFd,SHUT_WR)<0){
        std::cerr<<"Close error"<<std::endl;
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
