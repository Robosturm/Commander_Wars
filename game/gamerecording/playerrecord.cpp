#include "game/gamerecording/playerrecord.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

PlayerRecord::PlayerRecord()
    : QObject()
{
    setObjectName("PlayerRecord");
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

PlayerRecord::PlayerRecord(qint32 day, qint32 player, qint32 funds, qint32 income,
                          qint32 buildings, qint32 units, qint32 playerStrength)
    : m_Day(day),
      m_Player(player),
      m_Funds(funds),
      m_Income(income),
      m_Buildings(buildings),
      m_Units(units),
      m_PlayerStrength(playerStrength)
{
    setObjectName("PlayerRecord");
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void PlayerRecord::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_Day;
    pStream << m_Player;
    pStream << m_Funds;
    pStream << m_Income;
    pStream << m_Buildings;
    pStream << m_Units;
    pStream << m_PlayerStrength;
}

void PlayerRecord::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_Day;
    pStream >> m_Player;
    pStream >> m_Funds;
    pStream >> m_Income;
    pStream >> m_Buildings;
    pStream >> m_Units;
    pStream >> m_PlayerStrength;
}

qint32 PlayerRecord::getDay() const
{
    return m_Day;
}

qint32 PlayerRecord::getOwner() const
{
    return m_Player;
}

qint32 PlayerRecord::getFunds() const
{
    return m_Funds;
}

qint32 PlayerRecord::getIncome() const
{
    return m_Income;
}

qint32 PlayerRecord::getBuildings() const
{
    return m_Buildings;
}

qint32 PlayerRecord::getUnits() const
{
    return m_Units;
}

qint32 PlayerRecord::getPlayerStrength() const
{
    return m_PlayerStrength;
}
