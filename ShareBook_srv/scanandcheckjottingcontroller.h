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
    json pushJottings();
private:
    ScanAndCheckJottingController();
    static ScanAndCheckJottingController* m_instance;
};

#endif // SCANANDCHECKJOTTINGController_H
