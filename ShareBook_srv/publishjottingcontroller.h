#ifndef PUBLISHJOTTINGController_H
#define PUBLISHJOTTINGController_H

#include "controller.h"
#include <nlohmann/json.hpp>

class PublishJottingController : public Controller
{
public:
     static PublishJottingController *getInstance();
     std::string publishJottings(nlohmann::json j);
private:
     PublishJottingController();
     static PublishJottingController* m_instance;
};

#endif // PUBLISHJOTTINGController_H
