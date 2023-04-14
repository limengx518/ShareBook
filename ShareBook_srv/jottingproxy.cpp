#include "jottingproxy.h"
#include "jotting.h"
#include "jottingbroker.h"

JottingProxy::JottingProxy(const std::string &tid):
    JottingInterface{tid},_jotting(nullptr)
{
}

JottingProxy::~JottingProxy()
{
}


nlohmann::json JottingProxy::getAbstract()
{
    if(_jotting == nullptr){
        _jotting = &JottingBroker::getInstance()->findById(id());
    }
    return _jotting->getAbstract();
}

nlohmann::json JottingProxy::getDetail()
{
    if(_jotting == nullptr){
        _jotting = &JottingBroker::getInstance()->findById(id());
    }
    return _jotting->getDetail();
}

nlohmann::json JottingProxy::getOnePicAbstract()
{
    if(_jotting == nullptr){
        _jotting = &JottingBroker::getInstance()->findById(id());
    }
    return _jotting->getOnePicAbstract();
}

nlohmann::json JottingProxy::getInfoDetail()
{
    if(_jotting == nullptr){
        _jotting = &JottingBroker::getInstance()->findById(id());
    }
    return _jotting->getInfoDetail();
}
