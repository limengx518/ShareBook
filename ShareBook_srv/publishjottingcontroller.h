#ifndef PUBLISHJOTTINGController_H
#define PUBLISHJOTTINGController_H

#include "controller.h"
#include "base64.h"
#include <nlohmann/json.hpp>

class PublishJottingController : public Controller
{
public:
     static PublishJottingController *getInstance();
     std::string publishJottings(std::string netizenId,nlohmann::json j);
     void comment(std::string netizenId,std::string comment,std::string jottingId);
private:
     PublishJottingController();
     static PublishJottingController* m_instance;
};

#endif // PUBLISHJOTTINGController_H
