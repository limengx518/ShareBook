#ifndef SCANANDCHECKJOTTINGCONTROLLER_H
#define SCANANDCHECKJOTTINGCONTROLLER_H

#include "controller.h"
#include <nlohmann/json.hpp>
#include "client.h"

using json = nlohmann::json ;

class ScanAndCheckJottingController : public Controller
{
public:
    static ScanAndCheckJottingController *getInstance();
    json scanJottings(char* ipaddr);
private:
    ScanAndCheckJottingController();
    static ScanAndCheckJottingController* m_instance;
};
#endif // SCANANDCHECKJOTTINGCONTROLLER_H
