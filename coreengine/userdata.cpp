#include "userdata.h"

Userdata* Userdata::m_pInstance = nullptr;

Userdata* Userdata::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new Userdata();
    }
    return m_pInstance;
}

Userdata::Userdata()
    : QObject()
{

}

void Userdata::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
}

void Userdata::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
}
