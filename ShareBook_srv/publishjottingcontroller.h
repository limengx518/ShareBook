#ifndef PUBLISHJOTTINGController_H
#define PUBLISHJOTTINGController_H

#include "controller.h"

class PublishJottingController : public Controller
{
public:
     static PublishJottingController *getInstance();
private:
     PublishJottingController();
     static PublishJottingController* m_instance;
};

#endif // PUBLISHJOTTINGController_H
