#include "gamerules.h"

#include "coreengine/interpreter.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unit.h"

#include "resource_management/cospritemanager.h"

#include "resource_management/gamemanager.h"
#include "resource_management/gamerulemanager.h"

#include "game/gameanimationfactory.h"
#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"

GameRules::GameRules()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    if (getWeatherCount() != pGameRuleManager->getWeatherCount())
    {
        qint32 weatherChance = 100 / pGameRuleManager->getWeatherCount();
        for (qint32 i = 0; i < pGameRuleManager->getWeatherCount(); i++)
        {
            addWeather(pGameRuleManager->getWeatherID(i), weatherChance);
        }
    }
    m_StartWeather = 0;
    m_RoundTimer.setSingleShot(true);
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

VictoryRule* GameRules::getVictoryRule(QString rule)
{
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        if (m_VictoryRules[i]->getRuleID() == rule)
        {
            return m_VictoryRules[i].get();
        }
    }
    return nullptr;
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    GameMap* pMap = GameMap::getInstance();
    if (pMap != nullptr)
    {
        for (qint32 i = 0; i < m_VictoryRules.size(); i++)
        {
            m_VictoryRules[i]->checkDefeat();
        }
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
     pApp->continueThread();
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

void GameRules::changeWeatherChance(qint32 index, qint32 weatherChance)
{
    if ((index >= 0) && (index < m_Weathers.size()))
    {
        m_WeatherChances[index] = weatherChance;
    }
}

Weather* GameRules::getWeather(qint32 index)
{
    if ((index >= 0) && (index < m_Weathers.size()))
    {
        return m_Weathers[index].get();
    }
    return nullptr;
}

Weather* GameRules::getWeather(QString weatherId)
{
    for (qint32 i = 0; i < m_Weathers.size(); i++)
    {
        if (m_Weathers[i]->getWeatherId() == weatherId)
        {
            return m_Weathers[i].get();
        }
    }
    return nullptr;
}

qint32 GameRules::getWeatherChance(QString weatherId)
{
    for (qint32 i = 0; i < m_WeatherChances.size(); i++)
    {
        if (m_Weathers[i]->getWeatherId() == weatherId)
        {
            return m_WeatherChances[i];
        }
    }
    return 0;
}

void GameRules::reduceWeatherDuration(qint32 duration)
{
    m_weatherDuration -= duration;
}

void GameRules::startOfTurn()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    m_weatherDuration -= 1;
    if (m_weatherDuration <= 0)
    {

        qint32 playerCount = pMap->getPlayerCount();
        if (m_randomWeather)
        {
            qint32 totalWeatherChances = 0;
            for (qint32 i = 0; i < m_WeatherChances.size(); i++)
            {
                totalWeatherChances += m_WeatherChances[i];
            }
            qint32 erg = Mainapp::randInt(0, totalWeatherChances);
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
        else
        {
            changeWeather(m_Weathers[getStartWeather()]->getWeatherId() , playerCount);
        }
    }
    createFogVision();
    pApp->continueThread();
}

void GameRules::setStartWeather(qint32 index)
{
    m_StartWeather = index;
}

void GameRules::changeWeather(QString weatherId, qint32 duration)
{
    for (qint32 i = 0; i < m_Weathers.size(); i++)
    {
        if (m_Weathers[i]->getWeatherId() == weatherId)
        {
            changeWeather(i, duration);
            break;
        }
    }
}

void GameRules::changeWeather(qint32 weatherId, qint32 duration)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (weatherId >= 0 && weatherId < m_Weathers.size())
    {
        if (m_CurrentWeather >= 0)
        {
            m_Weathers[m_CurrentWeather]->deactivate();
        }
        m_CurrentWeather = weatherId;
        m_Weathers[m_CurrentWeather]->activate();
        m_weatherDuration = duration;
    }
    // create weather sprites :)
    createWeatherSprites();
    pApp->continueThread();
}

