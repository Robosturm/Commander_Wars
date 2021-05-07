#include "multiplayer/gamedata.h"

GameData::GameData()
    : QObject()
{

}
void GameData::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_Host;
    pStream << m_MapName;
    pStream << m_Player;
    pStream << m_MaxPlayer;
    pStream << m_Version;
    pStream << m_password;
    pStream << static_cast<qint32>(m_modData.size());
    for (qint32 i = 0; i < m_modData.size(); ++i)
    {
        pStream << m_modData[i];
    }
}

void GameData::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_Host;
    pStream >> m_MapName;
    pStream >> m_Player;
    pStream >> m_MaxPlayer;
    pStream >> m_Version;
    pStream >> m_password;
    qint32 size = 0;
    pStream >> size;
    m_modData.clear();
    for (qint32 i = 0; i < size; ++i)
    {
        QString value;
        pStream >> value;
        m_modData.append(value);
    }
}
