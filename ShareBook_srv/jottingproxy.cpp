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
    try {
        if(_jotting == nullptr){
            _jotting = &JottingBroker::getInstance()->findById(id());
        }
        return _jotting->getOnePicAbstract();
    }  catch (...) {
        std::cout<<"get One Pic Abstract error";
    }

}

nlohmann::json JottingProxy::getInfoDetail()
{
    if(_jotting == nullptr){
        _jotting = &JottingBroker::getInstance()->findById(id());
    }
    return _jotting->getInfoDetail();
}

nlohmann::json JottingProxy::getMessageAbstract()
{
    if(_jotting == nullptr){
        _jotting = &JottingBroker::getInstance()->findById(id());
    }
    return _jotting->getMessageAbstract();

}
