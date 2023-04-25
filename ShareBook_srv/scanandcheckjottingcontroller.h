#ifndef SCANANDCHECKJOTTINGController_H
#define SCANANDCHECKJOTTINGController_H

#include "controller.h"
#include <nlohmann/json.hpp>
#include "base64.h"

using json = nlohmann::json;

class ScanAndCheckJottingController : public Controller
{
public:
    static ScanAndCheckJottingController *getInstance();
    json pushJottings(std::string netizenId);
    json pushJottingDetial(std::string netizenId,std::string jottingId);
    json pushInfoJottingDetail(std::string netizenId,std::string jottingId);
    json scanVideos(std::string netizenId);
private:
    ScanAndCheckJottingController();
    static ScanAndCheckJottingController* m_instance;
};

#endif // SCANANDCHECKJOTTINGController_H
