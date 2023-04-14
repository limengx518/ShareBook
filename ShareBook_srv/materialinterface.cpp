#include "materialinterface.h"

MaterialInterface::MaterialInterface(const std::string &tid):
    m_timeId(tid)
{

}

const std::string MaterialInterface::id() const
{
    return m_timeId;
}
