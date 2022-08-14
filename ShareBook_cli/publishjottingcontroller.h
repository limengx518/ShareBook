#ifndef PUBLISHJOTTINGCONTROLLER_H
#define PUBLISHJOTTINGCONTROLLER_H

#include "controller.h"

class PublishJottingController : public Controller
{
public:
     static PublishJottingController *getInstance();
private:
     PublishJottingController();
     static PublishJottingController* m_instance;
};

#endif // PUBLISHJOTTINGCONTROLLER_H
