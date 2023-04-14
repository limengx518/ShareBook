#include "netizenproxy.h"
#include "netizenbroker.h"
#include "netizen.h"

NetizenProxy::NetizenProxy(const std::string &tid):
    NetizenInterface(tid),_netizen(nullptr)
{
}

NetizenProxy::~NetizenProxy()
{
}

nlohmann::json NetizenProxy::getAbstract()
{
    if(_netizen==nullptr){
        _netizen=&NetizenBroker::getInstance()->findById(id());
    }
    return _netizen->getAbstract();
}

bool NetizenProxy::isOnline()
{
    if(_netizen==nullptr){
        _netizen=&NetizenBroker::getInstance()->findById(id());
    }
    return _netizen->isOnline();
}

void NetizenProxy::setOnline(bool online)
{
    if(_netizen==nullptr){
        _netizen=&NetizenBroker::getInstance()->findById(id());
    }
    _netizen->setOnline(online);
}

void NetizenProxy::updateMessage(std::string messageId)
{
    if(_netizen==nullptr){
        _netizen=&NetizenBroker::getInstance()->findById(id());
    }
    return _netizen->updateMessage(messageId);
}
