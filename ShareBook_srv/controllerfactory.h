#ifndef ControllerFACTORY_H
#define ControllerFACTORY_H

#include "controller.h"
#include "publishjottingcontroller.h"
#include "scanandcheckjottingcontroller.h"

class ControllerFactory
{
public: 
    static ControllerFactory *getInstance();
    ~ControllerFactory();

    PublishJottingController* createPublishJottingController();
    ScanAndCheckJottingController* createScanAndCheckJottingController();
private:
    ControllerFactory();
    static ControllerFactory* m_instance;

};

#endif // ControllerFACTORY_H