void GameRules::createWeatherSprites()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if ((m_CurrentWeather < 0) && (m_CurrentWeather < m_Weathers.size()))
    {
        m_CurrentWeather = 0;
    }
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < m_WeatherSprites.size(); i++)
    {
        m_WeatherSprites[i]->detach();
        m_WeatherSprites[i] = nullptr;
    }
    m_WeatherSprites.clear();
    if (m_Weathers.size() > 0)
    {
        qint32 width = pMap->getMapWidth();
        qint32 heigth = pMap->getMapHeight();
        QString weatherSprite = m_Weathers[m_CurrentWeather]->getWeatherTerrainSprite();
        if (!weatherSprite.isEmpty())
        {
            oxygine::ResAnim* pAnim = GameManager::getInstance()->getResAnim(weatherSprite.toStdString().c_str());
            if (pAnim != nullptr)
            {
                for (qint32 x = 0; x < width; x++)
                {
                    for (qint32 y = 0; y < heigth; y++)
                    {
                        oxygine::spSprite pSprite = new oxygine::Sprite();
                        if (pAnim->getTotalFrames() > 1)
                        {
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                            pSprite->addTween(tween);
                        }
                        else
                        {
                            pSprite->setResAnim(pAnim);
                        }
                        pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
                        pSprite->setPosition(x * GameMap::Imagesize, y * GameMap::Imagesize);
                        pSprite->setPriority(static_cast<qint16>(Mainapp::ZOrder::Weather));
                        m_WeatherSprites.append(pSprite);
                        pMap->addChild(pSprite);
                    }
                }
            }
        }
    }
    pApp->continueThread();
}

qint32 GameRules::getUnitLimit() const
{
    return m_UnitLimit;
}

void GameRules::setUnitLimit(const qint32 &UnitLimit)
{
    m_UnitLimit = UnitLimit;
}

bool GameRules::getRandomWeather() const
{
    return m_randomWeather;
}

void GameRules::setRandomWeather(bool randomWeather)
{
    m_randomWeather = randomWeather;
}

GameEnums::Fog GameRules::getFogMode() const
{
    return m_FogMode;
}

void GameRules::setFogMode(const GameEnums::Fog &FogMode)
{
    m_FogMode = FogMode;
}

void GameRules::createFogVision()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    GameMap* pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    if (m_FogSprites.size() == 0)
    {
        for (qint32 x = 0; x < width; x++)
        {
            m_FogSprites.append(QVector<oxygine::spSprite>());
            for (qint32 y = 0; y < heigth; y++)
            {
                m_FogSprites[x].append(nullptr);
            }
        }
    }
    // update vision for each player
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->updatePlayerVision(false);
    }
    // get player for which we should create the vision
    Player* pPlayer = pMap->getCurrentViewPlayer();
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            bool visible = pPlayer->getFieldVisible(x, y);
            Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            switch (m_FogMode)
            {
                case GameEnums::Fog_Off:
                {
                    if (m_FogSprites[x][y].get() != nullptr)
                    {
                        pMap->removeChild(m_FogSprites[x][y]);
                        m_FogSprites[x][y] = nullptr;
                    }
                    if (pUnit != nullptr)
                    {
                        showHideStealthUnit(pPlayer, pUnit);
                    }
                    if (pBuilding != nullptr)
                    {
                        pBuilding->updatePlayerColor(true);
                    }
                    break;
                }
                case GameEnums::Fog_OfWar:
                {
                    if (pUnit != nullptr)
                    {
                        showHideStealthUnit(pPlayer, pUnit);
                        if (!visible)
                        {
                            pUnit->setUnitVisible(false);
                        }
                    }
                    if (pBuilding != nullptr)
                    {
                        pBuilding->updatePlayerColor(visible);
                    }
                    if (!visible)
                    {
                        if (m_FogSprites[x][y].get() == nullptr)
                        {
                            // create fog of war sprite
                            oxygine::spColorRectSprite sprite = new oxygine::ColorRectSprite();
                            sprite->setSize(GameMap::Imagesize, GameMap::Imagesize);
                            sprite->setColor(70, 70, 70, 100);
                            sprite->setPosition(GameMap::Imagesize * x, y * GameMap::Imagesize);
                            sprite->setPriority(static_cast<qint16>(Mainapp::ZOrder::FogFields));
                            pMap->addChild(sprite);
                            m_FogSprites[x][y] = sprite;
                        }
                    }
                    else if (m_FogSprites[x][y].get() != nullptr)
                    {
                        pMap->removeChild(m_FogSprites[x][y]);
                        m_FogSprites[x][y] = nullptr;
                    }
                    break;
                }
            }
        }
    }
    pApp->continueThread();
}

