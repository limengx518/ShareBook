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
#define MAXLINE 1024
#define INVALID_SOCKET_FD -1

using json = nlohmann::json;

Network::Network()
{

}

int Network::createSocket()
{
    m_socketFd = socket(AF_INET,SOCK_STREAM,0);
    if(m_socketFd==INVALID_SOCKET_FD){
       printf("Create socket failed. Errorn info: %d %s\n",errno,strerror(errno));
    }
    std::cout<<m_socketFd<<std::endl;
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

std::string Network::receiveMessage()
{
    char buf[MAXLINE];
    memset(buf,0,sizeof(buf));
    int n=recv(m_socketFd,buf,sizeof(buf),0);
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

bool Network::sendMessage(char *buf, size_t size)
{
    if(m_socketFd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    if(buf == NULL || size <= 0) return -1;
    int n = ::send(m_socketFd,buf,size,0);
    if(n <0){
        if(errno == EWOULDBLOCK || errno == EINTR || errno == EWOULDBLOCK){
            printf("Client write error. Errorn info: %d %s\n",errno,strerror(errno));
        }
        return false;
    }

    return true;
}

#include <cstring>

bool Network::sendPic(const char *buf, size_t size)
{
    if(m_socketFd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    int send_size = 0,msg_size = strlen(buf);

    std::cout<<"发送字节流:"<<msg_size<<std::endl;
    std::string str = std::to_string(msg_size);
    send_size = send(m_socketFd, str.c_str(), str.size(), 0);
    std::cout<<"send_size =="<<send_size<<std::endl;
    std::cout<<"msg_size =="<<msg_size<<std::endl;

    int pos = 0;
    std::string tmp(buf);
//    while (msg_size > 0) {
//       send_size = send(m_socketFd, buf+pos, 1024, 0);
//       std::cout<<"while:: send_size =="<<send_size<<std::endl;

//       if (send_size < 0) {
//            printf("Client write error. Errorn info: %d %s\n",errno,strerror(errno));
//            return false;
//       }
//       std::cout<<"\n"<<"pos=="<<pos<<std::endl;
//       pos += send_size;
//       msg_size -= send_size;;

//    }
    std::cout<<"success"<<std::endl;
    return true;
}

bool Network::sendFile(char *buf, size_t size, std::string filePath)
{

    if(m_socketFd<0){
        std::cerr<<"Client Socket error"<<std::endl;
        return false;
    }
    FILE *fq;
    if( ( fq = fopen(filePath.c_str(),"rb") ) == NULL ){
        printf("File open.\n");
        close(m_socketFd);
        exit(1);
    }
    int len;
    while(!feof(fq)){
        len = fread(buf, 1, sizeof(buf), fq);
        if(len != ::send(m_socketFd, buf, len,0)){
            printf("write.\n");
            break;
        }
    }
    fclose(fq);
    return true;
}

Network::~Network()
{
    close(m_socketFd);
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
