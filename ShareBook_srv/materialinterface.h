#ifndef MATERIALINTERFACE_H
#define MATERIALINTERFACE_H

#include "nlohmann/json_fwd.hpp"
#include <iostream>

class MaterialInterface
{
public:
    explicit MaterialInterface(const std::string &tid);
    virtual  ~MaterialInterface(){}

    virtual nlohmann::json getAbstract()  = 0;
    const std::string id() const;
private:
    std::string m_timeId;
};

#endif // MATERIALINTERFACE_H
