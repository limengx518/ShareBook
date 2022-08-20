#include "boostclient.h"
#include "boostnetwork.h"

#include <boost/make_shared.hpp>
#include <iostream>

using namespace boost::asio;

BoostClient::BoostClient(std::string serverIp)
{
    //所有使用asio的程序都必须至少有一个I/O执行上下文

    io_service io_service;
    //指定服务器的地址以及端口号
    ip::tcp::endpoint ep(ip::address::from_string(serverIp),BOOSERV_PORT);
    //创建并连接套接字
    ip::tcp::socket s(io_service);
    boost::system::error_code er;
    s.connect(ep,er);
    if(er){
        std::cout<<"连接出错"<<std::endl;
    }
    m_network =  boost::make_shared<ip::tcp::socket>((std::move(s)));
}

void BoostClient::send(std::string message)
{
    m_network.sendMessage(message);
}

std::string BoostClient::receive()
{
    return m_network.receieveMessage();
}

BoostClient::~BoostClient()
{

}
