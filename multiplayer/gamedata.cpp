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
    pStream << password;
    pStream << static_cast<qint32>(modData.size());
    for (qint32 i = 0; i < modData.size(); ++i)
    {
        pStream << modData[i];
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
    pStream >> password;
    qint32 size = 0;
    pStream >> size;
    modData.clear();
    for (qint32 i = 0; i < size; ++i)
    {
        QString value;
        pStream >> value;
        modData.append(value);
    }
}
