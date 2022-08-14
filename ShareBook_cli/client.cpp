#include "client.h"

using json=nlohmann::json;

Client::Client()
{

}


void Client::send(char *buf, size_t size, char *ipaddr)
{
    Network  netWork;
    netWork.Create();
    netWork.Connect(ipaddr);
    netWork.sendMessage(buf,size);
    netWork.Close();
}


nlohmann::json Client::receive(char *ipaddr)
{
    Network  netWork;
    netWork.Create();
    netWork.Connect(ipaddr);
    std::cout<<"receive"<<std::endl;
    json j=netWork.receiveMessage();
    netWork.Close();
    return j;
}
