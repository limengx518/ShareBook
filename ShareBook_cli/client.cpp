#include "client.h"

using json=nlohmann::json;

Client::Client(const char *ipaddr)
{
    m_network.createSocket();
    m_network.connectSocket(ipaddr);
}

void Client::send( const char *buf, size_t size)
{
    m_network.sendMessage(buf,size);
}

void Client::sendFile(char *buf,size_t size,std::string filePath)
{
    m_network.sendFile(buf,size,filePath);
}

bool Client::receive(char * buf)
{
    return m_network.receiveMessage(buf);
}

Client::~Client()
{
}
