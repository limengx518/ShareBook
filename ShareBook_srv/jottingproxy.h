#ifndef JOTTINGPROXY_H
#define JOTTINGPROXY_H

#include "jottinginterface.h"
#include <memory>

class Jotting;

class JottingProxy : public JottingInterface
{
public:
    explicit JottingProxy(const std::string &tid);
    virtual ~JottingProxy();
    virtual nlohmann::json getAbstract() override;
    virtual nlohmann::json getDetail() override;
    virtual nlohmann::json getOnePicAbstract() override;
    virtual nlohmann::json getInfoDetail() override;//非主页的笔记详情
        virtual nlohmann::json getMessageAbstract();//消息界面
private:
    Jotting* _jotting;
};

#endif // JOTTINGPROXY_H
