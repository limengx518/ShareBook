#ifndef JOTTINGINTERFACE_H
#define JOTTINGINTERFACE_H
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

class JottingInterface
{
public:
    explicit JottingInterface(const std::string &tid);
    virtual ~JottingInterface(){}

    virtual nlohmann::json getAbstract()= 0;
    virtual nlohmann::json getDetail()= 0;//主页的笔记详情
    virtual nlohmann::json getOnePicAbstract() = 0;
    virtual nlohmann::json getInfoDetail() = 0;//非主页的笔记详情

    std::string id() const;
private:
    std::string m_timeId;

};

#endif // JOTTINGINTERFACE_H
