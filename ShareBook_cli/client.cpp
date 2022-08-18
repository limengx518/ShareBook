#include "client.h"

using json=nlohmann::json;

Client::Client(char *ipaddr)
{
    m_network.createSocket();
    m_network.connectSocket(ipaddr);
}

void Client::send(char *buf, size_t size)
{
    m_network.sendMessage(buf,size);
}

void Client::sendPic(char *buf, size_t size)
{
    m_network.sendPic(buf,size);
}

void Client::sendFile(char *buf,size_t size,std::string filePath)
{
    m_network.sendFile(buf,size,filePath);
}

std::string Client::receive()
{
    return m_network.receiveMessage();
}

Client::~Client()
{
}
