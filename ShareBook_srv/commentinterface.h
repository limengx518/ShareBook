#ifndef COMMENTINTERFACE_H
#define COMMENTINTERFACE_H

#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

class CommentInterface
{
public:
    explicit CommentInterface(const std::string &tid);
    explicit CommentInterface(const CommentInterface & ci);
    virtual ~CommentInterface(){}

    virtual nlohmann::json getInfo()=0;

    std::string id() const;
private:
    std::string m_timeId;
};

#endif // COMMENTINTERFACE_H
