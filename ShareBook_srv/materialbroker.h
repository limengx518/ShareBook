#ifndef MATERIALBROKER_H
#define MATERIALBROKER_H

#include "relationalbroker.h"
#include "material.h"
#include <unordered_map>
#include <set>

class MaterialBroker:public RelationalBroker
{
public:
    static MaterialBroker *getInstance();

    Material& findById(std::string id);
    std::vector<std::string> findJottingNewMaterial(std::string jottingId);
    static void flush();
    void addMaterial(Material material);
    virtual ~MaterialBroker();

private:
    MaterialBroker();
    Material* inCache(std::string id);
    Material& retrieveMaterial(std::string materialId);
    static void newCacheFlush();
    static MaterialBroker* m_materialBroker;
    static std::unordered_map<std::string,Material> m_newClean;
    static std::unordered_map<std::string,Material> m_oldClean;
    static std::mutex m_mutex;
};

#endif // MATERIALBROKER_H
