#include "gamerules.h"

#include "coreengine/interpreter.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unit.h"

#include "resource_management/cospritemanager.h"

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
void GameRules::addVictoryRule(spVictoryRule rule)
{
    bool found = false;
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        if (m_VictoryRules[i]->getRuleID() == rule->getRuleID())
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        m_VictoryRules.append(rule);
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

void GameRules::addWeather(QString weatherId, qint32 weatherChance)
{
    bool found = false;
    for (qint32 i = 0; i < m_Weathers.size(); i++)
    {
        if (m_Weathers[i]->getWeatherId() == weatherId)
        {
            m_WeatherChances[i] = weatherChance;
            found = true;
            break;
        }
    }
    if (!found)
    {
        m_Weathers.append(new Weather(weatherId));
        m_WeatherChances.append(weatherChance);
    }
}

void GameRules::changeWeatherChance(QString weatherId, qint32 weatherChance)
{
    for (qint32 i = 0; i < m_Weathers.size(); i++)
    {
        if (m_Weathers[i]->getWeatherId() == weatherId)
        {
            m_WeatherChances[i] = weatherChance;
            break;
        }
    }
}

void GameRules::startOfTurn()
{
    GameMap* pMap = GameMap::getInstance();
    m_weatherDuration -= 1;
    if (m_weatherDuration <= 0)
    {
        // calc a pseudo random
        qint32 heigth = pMap->getMapHeight();
        qint32 width = pMap->getMapWidth();
        qint32 playerCount = pMap->getPlayerCount();
        if (randCounter <= 0)
        {
            randCounter = 100;
            quint32 value = static_cast<quint32>(heigth) * static_cast<quint32>(width);
            for (qint32 x = 0; x < width; x++)
            {
                for (qint32 y = 0; y < heigth; y++)
                {
                    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                    if (pUnit != nullptr)
                    {
                        value += static_cast<quint32>(pUnit->getUnitCosts());
                    }
                }
            }
            COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
            for (qint32 i = 0; i < playerCount; i++)
            {
                Player* pPlayer = pMap->getPlayer(i);
                CO* pCO = pPlayer->getCO(0);
                if (pCO != nullptr)
                {
                    for (qint32 i2 = 0; i2 < pCOSpriteManager->getCOCount(); i2++)
                    {
                        if (pCOSpriteManager->getCOID(i2) == pCO->getCoID())
                        {
                            value += static_cast<quint32>(i2);
                            break;
                        }
                    }
                }
                pCO = pPlayer->getCO(1);
                if (pCO != nullptr)
                {
                    for (qint32 i2 = 0; i2 < pCOSpriteManager->getCOCount(); i2++)
                    {
                        if (pCOSpriteManager->getCOID(i2) == pCO->getCoID())
                        {
                            value += static_cast<quint32>(i2);
                            break;
                        }
                    }
                }
            }
            randInt = QRandomGenerator(value);
        }
        else
        {
            randCounter--;
        }
        qint32 totalWeatherChances = 0;
        for (qint32 i = 0; i < m_WeatherChances.size(); i++)
        {
            totalWeatherChances += m_WeatherChances[i];
        }


        qint32 erg = randInt.bounded(0, totalWeatherChances);
        totalWeatherChances = 0;
        for (qint32 i = 0; i < m_WeatherChances.size(); i++)
        {
            if (erg < totalWeatherChances + m_WeatherChances[i])
            {
                changeWeather(m_Weathers[i]->getWeatherId() , playerCount);
                break;
            }
            totalWeatherChances += m_WeatherChances[i];
        }
    }
}

void GameRules::changeWeather(QString weatherId, qint32 duration)
{
    for (qint32 i = 0; i < m_Weathers.size(); i++)
    {
        if (m_Weathers[i]->getWeatherId() == weatherId)
        {
            m_Weathers[m_CurrentWeather]->deactivate();
            m_CurrentWeather = i;
            m_Weathers[m_CurrentWeather]->activate();
            break;
        }
    }
    m_weatherDuration = duration;
    // create weather sprites :)
}

void GameRules::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_VictoryRules.size();
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        m_VictoryRules[i]->serialize(pStream);
    }

    pStream << m_Weathers.size();
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        m_Weathers[i]->serialize(pStream);
        pStream << m_WeatherChances[i];
    }
    pStream << m_weatherDuration;
    pStream << m_CurrentWeather;
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
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        m_Weathers.append(new Weather());
        m_Weathers[i]->deserialize(pStream);
        qint32 chance = 0;
        pStream >> chance;
        m_WeatherChances.append(chance);
    }
    pStream >> m_weatherDuration;
    pStream >> m_CurrentWeather;
}
