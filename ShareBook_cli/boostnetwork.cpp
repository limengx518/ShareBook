#include "boostnetwork.h"
#include <boost/array.hpp>
#include <iostream>
#include <string>


#define NUMLENGTH 10 //所发送数据大小的长度

using json=nlohmann::json;
using boost::asio::ip::tcp;

BoostNetwork::BoostNetwork()
{

}

BoostNetwork::BoostNetwork(socket_ptr socket)
    :m_socket(socket)
{
}

BoostNetwork::~BoostNetwork()
{
}


std::string BoostNetwork::receieveMessage()
{
    boost::system::error_code error;
    std::cout<<"receieve Message"<<std::endl;


    //首先接受所发数据的大小
    std::string messageSize(NUMLENGTH,' ');
    //用read函数读出套接字中的所有内容
    boost::asio::read(*m_socket, boost::asio::buffer(messageSize,NUMLENGTH), error);
    if (error == boost::asio::error::eof)
        return NULL; // 对端关闭连接
    else if (error)
        throw boost::system::system_error(error);

    //然后将字符串转换为数字
    const size_t size=std::stoi(messageSize);
    std::cout<<size<<std::endl;

    //定义大小为size的字符串缓冲区
    std::string buff(size,' ');
    //用read函数读出套接字中的所有内容
    boost::asio::read(*m_socket, boost::asio::buffer(buff,size), error);

    std::cout<<"receieve end"<<std::endl;

    return buff;
}

int BoostNetwork::sendMessage(std::string message)
{
    boost::system::error_code ignored_error;

    //首先发送数据的大小,规定长度为NUMLENGTH
    size_t size=message.length()*sizeof(char);
    std::string messageSize=std::to_string(size);
    size_t l=messageSize.length();

    if(l<NUMLENGTH){
        for(int i=0;i<NUMLENGTH-l;i++){
            messageSize.insert(0,"0");
        }
    }
     //写操作可能不会将所有的数据传输给对等体。如果需要确保在阻塞操作完成之前写入所有数据，可以考虑使用write函数。
    size_t n=boost::asio::write(*m_socket, boost::asio::buffer(messageSize), ignored_error);
    if(n!=(messageSize.length()*sizeof(char))){
        return 0;
    }


    //然后发送数据
    n=boost::asio::write(*m_socket, boost::asio::buffer(message), ignored_error);
    if(n!=(message.length()*sizeof(char))){
        return 0;
    }

    return 1;
}
