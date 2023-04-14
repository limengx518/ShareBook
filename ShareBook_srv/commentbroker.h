#ifndef COMMENTBROKER_H
#define COMMENTBROKER_H

#include <vector>
#include <unordered_map>
#include <set>

#include "commentinterface.h"
#include "relationalbroker.h"
#include "comment.h"

class CommentBroker : public RelationalBroker
{
private:
    CommentBroker();
    static void newCleanFlush();
    static CommentBroker* m_CommentBroker;

    Comment* inCache(std::string id);

    static std::unordered_map<std::string,Comment> m_newClean;
    static std::unordered_map<std::string,Comment> m_oldClean;
    static std::mutex m_mutex;

public:
    static CommentBroker *getInstance();

    Comment &findById(std::string id);
    std::vector<std::string> getSomeCommentsId(std::string lastTime,std::string thisTime);
    std::vector<std::string> findNewComments(std::string netizenId);
    std::vector<std::string> findJottingNewComment(std::string jottingId);

    Comment& retrieveComment(std::string commentiId);

    void addComment(const Comment& comment);

    static void flush();

    virtual ~CommentBroker();
};

#endif // COMMENTBROKER_H
