#include "game/gamerecording/daytodayrecord.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

DayToDayRecord::DayToDayRecord()
    : QObject()
{
    setObjectName("DayToDayRecord");
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

DayToDayRecord::DayToDayRecord(qint32 playerCount)
{
    setObjectName("DayToDayRecord");
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    for (qint32 i = 0; i < playerCount; i++)
    {
        m_PlayerRecords.append(spPlayerRecord::create());
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
        m_SpecialEvents.append(spSpecialEvent::create());
        m_SpecialEvents[i]->deserializeObject(pStream);
    }
    pStream >> size;
    m_PlayerRecords.clear();
    for (qint32 i = 0; i < size; i++)
    {
        m_PlayerRecords.append(spPlayerRecord::create());
        m_PlayerRecords[i]->deserializeObject(pStream);
    }
}

void DayToDayRecord::addSpecialEvent(qint32 player, qint32 day, GameEnums::GameRecord_SpecialEvents event)
{
    m_SpecialEvents.append(spSpecialEvent::create(player, day, event));
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
    spGameMap pMap = GameMap::getInstance();
    Player* pPlayer = pMap->getPlayer(player);
    if (!pPlayer->getIsDefeated())
    {
        m_PlayerRecords[player] = spPlayerRecord::create(day, player, pPlayer->getFunds(), pPlayer->calcIncome(),
                                                         pPlayer->getBuildingCount(), pPlayer->getUnitCount(), pPlayer->calculatePlayerStrength());
    }
}
