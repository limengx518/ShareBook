#ifndef JOTTINGBROKER_H
#define JOTTINGBROKER_H

#include <unordered_map>
#include <set>
#include "jottinginterface.h"
#include "jotting.h"
#include "relationalbroker.h"
#include <vector>
#include "jottingproxy.h"
#include <mutex>
#include <condition_variable>

class JottingBroker : public RelationalBroker
{
public:
    static JottingBroker *getInstance();

    Jotting &findById(std::string id);

    std::vector<JottingProxy> pushJottings(std::string netizenId,std::string lastTime,std::string thisTime);

    //找到某一网民新发布的还未存入数据库中的笔记
    std::vector<std::string> findNewJottings(std::string netizenId);

    static void flush();
    void addJotting(const Jotting& jotting);

    virtual ~JottingBroker();
private:
    JottingBroker();

    Jotting* inCache(std::string id);
    static void newCleanFlush();

    Jotting& retrieveJotting(std::string jottingId);
    std::vector<std::string> findMaterials(std::string jottingid);
    std::vector<std::string> findComments(std::string jottingId);

    static JottingBroker* m_jottingBroker;
    static std::mutex m_mutex;
    static std::unordered_map<std::string,Jotting> m_newClean;
    static std::unordered_map<std::string,Jotting> m_oldClean;
};

#endif // JOTTINGBROKER_H
