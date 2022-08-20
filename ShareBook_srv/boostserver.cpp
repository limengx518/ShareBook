#include "boostserver.h"
#include "scanandcheckjottingcontroller.h"
#include "controllerfactory.h"
#include "publishjottingcontroller.h"
#include <boost/make_shared.hpp>

using boost::asio::ip::tcp;
using json=nlohmann::json;

BoostServer::BoostServer()
{
    m_scanAndCheckJottingController = ControllerFactory::getInstance()->createScanAndCheckJottingController();
    m_publishJottingController = ControllerFactory::getInstance()->createPublishJottingController();
}

void BoostServer::start()
{
    //所有使用asio的程序都必须至少有一个I/O执行上下文
    boost::asio::io_service io_service;
    //创建一个ip::tcp::accept对象来监听新的连接，初始化为BOOSERV_PORT端口，IPv4版本
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), BOOSERV_PORT));

    for(;;){
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        m_threadPool.submit(std::bind(&BoostServer::processClientRequest,this,boost::make_shared<tcp::socket>((std::move(socket)))));
    }

}

void BoostServer::processClientRequest(socket_ptr s)
{
    try {
        BoostNetwork boostNetwork(s);
        std::string message=boostNetwork.receiveMessage();
        json j=json::parse(message);
        std::string request = j["request"];
        if(request == "ScanJottings"){
            std::cout<<"Server << "<<request<<std::endl;
            json j = m_scanAndCheckJottingController->pushJottings();

            boostNetwork.sendMessage(j.dump());

        }else if(request == "PublishJottings"){
            std::string  isPub=m_publishJottingController->publishJottings(j);
            boostNetwork.sendMessage(isPub);
        }

    }  catch (...) {
        std::cerr<<"BoostServer::processClientRequest Error"<<std::endl;
    }

}
