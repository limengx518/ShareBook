#include "boostclient.h"
#include "boostnetwork.h"

#include <boost/make_shared.hpp>
#include <iostream>

using namespace boost::asio;
using json=nlohmann::json;

BoostClient::BoostClient()
{

}

void BoostClient::send(std::string serverIp,std::string message)
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
    std::cout<<message<<std::endl;
    BoostNetwork network(boost::make_shared<ip::tcp::socket>((std::move(s))));
    network.sendMessage(message);
    std::string receive=network.receieveMessage();
    std::cout<<receive<<std::endl;
//    std::cout<<json::parse(receive).dump()<<std::endl;
}
