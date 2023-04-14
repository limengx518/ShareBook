#include "commentproxy.h"
#include "comment.h"
#include "commentbroker.h"

CommentProxy::CommentProxy(const std::string &tid):
    CommentInterface{tid},_comment(nullptr)
{
}


CommentProxy::~CommentProxy()
{
}

nlohmann::json CommentProxy::getInfo()
{
    if(_comment == nullptr){
        _comment = &CommentBroker::getInstance()->findById(id());
    }
     return _comment->getInfo();

}

