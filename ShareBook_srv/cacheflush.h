#ifndef CACHEFLUSH_H
#define CACHEFLUSH_H

#include "controller.h"
#include "mytimer.h"

class CacheFlush:public Controller
{
public:
    static CacheFlush * getInstance();
private:
    CacheFlush();
    static void flush();
    static CacheFlush* m_instance;
    MyTimer myTimer;
};

#endif // CACHEFLUSH_H
