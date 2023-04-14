#include "jottinginterface.h"

JottingInterface::JottingInterface(const std::string &tid):
    m_timeId{tid}
{

}

std::string JottingInterface::id() const
{
    return m_timeId;
}
