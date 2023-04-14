#ifndef COMMENTPROXY_H
#define COMMENTPROXY_H

#include "commentinterface.h"
#include <memory>

class Comment;

class CommentProxy : public CommentInterface
{
public:
    explicit CommentProxy(const std::string &tid);
    virtual ~CommentProxy();
    virtual nlohmann::json getInfo() override;

private:
    Comment* _comment;
};

#endif // COMMENTPROXY_H
