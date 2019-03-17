#include "gamerules.h"

#include "coreengine/interpreter.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

GameRules::GameRules()
    : QObject()
{
    Interpreter::setCppOwnerShip(this);
}

void GameRules::addVictoryRule(QString rule)
{
    bool found = false;
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        if (m_VictoryRules[i]->getRuleID() == rule)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        m_VictoryRules.append(new VictoryRule(rule));
    }
}

void GameRules::removeVictoryRule(QString rule)
{
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        if (m_VictoryRules[i]->getRuleID() == rule)
        {
            m_VictoryRules.removeAt(i);
            break;
        }
    }
}

void GameRules::checkVictory()
{
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        m_VictoryRules[i]->checkDefeat();
    }
    GameMap* pMap = GameMap::getInstance();
    QVector<qint32> teamsAlive;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = pMap->getPlayer(i);
        if (!pPlayer->getIsDefeated())
        {
            if (!teamsAlive.contains(pPlayer->getTeam()))
            {
                teamsAlive.append(pPlayer->getTeam());
            }
        }
    }
    if (teamsAlive.size() <= 1)
    {
        // go to victory screen
        if (teamsAlive.size() == 1)
        {
            emit signalVictory(teamsAlive[0]);
        }
        else
        {
            emit signalVictory(-1);
        }
    }
}

void GameRules::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_VictoryRules.size();
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        m_VictoryRules[i]->serialize(pStream);
    }
}

void GameRules::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        m_VictoryRules.append(new VictoryRule());
        m_VictoryRules[i]->deserialize(pStream);
    }
}
