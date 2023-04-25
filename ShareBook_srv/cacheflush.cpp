#include "cacheflush.h"
#include "netizenbroker.h"
#include "jottingbroker.h"
#include "commentbroker.h"
#include "materialbroker.h"

CacheFlush* CacheFlush::m_instance = nullptr;
CacheFlush *CacheFlush::getInstance()
{
    if(m_instance == nullptr){
        m_instance = new CacheFlush();
    }
    return m_instance;
}

CacheFlush::CacheFlush()
{
    myTimer.start(30000,flush);
}

void CacheFlush::flush()
{
//    NetizenBroker::getInstance()->flush();
//    JottingBroker::getInstance()->flush();
//    CommentBroker::getInstance()->flush();
//    MaterialBroker::getInstance()->flush();
}
