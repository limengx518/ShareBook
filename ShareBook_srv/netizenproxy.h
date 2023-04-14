#ifndef NETIZENPROXY_H
#define NETIZENPROXY_H

#include "netizeninterface.h"

class Netizen;

class NetizenProxy:public NetizenInterface
{
public:
    explicit NetizenProxy(const std::string &tid);
    virtual ~NetizenProxy();
    virtual nlohmann::json getAbstract() override;

    virtual bool isOnline() override;
    virtual void setOnline(bool online) override;
    virtual void updateMessage(std::string id) override;
private:
    Netizen *_netizen;
};
#endif // NETIZENPROXY_H
