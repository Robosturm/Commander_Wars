#include "playerrecord.h"

#include "coreengine/mainapp.h"

PlayerRecord::PlayerRecord()
    : QObject()
{
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
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void PlayerRecord::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_Day;
    pStream << m_Player;
    pStream << m_Funds;
    pStream << m_Income;
    pStream << m_Buildings;
    pStream << m_Units;
    pStream << m_PlayerStrength;
    pStream << m_Attackreports.size();
    for (qint32 i = 0; i < m_Attackreports.size(); i++)
    {
        pStream << m_Attackreports[i].attackerDamage;
        pStream << m_Attackreports[i].attackerX;
        pStream << m_Attackreports[i].attackerY;
        pStream << m_Attackreports[i].attackerID;
        pStream << m_Attackreports[i].attackerOwnerID;
        pStream << m_Attackreports[i].defenderX;
        pStream << m_Attackreports[i].defenderY;
        pStream << m_Attackreports[i].defenderDamage;
        pStream << m_Attackreports[i].defenderID;
    }
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
    if (version > 1)
    {
        qint32 size = 0;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            m_Attackreports.append(AttackReport());
            pStream >> m_Attackreports[i].attackerDamage;
            pStream >> m_Attackreports[i].attackerX;
            pStream >> m_Attackreports[i].attackerY;
            pStream >> m_Attackreports[i].attackerID;
            pStream >> m_Attackreports[i].attackerOwnerID;
            pStream >> m_Attackreports[i].defenderX;
            pStream >> m_Attackreports[i].defenderY;
            pStream >> m_Attackreports[i].defenderDamage;
            pStream >> m_Attackreports[i].defenderID;
        }
    }
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
