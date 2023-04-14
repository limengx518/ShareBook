#ifndef MATERIALPROXY_H
#define MATERIALPROXY_H

#include "materialinterface.h"
#include "nlohmann/json_fwd.hpp"
#include <memory>

class Material;

class MaterialProxy:public MaterialInterface
{
public:
    explicit MaterialProxy(const std::string &tid);
    virtual ~MaterialProxy();
    virtual nlohmann::json getAbstract() override;

private:
    Material *_material;
};

#endif // MATERIALPROXY_H