void GameRules::showHideStealthUnit(Player* pPlayer, Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    switch (pPlayer->checkAlliance(pUnit->getOwner()))
    {
        case GameEnums::Alliance_Enemy:
        {
            if (pUnit->isStealthed(pPlayer))
            {
                pUnit->setUnitVisible(false);
            }
            else
            {
                pUnit->setUnitVisible(true);
            }
            break;
        }
        case GameEnums::Alliance_Friend:
        {
            pUnit->setUnitVisible(true);
            break;
        }
    }
    pApp->continueThread();
}

qint32 GameRules::getStartWeather() const
{
    if (m_StartWeather < 0 || m_StartWeather >= m_Weathers.size())
    {
        return 0;
    }
    return m_StartWeather;
}

bool GameRules::getNoPower() const
{
    return m_NoPower;
}

void GameRules::setNoPower(bool NoPower)
{
    m_NoPower = NoPower;
}

bool GameRules::getRankingSystem() const
{
    return m_RankingSystem;
}

void GameRules::setRankingSystem(bool RankingSystem)
{
    m_RankingSystem = RankingSystem;
}

void GameRules::resumeRoundTime()
{
    if (roundTime > 0)
    {
        m_RoundTimer.resume();
    }
}

void GameRules::initRoundTime()
{
    if (roundTime > 0)
    {
        m_RoundTimer.setInterval(roundTime);
    }
}

void GameRules::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_VictoryRules.size());
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        m_VictoryRules[i]->serializeObject(pStream);
    }

    pStream << static_cast<qint32>(m_Weathers.size());
    for (qint32 i = 0; i < m_Weathers.size(); i++)
    {
        m_Weathers[i]->serializeObject(pStream);
        pStream << m_WeatherChances[i];
    }
    pStream << m_weatherDuration;
    pStream << m_CurrentWeather;
    pStream << m_StartWeather;
    pStream << m_randomWeather;
    pStream << m_RankingSystem;
    pStream << m_NoPower;
    pStream << m_UnitLimit;
    pStream << static_cast<qint32>(m_FogMode);
    pStream << roundTime;
    pStream << m_RoundTimer.interval();
}

void GameRules::deserializeObject(QDataStream& pStream)
{
    m_VictoryRules.clear();
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        m_VictoryRules.append(new VictoryRule());
        m_VictoryRules[i]->deserializeObject(pStream);
    }
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        spWeather pWeather = new Weather();
        pWeather->deserializeObject(pStream);
        qint32 chance = 0;
        pStream >> chance;
        for (qint32 i2 = 0; i2 < m_Weathers.size(); i2++)
        {
            if (m_Weathers[i2]->getWeatherId() == pWeather->getWeatherId())
            {
                m_Weathers[i] = pWeather;
                m_WeatherChances[i2] = chance;
                break;
            }
            else if (i2 == m_Weathers.size() - 1)
            {
                m_Weathers.append(pWeather);
                m_WeatherChances.append(chance);
            }
        }
    }
    pStream >> m_weatherDuration;
    pStream >> m_CurrentWeather;
    pStream >> m_StartWeather;
    if (m_StartWeather < 0 || m_StartWeather >= m_Weathers.size())
    {
        m_StartWeather = 0;
    }
    pStream >> m_randomWeather;
    pStream >> m_RankingSystem;
    pStream >> m_NoPower;
    pStream >> m_UnitLimit;
    if (version > 1)
    {
        qint32 value = 0;
        pStream >> value;
        m_FogMode = static_cast<GameEnums::Fog>(value);
    }
    if (version > 2)
    {
        pStream >> roundTime;
        qint32 intervall;
        pStream >> intervall;
        m_RoundTimer.setInterval(intervall);
    }
}
