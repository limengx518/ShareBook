#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include "controller.h"
#include "nlohmann/json_fwd.hpp"
#include "mytimer.h"

class MessageController:public Controller
{
public:
    static MessageController *getInstance();
    nlohmann::json getJottingNotification(std::string netizenId);
private:
    static void NotificationFlush();
    MessageController();
    static MessageController* m_instance;
    MyTimer m_myTimer;
};

#endif // MESSAGECONTROLLER_H
