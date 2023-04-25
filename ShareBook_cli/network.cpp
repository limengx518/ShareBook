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

#define SERV_PORT 9878
#define INFTIM -1 //poll永远等待
#define MAXLINE 9999999
#define INVALID_SOCKET_FD -1

using json = nlohmann::json;

Network::Network()
{

}

int Network::createSocket()
{
    m_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(m_sockfd==INVALID_SOCKET_FD){
       printf("Create socket failed. Errorn info: %d %s\n",errno,strerror(errno));
    }
//    std::cout<<"Client:: socketFd:"<<m_sockfd<<std::endl;
    return m_sockfd;
}

int Network::connectSocket(const char* ipaddr)
{
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,ipaddr,&servaddr.sin_addr);

    if(connect(m_sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        printf("Create socket failed. Errorn info: %d %s\n",errno,strerror(errno));
        return -1;
    }
    return 0;
}

bool Network::receiveMessage(char* buffer)
{
    int one_size = 0,msg_size;
    std::string msg;

//    //每次发送数据之前，先发送一个固定长度的自定义包头，包头中定义了这一次数据的长度。
//    //服务端先按照包头长度接受包头数据，再按照包头数据中指定的数据长度接受这一次通信的数据。
//    //我们使用一个int类型作为“包头”，代表发送数据的长度。
//    //而int类型固定4字节，因此服务端每次先接受4字节的数据x，再接受x字节的字符串数据。

    one_size = recv(m_sockfd,&msg_size,sizeof(4),0);
    int pos = 0;
    while (msg_size > 0) {
        one_size = recv(m_sockfd, buffer+pos , 1024, 0);
        if (one_size == 0) {
            printf("client disconnect\n");
            return false;
        }
        if (one_size < 0) {
            printf("Server read error. Errorn info: %d %s\n",errno,strerror(errno));
            return false;
        }
        pos += one_size;
        msg_size -= one_size;
    }
    return true;
}

bool Network::sendMessage(const char *buf, size_t size)
{
    if(m_sockfd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    int send_size = 0 , msg_size = strlen(buf);

    send_size = send(m_sockfd, &msg_size, sizeof(4), 0);

    int pos = 0;
    std::string tmp(buf);
    while (msg_size > 0) {
       send_size = send(m_sockfd, buf+pos, 1024, 0);

       if (send_size < 0) {
            printf("Client write error. Errorn info: %d %s\n",errno,strerror(errno));
            return false;
       }
       pos += send_size;
       msg_size -= send_size;;

    }
    return true;
}

bool Network::sendFile(char *buf, size_t size, std::string filePath)
{

    if(m_sockfd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    FILE *fq;
    if( ( fq = fopen(filePath.c_str(),"rb") ) == NULL ){
        printf("File open.\n");
        close(m_sockfd);
        exit(1);
    }
    int len;
    while(!feof(fq)){
        len = fread(buf, 1, sizeof(buf), fq);
        if(len != ::send(m_sockfd, buf, len,0)){
            printf("write.\n");
            break;
        }
    }
    fclose(fq);
    return true;
}

Network::~Network()
{
    close(m_sockfd);
}

//连接半关闭函数，当前留在套接字发送缓冲区的数据将被发送到，后跟TCP的正常连接终止序列。
//进程不能对当前套接字调用任何写函数。
int Network::closeSocket()
{
    if(shutdown(m_sockfd,SHUT_WR)<0){
        printf("CloseSocket failed. Errorn info: %d %s\n",errno,strerror(errno));
        return -1;
    }
    return 0;
}

std::string Network::receiveFile()
{
    char buf[MAXLINE];
    memset(buf,0,sizeof(buf));
    int n=recv(m_sockfd,buf,sizeof(buf),0);
    std::cout<<"receive size:"<<n;
    std::cout<<buf;
    if( n == -1){
        if(errno == ECONNRESET || errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN){
            printf("Client read error. Errorn info: %d %s\n",errno,strerror(errno));
        }
        return nullptr;
    }else if(n==0){
        printf("The opposite end has closed the socket.\n");
        return nullptr;
    }
    std::string s(buf);
    if(s.empty()){
        std::cerr<<"Network: Client receieve null"<<std::endl;
        return nullptr;
    };
    return s;
}

/*
std::string Network::receiveMessage()
{
    char buf[MAXLINE];
    memset(buf,0,sizeof(buf));
    int n=recv(m_sockfd,buf,sizeof(buf),0);
    if( n == -1){
        if(errno == ECONNRESET || errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN){
            printf("Client read error. Errorn info: %d %s\n",errno,strerror(errno));
        }
        return nullptr;
    }else if(n==0){
        printf("The opposite end has closed the socket.\n");
        return nullptr;
    }
    std::string s(buf);
    if(s.empty()){
        std::cerr<<"Network: Client receieve null"<<std::endl;
        return nullptr;
    };
    return s;
}*/


/*
bool Network::sendMessage(char *buf, size_t size)
{
    if(m_sockfd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    if(buf == NULL || size <= 0) return -1;
    int n = ::send(m_sockfd,buf,size,0);
    if(n <0){
        if(errno == EWOULDBLOCK || errno == EINTR || errno == EWOULDBLOCK){
            printf("Client write error. Errorn info: %d %s\n",errno,strerror(errno));
        }
        return false;
    }

    return true;
}*/
