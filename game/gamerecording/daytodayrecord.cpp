#include "game/gamerecording/daytodayrecord.h"

#include "game/gamemap.h"
#include "game/player.h"

DayToDayRecord::DayToDayRecord(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DayToDayRecord");
#endif
    Interpreter::setCppOwnerShip(this);
}

DayToDayRecord::DayToDayRecord(GameMap* pMap, qint32 playerCount)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DayToDayRecord");
#endif
    Interpreter::setCppOwnerShip(this);
    for (qint32 i = 0; i < playerCount; i++)
    {
        m_PlayerRecords.append(MemoryManagement::create<PlayerRecord>());
    }
}

void DayToDayRecord::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_SpecialEvents.size());
    for (qint32 i = 0; i < m_SpecialEvents.size(); i++)
    {
        m_SpecialEvents[i]->serializeObject(pStream);
    }
    pStream << static_cast<qint32>(m_PlayerRecords.size());
    for (qint32 i = 0; i < m_PlayerRecords.size(); i++)
    {
        m_PlayerRecords[i]->serializeObject(pStream);
    }
}

void DayToDayRecord::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    m_SpecialEvents.clear();
    for (qint32 i = 0; i < size; i++)
    {
        m_SpecialEvents.append(MemoryManagement::create<SpecialEvent>());
        m_SpecialEvents[i]->deserializeObject(pStream);
    }
    pStream >> size;
    m_PlayerRecords.clear();
    for (qint32 i = 0; i < size; i++)
    {
        m_PlayerRecords.append(MemoryManagement::create<PlayerRecord>());
        m_PlayerRecords[i]->deserializeObject(pStream);
    }
}

void DayToDayRecord::addSpecialEvent(qint32 player, qint32 day, GameEnums::GameRecord_SpecialEvents event)
{
    m_SpecialEvents.append(MemoryManagement::create<SpecialEvent>(player, day, event));
}

PlayerRecord* DayToDayRecord::getPlayerRecord(qint32 player)
{
    if (player >= 0 &&
        player < m_PlayerRecords.size())
    {
        return m_PlayerRecords[player].get();
    }
    return nullptr;
}
SpecialEvent* DayToDayRecord::getSpecialEvent(qint32 index)
{
    if (index >= 0 &&
        index < m_SpecialEvents.size())
    {
        return m_SpecialEvents[index].get();
    }
    return nullptr;
}

void DayToDayRecord::addPlayerRecord(qint32 player, qint32 day)
{    
    Player* pPlayer = m_pMap->getPlayer(player);
    if (!pPlayer->getIsDefeated())
    {
        m_PlayerRecords[player] = MemoryManagement::create<PlayerRecord>(day, player, pPlayer->getFunds(), pPlayer->calcIncome(),
                                                                         pPlayer->getBuildingCount(), pPlayer->getUnitCount(), pPlayer->calculatePlayerStrength());
    }
}

void DayToDayRecord::addPlayerRecord(qint32 day, qint32 player, qint32 funds, qint32 income,
                                     qint32 buildings, qint32 units, qint32 playerStrength)
{
    Player* pPlayer = m_pMap->getPlayer(player);
    if (!pPlayer->getIsDefeated())
    {
        m_PlayerRecords[player] = MemoryManagement::create<PlayerRecord>(day, player, funds, income, buildings, units, playerStrength);
    }
}
