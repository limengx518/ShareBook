#include "scanandcheckjottingcontroller.h"

ScanAndCheckJottingController * ScanAndCheckJottingController::m_instance = nullptr;

ScanAndCheckJottingController *ScanAndCheckJottingController::getInstance()
{
    if(m_instance==nullptr){
        m_instance=new ScanAndCheckJottingController();
    }
    return m_instance;
}

ScanAndCheckJottingController::ScanAndCheckJottingController()
{

}
