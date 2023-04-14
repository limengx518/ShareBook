#ifndef INITCONTROLLER_H
#define INITCONTROLLER_H

#include "controller.h"
#include "nlohmann/json_fwd.hpp"
#include <string>

class InitController:public Controller
{
public:
    static InitController *getInstance();
    nlohmann::json getInfo(std::string netizenId);
    nlohmann::json getFansAndConsInfo(std::string netizenId);

private:
    InitController();
    static InitController* m_instance;
};

#endif // INITCONTROLLER_H
