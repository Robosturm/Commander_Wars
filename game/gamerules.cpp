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
        qint32 weatherChance = 30 / (pGameRuleManager->getWeatherCount() - 1);
        for (qint32 i = 0; i < pGameRuleManager->getWeatherCount(); i++)
        {
            if (i == 0)
            {
                addWeather(pGameRuleManager->getWeatherID(i), 70);
            }
            else
            {
                addWeather(pGameRuleManager->getWeatherID(i), weatherChance);
            }
        }
    }
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    for (qint32 i = 0; i < pCOSpriteManager->getCOCount(); i++)
    {
        m_COBannlist.append(pCOSpriteManager->getCOID(i));
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

Weather* GameRules::getWeatherAtDay(qint32 futureDay, qint32 player)
{
    if (futureDay >= 0 &&
        futureDay < m_WeatherDays.size() &&
        player >= 0 &&
        player < m_WeatherDays[futureDay].size())
    {
        return getWeather(m_WeatherDays[futureDay][player]);
    }
    return nullptr;
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

void GameRules::startOfTurn(bool newDay)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    if (newDay && m_WeatherDays.size() > 0)
    {
        // delete last day weather
        m_WeatherDays.removeAt(0);
    }
    // todo maybe make this changeable some day
    const qint32 predictionSize = 4;

    qint32 dayInsert = -1;
    qint32 currentPlayer = pMap->getCurrentPlayer()->getPlayerID();
    if (m_WeatherDays.size() < predictionSize)
    {
        dayInsert = m_WeatherDays.size();
    }
    else if (m_WeatherDays[0][currentPlayer] < 0)
    {
        dayInsert = 0;
    }
    if (dayInsert >= 0)
    {
        qint32 playerCount = pMap->getPlayerCount();
        // increase weather prediction till enough data is avaiable
        while(m_WeatherDays.size() < predictionSize)
        {
            if (m_randomWeather)
            {
                qint32 totalWeatherChances = 0;
                for (qint32 i = 0; i < m_WeatherChances.size(); i++)
                {
                    totalWeatherChances += m_WeatherChances[i];
                }
                qint32 erg = Mainapp::randInt(0, totalWeatherChances - 1);
                totalWeatherChances = 0;
                for (qint32 i = 0; i < m_WeatherChances.size(); i++)
                {
                    if (erg < totalWeatherChances + m_WeatherChances[i] ||
                        i == m_WeatherChances.size() - 1)
                    {
                        changeWeather(i, playerCount, dayInsert, false);
                        break;
                    }
                    totalWeatherChances += m_WeatherChances[i];
                }
            }
            else
            {
                changeWeather(getStartWeather() , playerCount, dayInsert, false);
            }
            dayInsert += 1;
        }
    }
    if (newDay)
    {
        if (m_randomWeather)
        {
            m_DayWeather = m_WeatherDays[0][currentPlayer];
        }
        else
        {
            m_DayWeather = m_StartWeather;
        }
    }
    setCurrentWeather(m_WeatherDays[0][currentPlayer]);
    createFogVision();
    pApp->continueThread();
}

void GameRules::setStartWeather(qint32 index)
{
    m_StartWeather = index;
}

void GameRules::changeWeather(QString weatherId, qint32 duration, qint32 startDay)
{
    for (qint32 i = 0; i < m_Weathers.size(); i++)
    {
        if (m_Weathers[i]->getWeatherId() == weatherId)
        {
            changeWeather(i, duration, startDay);
            break;
        }
    }
}

void GameRules::changeWeather(qint32 weatherId, qint32 duration, qint32 startDay, bool apply)
{
    if (weatherId >= 0 && weatherId < m_Weathers.size())
    {
        GameMap* pMap = GameMap::getInstance();
        qint32 startPlayer = pMap->getCurrentPlayer()->getPlayerID();
        qint32 playerCount = pMap->getPlayerCount();
        qint32 day = startDay;
        while (duration > 0)
        {
            if (day >= m_WeatherDays.size())
            {
                m_WeatherDays.append(QVector<qint32>(playerCount, -1));
                if (startPlayer > 0)
                {
                    m_WeatherDays.append(QVector<qint32>(playerCount, -1));
                }
            }
            // current player to end
            for (qint32 i = startPlayer; i < playerCount; i++)
            {
                if (duration > 0)
                {
                    // set weather for the day
                    m_WeatherDays[day][i] = weatherId;
                    duration--;
                }
            }
            if (startPlayer > 0)
            {
                // player 0 to start player
                day++;
                for (qint32 i = 0; i < startPlayer; i++)
                {
                    if (duration > 0)
                    {
                        // set weather for the day
                        m_WeatherDays[day][i] = weatherId;
                        duration--;
                    }
                }
            }
            else
            {
                day++;
            }
        }
        // update day weather for cop's and effects happened during a day
        if (startPlayer > 0)
        {
            for (qint32 i = startPlayer; i < playerCount; i++)
            {
                m_WeatherDays[day][i] = m_DayWeather;
            }
        }
        if (apply)
        {
            setCurrentWeather(m_WeatherDays[0][startPlayer]);
        }
    }
}

void GameRules::setCurrentWeather(qint32 weatherId)
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
        // create weather sprites :)
        createWeatherSprites();
    }
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
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
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
    if (pPlayer != nullptr)
    {
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

bool GameRules::getWeatherPrediction() const
{
    return m_WeatherPrediction;
}

void GameRules::setWeatherPrediction(bool WeatherPrediction)
{
    m_WeatherPrediction = WeatherPrediction;
}

QStringList GameRules::getCOBannlist() const
{
    return m_COBannlist;
}

void GameRules::setCOBannlist(const QStringList &COBannlist)
{
    m_COBannlist = COBannlist;
    m_COBannlistEdited = true;
}

bool GameRules::getAiAttackTerrain() const
{
    return m_AiAttackTerrain;
}

void GameRules::setAiAttackTerrain(bool AiAttackTerrain)
{
    m_AiAttackTerrain = AiAttackTerrain;
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

    pStream << static_cast<qint32>(m_WeatherDays.size());
    for (qint32 i = 0; i < m_WeatherDays.size(); i++)
    {
        pStream << static_cast<qint32>(m_WeatherDays[i].size());
        for (qint32 i2 = 0; i2 < m_WeatherDays[i].size(); i2++)
        {
            pStream << m_WeatherDays[i][i2];
        }
    }
    pStream << m_CurrentWeather;
    pStream << m_StartWeather;
    pStream << m_randomWeather;
    pStream << m_RankingSystem;
    pStream << m_NoPower;
    pStream << m_UnitLimit;
    pStream << static_cast<qint32>(m_FogMode);
    pStream << roundTime;
    pStream << m_RoundTimer.interval();
    pStream << m_AiAttackTerrain;
    pStream << m_COBannlistEdited;
    pStream << static_cast<qint32>(m_COBannlist.size());
    for (qint32 i = 0; i < m_COBannlist.size(); i++)
    {
        pStream << m_COBannlist[i];
    }    
    pStream << m_WeatherPrediction;
    pStream << m_DayWeather;
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
    qint32 weatherDuration = 0;
    if (version > 4)
    {
        qint32 size = 0;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            qint32 size2 = 0;
            pStream >> size2;
            m_WeatherDays.append(QVector<qint32>());
            for (qint32 i2 = 0; i2 < size2; i2++)
            {
                qint32 value = 0;
                pStream >> value;
                m_WeatherDays[i].append(value);
            }
        }
    }
    else
    {

        pStream >> weatherDuration;
    }
    pStream >> m_CurrentWeather;
    pStream >> m_StartWeather;
    if (m_StartWeather < 0 || m_StartWeather >= m_Weathers.size())
    {
        m_StartWeather = 0;
    }
    if (version <= 4)
    {
        GameMap* pMap = GameMap::getInstance();
        qint32 startPlayer  = pMap->getCurrentPlayer()->getPlayerID();
        qint32 playerCount = pMap->getPlayerCount();
        qint32 day = 0;
        // loop while remaining counter >= 0
        while (weatherDuration > 0)
        {
            // add new day
            m_WeatherDays.append(QVector<qint32>(playerCount, -1));
            if (day == 0)
            {
                // start at current player
                for (qint32 i = startPlayer; i < playerCount; i++)
                {
                    if (weatherDuration > 0)
                    {
                        // set weather for the day
                        m_WeatherDays[day][i] = m_CurrentWeather;
                        weatherDuration--;
                    }
                }
            }
            else
            {
                // start at player 0
                for (qint32 i = 0; i < playerCount; i++)
                {
                    if (weatherDuration > 0)
                    {
                        // set weather for the day
                        m_WeatherDays[day][i] = m_CurrentWeather;
                        weatherDuration--;
                    }
                }
            }
            day ++;
        }
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
    m_COBannlist.clear();
    if (version > 3)
    {
        pStream >> m_AiAttackTerrain;
        pStream >> m_COBannlistEdited;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            QString coid;
            pStream >> coid;
            m_COBannlist.append(coid);
        }
    }
    if (version <= 3 || !m_COBannlistEdited)
    {
        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        for (qint32 i = 0; i < pCOSpriteManager->getCOCount(); i++)
        {
            m_COBannlist.append(pCOSpriteManager->getCOID(i));
        }
    }
    if (version > 5)
    {
        pStream >> m_WeatherPrediction;
        pStream >> m_DayWeather;
    }
}
