#ifndef NETIZEN_H
#define NETIZEN_H

#include <unordered_map>
#include <string>
#include "commentproxy.h"
#include "jottingproxy.h"
#include <vector>
#include "netizeninterface.h"
#include "netizenproxy.h"
#include <nlohmann/json.hpp>
#include <set>
#include "jottingnotification.h"

class Netizen:public NetizenInterface
{
public:
    explicit Netizen(const std::string& tid);
    explicit Netizen(const std::string id, std::string nickName,std::string signal,std::string avatar, std::vector<std::string> jottingId,std::vector<std::string> fansId, std::vector<std::string> conceredsId,std::vector<std::string> commentdId);

    virtual ~Netizen(){}
    nlohmann::json getInfo();
    nlohmann::json getFansInfo();
    nlohmann::json getConcernedInfo();
    nlohmann::json getJottingNotification();

    const std::string avatarPath() const;
    const std::string nickName()const;
    const std::vector<std::string> jottings() const;
    const std::vector<std::string> fans() const;
    const std::vector<std::string> concerneds() const;
    const std::vector<std::string> commentsId() const;

    virtual nlohmann::json getAbstract() override;
    const std::string readLog();
    void writeLog();
    nlohmann::json scanJottings();
    //首页中的笔记详情是不需要再次发送图片的
    nlohmann::json checkOneJotting(std::string jottingId);
    //个人主页中的笔记详情是需要再次发送图片的
    nlohmann::json checkInfoOneJotting(std::string jottingId);
    bool comment(const std::string content,const std::string jottingId);
    bool publishJotting(nlohmann::json jotting_json);

    void updateMessage(std::string messageId) override;    //更新消息关联
    virtual bool isOnline() override;   //判断是否在线
    virtual void setOnline(bool online) override;

    void followNetizen(std::string concernedId);

    nlohmann::json scanMessages();
    nlohmann::json checkMessage(std::string messageId);   //查看某个消息
    //涨粉
    void growFan(std::string id);

private:
    std::string m_nickName;
    std::string m_signal;
    std::string m_avatar;
    std::unordered_map<std::string, JottingProxy> _jottings;
    std::unordered_map<std::string, NetizenProxy> _fans;
    std::unordered_map<std::string, NetizenProxy> _concerneds;
    std::unordered_map<std::string, CommentProxy> _comments;

    std::set<std::string> _messages;
    bool m_online;

    void removeMessage(std::string id);
};

#endif // NETIZEN_H
