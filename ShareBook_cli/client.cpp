#include "client.h"

using json=nlohmann::json;

Client::Client()
{
}


void Client::send(char *buf, size_t size, char *ipaddr)
{
    Network  m_network;
    m_network.createSocket();
    m_network.connectSocket(ipaddr);
    m_network.sendMessage(buf,size);
    m_network.closeSocket();
}


nlohmann::json Client::receive(char *ipaddr)
{
    Network  m_network;
    m_network.createSocket();
    m_network.connectSocket(ipaddr);
//    std::cout<<"Receive"<<std::endl;
    json j=m_network.receiveMessage();
    m_network.closeSocket();
    return j;
}

Client::~Client()
{
}
