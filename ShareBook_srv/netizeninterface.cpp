#include "netizeninterface.h"

NetizenInterface::NetizenInterface(const std::string &tid)
    :m_id(tid)
{

}

std::string NetizenInterface::id() const
{
    return m_id;
}
