#include "materialproxy.h"
#include "materialbroker.h"
#include "material.h"

MaterialProxy::MaterialProxy(const std::string &tid):
    MaterialInterface(tid),_material(nullptr)
{
}

MaterialProxy::~MaterialProxy()
{
}


nlohmann::json MaterialProxy::getAbstract()
{
    if(_material==nullptr){
        _material=&MaterialBroker::getInstance()->findById(id());
    }
     return _material->getAbstract();
}
