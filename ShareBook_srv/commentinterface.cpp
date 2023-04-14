#include "commentinterface.h"

CommentInterface::CommentInterface(const std::string &tid):
    m_timeId{tid}
{

}

CommentInterface::CommentInterface(const CommentInterface &ci)
{
    m_timeId = ci.m_timeId;
}

std::string CommentInterface::id() const
{
    return m_timeId;
}
