#include "controllerfactory.h"


ControllerFactory* ControllerFactory::m_instance=nullptr;


ControllerFactory *ControllerFactory::getInstance()
{
    if(m_instance==nullptr){
        m_instance=new ControllerFactory();
    }
    return m_instance;
}

PublishJottingController *ControllerFactory::createPublishJottingController()
{
    return PublishJottingController::getInstance();
}

ScanAndCheckJottingController *ControllerFactory::createScanAndCheckJottingController()
{
    return ScanAndCheckJottingController::getInstance();
}

ControllerFactory::ControllerFactory()
{

}

ControllerFactory::~ControllerFactory()
{
}
