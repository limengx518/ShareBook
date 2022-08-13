#ifndef SCANANDCHECKJOTTINGController_H
#define SCANANDCHECKJOTTINGController_H

#include "controller.h"

class ScanAndCheckJottingController : public Controller
{
public:
    static ScanAndCheckJottingController *getInstance();
private:
    ScanAndCheckJottingController();
    static ScanAndCheckJottingController* m_instance;
};

#endif // SCANANDCHECKJOTTINGController_H
