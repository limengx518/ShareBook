#include "publishjottingcontroller.h"


PublishJottingController* PublishJottingController::m_instance = nullptr;

PublishJottingController* PublishJottingController::getInstance()
{
    if(m_instance==nullptr){
        m_instance=new PublishJottingController();
    }
    return m_instance;
}

PublishJottingController::PublishJottingController()
{

}
