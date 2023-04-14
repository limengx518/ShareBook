#include "initcontroller.h"
#include "netizen.h"
#include "netizenbroker.h"

InitController* InitController::m_instance = nullptr;
InitController *InitController::getInstance()
{
    if(m_instance==nullptr){
        m_instance= new InitController();
    }
    return m_instance;
}

nlohmann::json InitController::getInfo(std::string netizenId)
{
    Netizen &netizen=NetizenBroker::getInstance()->findById(netizenId);
    netizen.writeLog();
    netizen.readLog();
    return netizen.getInfo();
}

nlohmann::json InitController::getFansAndConsInfo(std::string netizenId)
{
    Netizen &netizen=NetizenBroker::getInstance()->findById(netizenId);
    nlohmann::json j;
    j["fans"] = netizen.getFansInfo();
    j["interests"] = netizen.getConcernedInfo();
    return j;
}



InitController::InitController()
{

}
