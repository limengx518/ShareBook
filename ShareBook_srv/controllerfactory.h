#ifndef ControllerFACTORY_H
#define ControllerFACTORY_H

#include "controller.h"
#include "messagecontroller.h"
#include "publishjottingcontroller.h"
#include "scanandcheckjottingcontroller.h"
#include "initcontroller.h"
#include "cacheflush.h"

class ControllerFactory
{
public: 
    static ControllerFactory *getInstance();
    ~ControllerFactory();

    PublishJottingController* createPublishJottingController();
    ScanAndCheckJottingController* createScanAndCheckJottingController();
    InitController *createInitController();
    CacheFlush *createCacheFlush();
    MessageController *createMessageController();
private:
    ControllerFactory();
    static ControllerFactory* m_instance;

};

#endif // ControllerFACTORY_H
