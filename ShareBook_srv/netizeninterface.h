#ifndef NETIZENINTERFACE_H
#define NETIZENINTERFACE_H

#include <iostream>
#include <nlohmann/json.hpp>

class NetizenInterface
{
public:
    explicit NetizenInterface(const std::string &tid);
    virtual ~NetizenInterface(){}

    virtual nlohmann::json getAbstract()  = 0;
    std::string id() const;

    virtual bool isOnline()=0;
    virtual void setOnline(bool online)=0;
    virtual void updateMessage(std::string jottingId)=0;

private:
    std::string m_id;
};

#endif // NETIZENINTERFACE_H
