#include "coreengine/mainapp.h"
#include "coreengine/filesupport.h"
#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

#include "game/gamerules.h"
#include "game/gamemap.h"
#include "game/player.h"
#include "game/unit.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/gamerulemanager.h"
#include "resource_management/coperkmanager.h"
#include "resource_management/gamemanager.h"

GameRules::GameRules(GameMap* pMap)
    : m_RoundTimer(this),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameRules");
#endif
    Interpreter::setCppOwnerShip(this);
    reset();
}

void GameRules::resetArrays()
{
    m_WeatherDays.clear();
    m_WeatherSprites.clear();
    m_VictoryRules.clear();
    m_COBannlist.clear();
    m_allowedPerks.clear();
    m_allowedActions.clear();
    m_observerList.clear();
    m_GameRules.clear();
    m_FogSprites.clear();
}

bool GameRules::getParallelCos() const
{
    return m_parallelCos;
}

void GameRules::setParallelCos(bool newParallelCos)
{
    m_parallelCos = newParallelCos;
}

GameEnums::DamageFormula GameRules::getDamageFormula() const
{
    return m_damageFormula;
}

void GameRules::setDamageFormula(const GameEnums::DamageFormula newDamageFormula)
{
    m_damageFormula = newDamageFormula;
}

qint32 GameRules::getMapPalette() const
{
    return m_mapPalette;
}

void GameRules::setMapPalette(qint32 newMapPalette)
{
    m_mapPalette = newMapPalette;
}

void GameRules::reset()
{
    CONSOLE_PRINT("GameRules::reset", GameConsole::eDEBUG);
    resetArrays();
    m_WeatherChances.clear();
    m_Weathers.clear();

    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    if (getWeatherCount() != pGameRuleManager->getWeatherCount())
    {
        for (qint32 i = 0; i < pGameRuleManager->getWeatherCount(); i++)
        {
            QString id = pGameRuleManager->getWeatherID(i);
            addWeather(id, pGameRuleManager->getDefaultWeatherChance(id));
        }
    }
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
    {
        QString coid = pCOSpriteManager->getID(i);
        if (!m_COBannlist.contains(coid))
        {
            m_COBannlist.append(coid);
        }
    }
    m_weatherMaster = MemoryManagement::create<oxygine::Sprite>();
    m_weatherMaster->setPriority(static_cast<qint32>(Mainapp::ZOrder::Weather));
    m_pMap->addChild(m_weatherMaster);

    m_fogMaster = MemoryManagement::create<oxygine::Sprite>();
    m_fogMaster->setPriority(static_cast<qint32>(Mainapp::ZOrder::FogFields));
    m_pMap->addChild(m_fogMaster);

    m_StartWeather = 0;
    m_RoundTimer.setSingleShot(true);
    m_allowedPerks = COPerkManager::getInstance()->getLoadedRessources();
    m_allowedActions = GameManager::getInstance()->getLoadedRessources();
}

void GameRules::onGameStart()
{
    CONSOLE_PRINT("On Game start rule selection enabling/disabling rules for the map.", GameConsole::eDEBUG);
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();

    for (qint32 i = 0; i < pGameRuleManager->getVictoryRuleCount(); i++)
    {
        QString ruleID = pGameRuleManager->getVictoryRuleID(i);
        VictoryRule* pRule = getVictoryRule(ruleID);
        if (pRule != nullptr)
        {
            QStringList inputTypes = pRule->getRuleType();
            if (inputTypes[0] == VictoryRule::checkbox)
            {
                qint32 ruleValue = pRule->getRuleValue(0);
                if (ruleValue == 0)
                {
                    CONSOLE_PRINT("Removing rule cause it's disabled: " + ruleID, GameConsole::eDEBUG);
                    removeVictoryRule(ruleID);
                }
                else
                {
                    CONSOLE_PRINT("Rule is enabled: " + ruleID, GameConsole::eDEBUG);
                }
            }
            else if (inputTypes[0] == VictoryRule::spinbox)
            {
                qint32 ruleValue = pRule->getRuleValue(0);
                qint32 infiniteValue = pRule->getInfiniteValue(0);
                if (ruleValue <= infiniteValue)
                {
                    CONSOLE_PRINT("Removing rule cause it's disabled: " + ruleID, GameConsole::eDEBUG);
                    removeVictoryRule(ruleID);
                }
                else
                {
                    CONSOLE_PRINT("Rule is enabled: " + ruleID, GameConsole::eDEBUG);
                }
            }
            else
            {
                CONSOLE_PRINT("Removing rule cause it's in unsupported format: " + ruleID, GameConsole::eERROR);
                removeVictoryRule(ruleID);
            }
        }
    }

    for (qint32 i = 0; i < pGameRuleManager->getGameRuleCount(); i++)
    {
        QString ruleID = pGameRuleManager->getGameRuleID(i);
        GameRule* pRule =getGameRule(ruleID);
        if (pRule != nullptr)
        {
            QStringList inputTypes = pRule->getRuleType();
            if (inputTypes[0] == VictoryRule::checkbox)
            {
                qint32 ruleValue = pRule->getRuleValue(0);
                if (ruleValue == 0)
                {
                    CONSOLE_PRINT("Removing rule cause it's disabled: " + ruleID, GameConsole::eDEBUG);
                    removeGameRule(ruleID);
                }
                else
                {
                    CONSOLE_PRINT("Rule is enabled: " + ruleID, GameConsole::eDEBUG);
                }
            }
            else if (inputTypes[0] == VictoryRule::spinbox)
            {
                qint32 ruleValue = pRule->getRuleValue(0);
                qint32 infiniteValue = pRule->getInfiniteValue(0);
                if (ruleValue <= infiniteValue)
                {
                    CONSOLE_PRINT("Removing rule cause it's disabled: " + ruleID, GameConsole::eDEBUG);
                    removeGameRule(ruleID);
                }
                else
                {
                    CONSOLE_PRINT("Rule is enabled: " + ruleID, GameConsole::eDEBUG);
                }
            }
            else
            {
                CONSOLE_PRINT("Removing rule cause it's in unsupported format: " + ruleID, GameConsole::eERROR);
                removeGameRule(ruleID);
            }
        }
    }
}

void GameRules::addGameRule(const QString rule)
{
    bool found = false;
    for (qint32 i = 0; i < m_GameRules.size(); i++)
    {
        if (m_GameRules[i]->getRuleID() == rule)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        m_GameRules.append(MemoryManagement::create<GameRule>(rule));
    }
}


spGameRule GameRules::getSpGameRule(const QString rule)
{
    for (qint32 i = 0; i < m_GameRules.size(); i++)
    {
        if (m_GameRules[i]->getRuleID() == rule)
        {
            return m_GameRules[i];
        }
    }
    return spGameRule();
}

GameRule* GameRules::getGameRule(const QString rule)
{
    return getSpGameRule(rule).get();
}

void GameRules::addGameRule(spGameRule rule)
{
    bool found = false;
    for (qint32 i = 0; i < m_GameRules.size(); i++)
    {
        if (m_GameRules[i]->getRuleID() == rule->getRuleID())
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        m_GameRules.append(rule);
    }
}

bool GameRules::hasGameRule(const QString rule)
{
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        if (m_GameRules[i]->getRuleID() == rule)
        {
            return true;
        }
    }
    return false;
}

void GameRules::removeGameRule(const QString rule)
{
    for (qint32 i = 0; i < m_GameRules.size(); i++)
    {
        if (m_GameRules[i]->getRuleID() == rule)
        {
            m_GameRules.removeAt(i);
            break;
        }
    }
}

bool GameRules::hasVictoryRule(const QString rule)
{
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        if (m_VictoryRules[i]->getRuleID() == rule)
        {
            return true;
        }
    }
    return false;
}

void GameRules::addVictoryRule(const QString rule)
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
        m_VictoryRules.append(MemoryManagement::create<VictoryRule>(rule, m_pMap));
    }
}

spVictoryRule GameRules::getSpVictoryRule(const QString rule)
{
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        if (m_VictoryRules[i]->getRuleID() == rule)
        {
            return m_VictoryRules[i];
        }
    }
    return spVictoryRule();
}

VictoryRule* GameRules::getVictoryRule(const QString rule)
{
    return getSpVictoryRule(rule).get();
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

void GameRules::removeVictoryRule(const QString rule)
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

void GameRules::init()
{
    CONSOLE_PRINT("GameRules::init", GameConsole::eDEBUG);
    for (qint32 i = 0; i < m_VictoryRules.size(); i++)
    {
        m_VictoryRules[i]->init();
    }
    
    qint32 playerCount = m_pMap->getPlayerCount();
    m_WeatherDays.append(QVector<qint32>(playerCount, -1));
    m_WeatherDays.append(QVector<qint32>(playerCount, -1));
    for (qint32 i = 0; i < playerCount; i++)
    {
        m_WeatherDays[0][i] = getStartWeather();
        m_WeatherDays[1][i] = getStartWeather();
    }
}

qint32 GameRules::getVictoryTeam()
{
    qint32 team = -1;
    
    if (m_pMap != nullptr)
    {
        QVector<qint32> teamsAlive;
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (!pPlayer->getIsDefeated())
            {
                if (!teamsAlive.contains(pPlayer->getTeam()))
                {
                    teamsAlive.append(pPlayer->getTeam());
                }
            }
        }
        if (teamsAlive.size() == 1)
        {
            team = teamsAlive[0];
        }
    }
    return team;
}

void GameRules::checkVictory()
{    
    if (m_pMap != nullptr)
    {
        CONSOLE_PRINT("Checking for victory", GameConsole::eDEBUG);
        for (qint32 i = 0; i < m_VictoryRules.size(); i++)
        {
            m_VictoryRules[i]->checkDefeat();
        }
        QVector<qint32> teamsAlive;
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
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
            CONSOLE_PRINT("Emitting victory game is finished", GameConsole::eDEBUG);
            m_victory = true;
            // go to victory screen
            if (teamsAlive.size() == 1)
            {
                emit sigVictory(teamsAlive[0]);
            }
            else
            {
                emit sigVictory(-1);
            }
        }
        if (m_pMap->getIsHumanMatch())
        {
            bool humanAlive = false;
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                Player* pPlayer = m_pMap->getPlayer(i);
                auto* baseGameInput = pPlayer->getBaseGameInput();
                if (baseGameInput != nullptr &&
                    baseGameInput->getAiType() == GameEnums::AiTypes_Human &&
                    !pPlayer->getIsDefeated())
                {
                    humanAlive = true;
                    break;
                }
            }
            if (!humanAlive)
            {
                emit sigVictory(-1);
            }
        }
    }
}

void GameRules::addWeather(const QString weatherId, qint32 weatherChance)
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
        m_Weathers.append(MemoryManagement::create<Weather>(weatherId, m_pMap));
        m_WeatherChances.append(weatherChance);
    }
}

void GameRules::changeWeatherChanceByName(QString weatherId, qint32 weatherChance)
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

Weather* GameRules::getWeather(const QString weatherId)
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

qint32 GameRules::getWeatherChance(const QString weatherId)
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
    CONSOLE_PRINT("GameRules::startOfTurn newDay=" + QString::number(newDay), GameConsole::eDEBUG);
    if (newDay && m_WeatherDays.size() > 0)
    {
        m_WeatherDays.removeAt(0);
        CONSOLE_PRINT("New Day removing current weather. Currently predicting for " + QString::number(m_WeatherDays.size()), GameConsole::eDEBUG);
    }
    const qint32 predictionSize = 4;

    qint32 dayInsert = -1;
    qint32 currentPlayer = m_pMap->getCurrentPlayer()->getPlayerID();
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
        qint32 playerCount = m_pMap->getPlayerCount();
        // increase weather prediction till enough data is avaiable
        while(m_WeatherDays.size() < predictionSize)
        {
            CONSOLE_PRINT("Adding weather for weather prediction.", GameConsole::eDEBUG);
            if (m_randomWeather)
            {
                qint32 totalWeatherChances = 0;
                for (qint32 i = 0; i < m_WeatherChances.size(); i++)
                {
                    totalWeatherChances += m_WeatherChances[i];
                }
                qint32 erg = GlobalUtils::randInt(0, totalWeatherChances - 1);
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
    
}

void GameRules::setStartWeather(qint32 index)
{
    m_StartWeather = index;
}

void GameRules::changeWeather(const QString weatherId, qint32 duration, qint32 startDay)
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
        CONSOLE_PRINT("GameRules::changeWeather to " + m_Weathers[weatherId]->getWeatherId() + " for " + QString::number(duration) + " from upcoming day " + QString::number(startDay), GameConsole::eDEBUG);
        qint32 startPlayer = m_pMap->getCurrentPlayer()->getPlayerID();
        qint32 playerCount = m_pMap->getPlayerCount();
        qint32 day = startDay;
        while (duration > 0)
        {
            if (day >= m_WeatherDays.size())
            {
                m_WeatherDays.append(QVector<qint32>(playerCount, -1));
            }
            if (startPlayer > 0 && day + 1 >= m_WeatherDays.size())
            {
                m_WeatherDays.append(QVector<qint32>(playerCount, -1));
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
    if (weatherId >= 0 && weatherId < m_Weathers.size())
    {
        if (m_CurrentWeather != weatherId)
        {
            if (m_CurrentWeather >= 0)
            {
                m_Weathers[m_CurrentWeather]->deactivate();
            }
            m_CurrentWeather = weatherId;
            CONSOLE_PRINT("Setting current weather to " + m_Weathers[m_CurrentWeather]->getWeatherId(), GameConsole::eDEBUG);
            m_Weathers[m_CurrentWeather]->activate();
        }
    }
}

void GameRules::onWeatherChanged()
{    
    if (m_pMap != nullptr)
    {
        m_pMap->onWeatherChanged(m_Weathers[m_CurrentWeather].get());
        // create weather sprites :)
        createWeatherSprites();
    }
}

void GameRules::createWeatherSprites()
{    
    CONSOLE_PRINT("creating weather Sprites", GameConsole::eDEBUG);
    if ((m_CurrentWeather < 0) && (m_CurrentWeather < m_Weathers.size()))
    {
        m_CurrentWeather = 0;
    }
    resetWeatherSprites();
    if (m_Weathers.size() > 0)
    {
        qint32 width = m_pMap->getMapWidth();
        qint32 heigth = m_pMap->getMapHeight();
        QString weatherSprite = m_Weathers[m_CurrentWeather]->getWeatherTerrainSprite();
        if (!weatherSprite.isEmpty())
        {
            oxygine::ResAnim* pAnim = GameManager::getInstance()->getResAnim(weatherSprite, oxygine::ep_ignore_error);
            if (pAnim != nullptr)
            {
                m_WeatherSprites.reserve(width * heigth);
                for (qint32 x = 0; x < width; x++)
                {
                    for (qint32 y = 0; y < heigth; y++)
                    {
                        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                        if (pAnim->getTotalFrames() > 1)
                        {
                            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
                            pSprite->addTween(tween);
                        }
                        else
                        {
                            pSprite->setResAnim(pAnim);
                        }
                        pSprite->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(pAnim->getWidth()));
                        pSprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
                        pSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Weather));
                        m_WeatherSprites.push_back(pSprite);
                        m_weatherMaster->addChild(pSprite);
                    }
                }
            }
        }
    }    
}

void GameRules::resetWeatherSprites()
{
    for (auto & sprite : m_WeatherSprites)
    {
        if (sprite.get() != nullptr)
        {
            sprite->detach();
            sprite.reset();
        }
    }
    m_WeatherSprites.clear();
}

void GameRules::resetFogSprites()
{
    for (auto & sprites : m_FogSprites)
    {
        for (auto & sprite : sprites)
        {
            if (sprite.get() != nullptr)
            {
                sprite->detach();
                sprite.reset();
            }
        }
    }
    m_FogSprites.clear();
}

qint32 GameRules::getUnitLimit() const
{
    return m_UnitLimit;
}

void GameRules::setUnitLimit(const qint32 UnitLimit)
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

void GameRules::setFogMode(const GameEnums::Fog FogMode)
{
    m_FogMode = FogMode;
}

void GameRules::createFogVision()
{
    CONSOLE_PRINT("Creating fog vision. Pausing Rendering", GameConsole::eDEBUG);
    Mainapp::getInstance()->pauseRendering();
    QColor fogOfWarColor = QColor(70, 70, 70, 100);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue ret = pInterpreter->doFunction("Global", "getFogOfWarColor");
    if (ret.isString())
    {
        fogOfWarColor = QColor(ret.toString());
    }
    QColor fogOfMistColor = QColor(180, 180, 180, 100);
    ret = pInterpreter->doFunction("Global", "getFogOfMistColor");
    if (ret.isString())
    {
        fogOfMistColor = QColor(ret.toString());
    }
    
    qint32 width = m_pMap->getMapWidth();
    qint32 heigth = m_pMap->getMapHeight();
    if (m_FogSprites.size() == 0)
    {
        m_FogSprites.reserve(width);
        for (qint32 x = 0; x < width; x++)
        {
            m_FogSprites.push_back(std::vector<oxygine::spSprite>(heigth, oxygine::spSprite()));
        }
    }
    // update vision for each player
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->updatePlayerVision(false);
    }
    // get player for which we should create the vision
    Player* pPlayer = m_pMap->getCurrentViewPlayer();
    if (pPlayer != nullptr)
    {
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                switch (m_FogMode)
                {
                    case GameEnums::Fog_Off:
                    {
                        createFieldFogClear(x, y, pPlayer);
                        break;
                    }
                    case GameEnums::Fog_OfWar:
                    {
                        createFieldFogWar(x, y, pPlayer, fogOfWarColor);
                        break;
                    }
                    case GameEnums::Fog_OfShroud:
                    {
                        createFieldFogShrouded(x, y, pPlayer, fogOfMistColor, fogOfWarColor);
                        break;
                    }
                    case GameEnums::Fog_OfMist:
                    {
                        createFieldFogMist(x, y, pPlayer, fogOfMistColor);
                        break;
                    }
                }
            }
        }
    }
    CONSOLE_PRINT("Fog vision created. Continue Rendering", GameConsole::eDEBUG);
    Mainapp::getInstance()->continueRendering();
}

void GameRules::createFieldFogClear(qint32 x, qint32 y, Player* pPlayer)
{
    
    Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
    Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
    if (m_FogSprites[x][y].get() != nullptr)
    {
        m_FogSprites[x][y]->detach();
        m_FogSprites[x][y].reset();
    }
    if (pUnit != nullptr)
    {
        showHideStealthUnit(pPlayer, pUnit);
    }
    if (pBuilding != nullptr)
    {
        pBuilding->updatePlayerColor(true);
    }
}

void GameRules::createFieldFogMist(qint32 x, qint32 y, Player* pPlayer, QColor fogOfMistColor)
{
    
    Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
    Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
    if (m_FogSprites[x][y].get() != nullptr)
    {
        m_FogSprites[x][y]->detach();
        m_FogSprites[x][y].reset();
    }
    if (pUnit != nullptr)
    {
        showHideStealthUnit(pPlayer, pUnit);
    }
    if (pBuilding != nullptr)
    {
        pBuilding->updatePlayerColor(true);
    }
    GameEnums::VisionType visible = pPlayer->getFieldVisibleType(x, y);
    if (visible != GameEnums::VisionType_Clear)
    {
        if (m_FogSprites[x][y].get() == nullptr)
        {
            // create fog of war sprite
            oxygine::spColorRectSprite sprite = MemoryManagement::create<oxygine::ColorRectSprite>();
            sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
            sprite->setColor(fogOfMistColor);
            sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::FogFields));
            sprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
            m_fogMaster->addChild(sprite);
            m_FogSprites[x][y] = sprite;
        }
    }
    else if (m_FogSprites[x][y].get() != nullptr)
    {
        m_FogSprites[x][y]->detach();
        m_FogSprites[x][y].reset();
    }
}

void GameRules::createFieldFogWar(qint32 x, qint32 y, Player* pPlayer, QColor fogOfWarColor)
{
    
    GameEnums::VisionType visible = pPlayer->getFieldVisibleType(x, y);
    Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
    Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
    if (pUnit != nullptr)
    {
        showHideStealthUnit(pPlayer, pUnit);
        if (visible != GameEnums::VisionType_Clear)
        {
            pUnit->setUnitVisible(false, pPlayer);
        }
    }
    if (pBuilding != nullptr)
    {
        pBuilding->updatePlayerColor((visible == GameEnums::VisionType_Clear));
    }
    if (visible != GameEnums::VisionType_Clear)
    {
        if (m_FogSprites[x][y].get() == nullptr)
        {
            // create fog of war sprite
            oxygine::spColorRectSprite sprite = MemoryManagement::create<oxygine::ColorRectSprite>();
            sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
            sprite->setColor(fogOfWarColor);
            sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::FogFields));
            sprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
            m_fogMaster->addChild(sprite);
            m_FogSprites[x][y] = sprite;
        }
    }
    else if (m_FogSprites[x][y].get() != nullptr)
    {
        m_FogSprites[x][y]->detach();
        m_FogSprites[x][y].reset();
    }
}

void GameRules::createFieldFogShrouded(qint32 x, qint32 y, Player* pPlayer, QColor fogOfMistColor, QColor fogOfWarColor)
{
    
    GameEnums::VisionType visible = pPlayer->getFieldVisibleType(x, y);
    Terrain* pTerrain = m_pMap->getTerrain(x, y);
    Unit* pUnit = pTerrain->getUnit();
    Building* pBuilding = pTerrain->getBuilding();
    if (pUnit != nullptr)
    {
        showHideStealthUnit(pPlayer, pUnit);
        if (visible != GameEnums::VisionType_Clear)
        {
            pUnit->setUnitVisible(false, pPlayer);
        }
    }
    if (m_FogSprites[x][y].get() != nullptr)
    {
        m_FogSprites[x][y]->detach();
        m_FogSprites[x][y].reset();
    }
    switch (visible)
    {
        case GameEnums::VisionType_Clear:
        {
            pTerrain->setSpriteVisibility(true);
            if (pBuilding != nullptr)
            {
                pBuilding->setVisible(true);
                pBuilding->updatePlayerColor(true);
            }
            break;
        }
        case GameEnums::VisionType_Fogged:
        {
            pTerrain->setSpriteVisibility(true);
            if (pBuilding != nullptr)
            {
                pBuilding->setVisible(true);
                pBuilding->updatePlayerColor(false);
            }
            // create fog of war sprite
            oxygine::spColorRectSprite sprite = MemoryManagement::create<oxygine::ColorRectSprite>();
            sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
            sprite->setColor(fogOfWarColor);
            sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::FogFields));
            sprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
            m_fogMaster->addChild(sprite);
            m_FogSprites[x][y] = sprite;
            break;
        }
        case GameEnums::VisionType_Shrouded:
        {
            if (pBuilding != nullptr &&
                pBuilding->getBuildingWidth() == 1 &&
                pBuilding->getBuildingHeigth() == 1)
            {
                pBuilding->setVisible(false);
            }
            // create fog of war sprite
            oxygine::spColorRectSprite sprite = MemoryManagement::create<oxygine::ColorRectSprite>();
            sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
            sprite->setColor(0, 0, 0, 255);
            sprite->setPriority(static_cast<qint32>(Terrain::DrawPriority::Shroud));
            pTerrain->addChild(sprite);
            pTerrain->setSpriteVisibility(false);
            m_FogSprites[x][y] = sprite;
            break;
        }
        case GameEnums::VisionType_Mist:
        {
            // create fog of war sprite
            oxygine::spColorRectSprite sprite = MemoryManagement::create<oxygine::ColorRectSprite>();
            sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
            sprite->setColor(fogOfMistColor);
            sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::FogFields));
            sprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
            m_fogMaster->addChild(sprite);
            m_FogSprites[x][y] = sprite;
            break;
        }
    }
}

void GameRules::showHideStealthUnit(Player* pPlayer, Unit* pUnit)
{
    
    switch (pPlayer->checkAlliance(pUnit->getOwner()))
    {
        case GameEnums::Alliance_Enemy:
        {
            if (pUnit->isStealthed(pPlayer))
            {
                pUnit->setUnitVisible(false, pPlayer);
            }
            else
            {
                pUnit->setUnitVisible(true, pPlayer);
            }
            break;
        }
        case GameEnums::Alliance_Friend:
        {
            pUnit->setUnitVisible(true, pPlayer);
            break;
        }
    }
    
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

bool GameRules::getCoUnits() const
{
    return m_coUnits;
}

void GameRules::setCoUnits(bool coUnits)
{
    m_coUnits = coUnits;
}

QString GameRules::getMatchType() const
{
    return m_matchType;
}

void GameRules::setMatchType(const QString newMatchType)
{
    m_matchType = newMatchType;
}

qint32 GameRules::getMaxPerkCount() const
{
    return m_maxPerkCount;
}

void GameRules::setMaxPerkCount(qint32 newMaxPerkCount)
{
    m_maxPerkCount = newMaxPerkCount;
}

QVector<quint64> &GameRules::getObserverList()
{
    return m_observerList;
}

qint32 GameRules::getMultiplayerObserver() const
{
    return m_multiplayerObserver;
}

void GameRules::setMultiplayerObserver(qint32 newMultiplayerObserver)
{
    m_multiplayerObserver = newMultiplayerObserver;
}

GameMap *GameRules::getMap() const
{
    return m_pMap;
}

bool GameRules::getHpDefenseReduction() const
{
    return m_hpDefenseReduction;
}

void GameRules::setHpDefenseReduction(bool newHpDefenseReduction)
{
    m_hpDefenseReduction = newHpDefenseReduction;
}

GameEnums::PowerGainMode GameRules::getPowerGainMode() const
{
    return m_powerGainMode;
}

void GameRules::setPowerGainMode(const GameEnums::PowerGainMode newPowerGainMode)
{
    m_powerGainMode = newPowerGainMode;
}

GameEnums::PowerGainZone GameRules::getPowerGainZone() const
{
    return m_powerGainZone;
}

void GameRules::setPowerGainZone(const GameEnums::PowerGainZone newPowerGainZone)
{
    m_powerGainZone = newPowerGainZone;
}

bool GameRules::getTransporterRefresh() const
{
    return m_transporterRefresh;
}

void GameRules::setTransporterRefresh(bool newTransporterRefresh)
{
    m_transporterRefresh = newTransporterRefresh;
}

bool GameRules::getEnableDayToDayCoAbilities() const
{
    return m_enableDayToDayCoAbilities;
}

void GameRules::setEnableDayToDayCoAbilities(bool newEnableDayToDayCoAbilities)
{
    m_enableDayToDayCoAbilities = newEnableDayToDayCoAbilities;
}

bool GameRules::getShipBridges() const
{
    return m_shipBridges;
}

void GameRules::setShipBridges(bool newShipBridges)
{
    m_shipBridges = newShipBridges;
}

qreal GameRules::getResellValue() const
{
    return m_resellValue;
}

void GameRules::setResellValue(qreal newResellValue)
{
    m_resellValue = newResellValue;
}

qreal GameRules::getPowerUsageReduction() const
{
    return m_powerUsageReduction;
}

void GameRules::setPowerUsageReduction(qreal newPowerUsageReduction)
{
    m_powerUsageReduction = newPowerUsageReduction;
}

qreal GameRules::getPowerLoose() const
{
    return m_powerLoose;
}

void GameRules::setPowerLoose(qreal newPowerLoose)
{
    m_powerLoose = newPowerLoose;
}

bool GameRules::getVictory() const
{
    return m_victory;
}

quint8 GameRules::getTerrainDefense() const
{
    return m_terrainDefense;
}

void GameRules::setTerrainDefense(const quint8 terrainDefense)
{
    m_terrainDefense = terrainDefense;
}

bool GameRules::getCosmeticModsAllowed() const
{
    return m_cosmeticModsAllowed;
}

void GameRules::setCosmeticModsAllowed(bool value)
{
    m_cosmeticModsAllowed = value;
}

bool GameRules::getSingleCo() const
{
    return m_singleCo;
}

void GameRules::setSingleCo(bool singleCo)
{
    m_singleCo = singleCo;
}

QString GameRules::getDescription() const
{
    return m_description;
}

void GameRules::setDescription(const QString description)
{
    m_description = description;
}

const Password & GameRules::getPassword() const
{
    return m_password;
}

QString GameRules::getPasswordText() const
{
    return m_password.getPasswordText();
}

void GameRules::setPassword(const QString password)
{
    m_password.setPassword(password);
}

qreal GameRules::getPowerGainSpeed() const
{
    return m_powerGainSpeed;
}

void GameRules::setPowerGainSpeed(qreal powerGainSpeed)
{
    m_powerGainSpeed = powerGainSpeed;
}

QStringList GameRules::getAllowedActions() const
{
    return m_allowedActions;
}

void GameRules::setAllowedActions(const QStringList allowedActions)
{
    m_allowedActions = allowedActions;
    m_actionBannlistEdited = true;
}

QStringList GameRules::getAllowedPerks() const
{
    return m_allowedPerks;
}

void GameRules::setAllowedPerks(const QStringList allowedPerks)
{
    m_allowedPerks = allowedPerks;
    m_perkBannlistEdited = true;
}

bool GameRules::getTeamFacingUnits() const
{
    return m_teamFacingUnits;
}

void GameRules::setTeamFacingUnits(bool teamFacingUnits)
{
    m_teamFacingUnits = teamFacingUnits;
}

bool GameRules::getSingleRandomCO() const
{
    return m_singleRandomCO;
}

void GameRules::setSingleRandomCO(bool singleRandomCO)
{
    m_singleRandomCO = singleRandomCO;
}

qint32 GameRules::getMaxPerkCost() const
{
    return m_maxPerkCost;
}

void GameRules::setMaxPerkCost(const qint32 maxPerkCost)
{
    m_maxPerkCost = maxPerkCost;
}

GameRules::DayToDayScreen GameRules::getDayToDayScreen() const
{
    return m_DayToDayScreen;
}

void GameRules::setDayToDayScreen(const DayToDayScreen &DayToDayScreen)
{
    m_DayToDayScreen = DayToDayScreen;
}

bool GameRules::getBuildingVisionHide() const
{
    return m_BuildingVisionHide;
}

void GameRules::setBuildingVisionHide(bool buildingVisionHide)
{
    m_BuildingVisionHide = buildingVisionHide;
}

bool GameRules::getVisionBlock() const
{
    return m_VisionBlock;
}

void GameRules::setVisionBlock(bool VisionBlock)
{
    m_VisionBlock = VisionBlock;
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

void GameRules::setCOBannlist(const QStringList COBannlist)
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

void GameRules::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void GameRules::serializeObject(QDataStream& pStream, bool forHash) const
{
    CONSOLE_PRINT("storing game rules with victory rules: " + QString::number(m_VictoryRules.count()) + " current weather " + QString::number(m_CurrentWeather), GameConsole::eDEBUG);
    pStream << getVersion();
    pStream << static_cast<qint32>(m_VictoryRules.size());
    for (auto & rule : m_VictoryRules)
    {
        auto ruleTypes = rule->getRuleType();
        for (qint32 i = 0; i < ruleTypes.size(); ++i)
        {
            if (ruleTypes[i] == VictoryRule::spinbox &&
                rule->getRuleValue(i) <= rule->getInfiniteValue(i))
            {
                rule->setRuleValue(std::numeric_limits<qint32>::min(), i);
            }
        }
        rule->serializeObject(pStream, forHash);
    }

    pStream << static_cast<qint32>(m_Weathers.size());
    for (qint32 i = 0; i < m_Weathers.size(); i++)
    {
        m_Weathers[i]->serializeObject(pStream, forHash);
        pStream << m_WeatherChances[i];
    }
    if (!forHash)
    {
        CONSOLE_PRINT("Saving weather prediction for days " + QString::number(m_WeatherDays.size()), GameConsole::eDEBUG);
        pStream << static_cast<qint32>(m_WeatherDays.size());
        for (qint32 i = 0; i < m_WeatherDays.size(); ++i)
        {
            pStream << static_cast<qint32>(m_WeatherDays[i].size());
            for (qint32 i2 = 0; i2 < m_WeatherDays[i].size(); ++i2)
            {
                pStream << m_WeatherDays[i][i2];
            }
        }
    }
    pStream << m_CurrentWeather;
    pStream << m_StartWeather;
    pStream << m_randomWeather;
    pStream << m_RankingSystem;
    pStream << m_NoPower;
    pStream << m_UnitLimit;
    pStream << static_cast<qint32>(m_FogMode);
    if (!forHash)
    {
        pStream << roundTime;
        pStream << static_cast<qint32>(m_RoundTimer.interval());
    }
    pStream << m_AiAttackTerrain;
    pStream << m_COBannlistEdited;
    pStream << static_cast<qint32>(m_COBannlist.size());
    for (qint32 i = 0; i < m_COBannlist.size(); i++)
    {
        pStream << m_COBannlist[i];
    }
    pStream << m_WeatherPrediction;
    pStream << m_DayWeather;
    pStream << m_coUnits;
    pStream << m_VisionBlock;
    pStream << m_BuildingVisionHide;
    pStream << static_cast<quint8>(m_DayToDayScreen);
    pStream << m_maxPerkCost;
    pStream << m_singleRandomCO;
    pStream << m_teamFacingUnits;
    pStream << m_perkBannlistEdited;
    Filesupport::writeVectorList(pStream, m_allowedPerks);
    pStream << m_actionBannlistEdited;
    Filesupport::writeVectorList(pStream, m_allowedActions);
    pStream << m_powerGainSpeed;

    pStream << static_cast<qint32>(m_GameRules.size());
    for (qint32 i = 0; i < m_GameRules.size(); i++)
    {
        m_GameRules[i]->serializeObject(pStream, forHash);
    }
    if (!forHash)
    {
        pStream << m_description;
        m_password.serializeObject(pStream);
    }
    pStream << m_singleCo;
    pStream << m_cosmeticModsAllowed;
    pStream << m_terrainDefense;
    pStream << m_resellValue;
    pStream << m_transporterRefresh;
    pStream << m_shipBridges;
    pStream << m_enableDayToDayCoAbilities;
    pStream << static_cast<qint32>(m_powerGainZone);
    pStream << static_cast<qint32>(m_powerGainMode);
    pStream << m_powerUsageReduction;
    pStream << m_powerLoose;
    pStream << m_hpDefenseReduction;
    if (!forHash)
    {
        pStream << m_multiplayerObserver;
        pStream << m_maxPerkCount;
        pStream << m_matchType;
        pStream << m_mapPalette;
    }
    pStream << static_cast<qint32>(m_damageFormula);
    pStream << m_parallelCos;
}

void GameRules::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void GameRules::deserializer(QDataStream& pStream, bool)
{
    CONSOLE_PRINT("reading game rules", GameConsole::eDEBUG);
    resetArrays();

    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        m_VictoryRules.append(MemoryManagement::create<VictoryRule>(m_pMap));
        m_VictoryRules[i]->deserializeObject(pStream);
    }
    qint32 ruleItem = 0;
    while (ruleItem < m_VictoryRules.size())
    {
        if (pGameRuleManager->existsVictoryRule(m_VictoryRules[ruleItem]->getRuleID()))
        {
            ruleItem++;
        }
        else
        {
            m_VictoryRules.removeAt(ruleItem);
        }
    }
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        spWeather pWeather = MemoryManagement::create<Weather>(m_pMap);
        pWeather->deserializeObject(pStream);
        qint32 chance = 0;
        pStream >> chance;
        if (pGameRuleManager->existsWeather(pWeather->getWeatherId()))
        {
            if (m_Weathers.size() > 0)
            {
                for (qint32 i2 = 0; i2 < m_Weathers.size(); i2++)
                {

                    if (m_Weathers[i2]->getWeatherId() == pWeather->getWeatherId())
                    {
                        m_Weathers[i2] = pWeather;
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
            else
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
        qint32 startPlayer  = m_pMap->getCurrentPlayer()->getPlayerID();
        qint32 playerCount = m_pMap->getPlayerCount();
        qint32 day = 0;
        // loop while remaining counter >= 0
        while (weatherDuration > 0)
        {
            // add day
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
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    if (version > 3)
    {
        pStream >> m_AiAttackTerrain;
        pStream >> m_COBannlistEdited;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            QString coid;
            pStream >> coid;
            if (pCOSpriteManager->exists(coid) &&
                !m_COBannlist.contains(coid))
            {
                m_COBannlist.append(coid);
            }
        }
    }
    if (version <= 3 || !m_COBannlistEdited)
    {
        for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
        {
            QString coid = pCOSpriteManager->getID(i);
            if (!m_COBannlist.contains(coid))
            {
                m_COBannlist.append(coid);
            }
        }
    }
    if (version > 5)
    {
        pStream >> m_WeatherPrediction;
        pStream >> m_DayWeather;
    }
    if (version > 6)
    {
        pStream >> m_coUnits;
    }
    if (version > 7)
    {
        pStream >> m_VisionBlock;
    }
    if (version > 8)
    {
        pStream >> m_BuildingVisionHide;
    }
    if (version > 9)
    {
        quint8 value = 0;
        pStream >> value;
        m_DayToDayScreen = static_cast<DayToDayScreen>(value);
    }
    if (version > 10)
    {
        pStream >> m_maxPerkCost;
    }
    if (version > 11)
    {
        pStream >> m_singleRandomCO;
    }
    if (version > 12)
    {
        pStream >> m_teamFacingUnits;
    }
    if (version > 13)
    {
        if (version > 16)
        {
            pStream >> m_perkBannlistEdited;
            m_allowedPerks = Filesupport::readVectorList<QString, QList>(pStream);
            if (!m_perkBannlistEdited)
            {
                m_allowedPerks = COPerkManager::getInstance()->getLoadedRessources();
            }
            pStream >> m_actionBannlistEdited;
            m_allowedActions = Filesupport::readVectorList<QString, QList>(pStream);
            if (!m_actionBannlistEdited)
            {
                m_allowedActions = GameManager::getInstance()->getDefaultActionbBannlist();
            }
        }
        else
        {
            m_allowedPerks = Filesupport::readVectorList<QString, QList>(pStream);
            m_allowedActions = Filesupport::readVectorList<QString, QList>(pStream);
        }
        if (version > 26)
        {
            pStream >> m_powerGainSpeed;
        }
        else
        {
            float value = 0;
            pStream >> value;
            m_powerGainSpeed = value;
        }
    }
    else
    {
        m_allowedPerks = COPerkManager::getInstance()->getLoadedRessources();
        m_allowedActions = GameManager::getInstance()->getDefaultActionbBannlist();
    }
    if (version > 14)
    {
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            m_GameRules.append(MemoryManagement::create<GameRule>());
            m_GameRules[i]->deserializeObject(pStream);
        }
    }
    if (version > 15)
    {
        pStream >> m_description;
        m_password.deserializeObject(pStream);
    }
    if (version > 17)
    {
        pStream >> m_singleCo;
    }
    if (version > 18)
    {
        pStream >> m_cosmeticModsAllowed;
    }
    if (version > 19)
    {
        pStream >> m_terrainDefense;
    }
    if (version > 20)
    {
        if (version > 26)
        {
            pStream >> m_resellValue;
        }
        else
        {
            float value = 0;
            pStream >> value;
            m_resellValue = value;
        }
        pStream >> m_transporterRefresh;
        pStream >> m_shipBridges;
        pStream >> m_enableDayToDayCoAbilities;
        qint32 value;
        pStream >> value;
        m_powerGainZone = static_cast<GameEnums::PowerGainZone>(value);
        pStream >> value;
        m_powerGainMode = static_cast<GameEnums::PowerGainMode>(value);
        if (version > 26)
        {
            pStream >> m_powerUsageReduction;
            pStream >> m_powerLoose;
        }
        else
        {
            float value = 0;
            pStream >> value;
            m_powerUsageReduction = value;
            pStream >> value;
            m_powerLoose = value;
        }
    }
    if (version > 21)
    {
        pStream >> m_hpDefenseReduction;
    }
    if (version > 22)
    {
        pStream >> m_multiplayerObserver;
    }
    if (version > 23)
    {
        pStream >> m_maxPerkCount;
    }
    if (version > 24)
    {
        pStream >> m_matchType;
    }
    if (version > 25)
    {
        pStream >> m_mapPalette;
    }
    if (version > 27)
    {
        qint32 value;
        pStream >> value;
        m_damageFormula = static_cast<GameEnums::DamageFormula>(value);
    }
    if (version > 28)
    {
        pStream >> m_parallelCos;
    }
    CONSOLE_PRINT("Weather prediction for days after restoring " + QString::number(m_WeatherDays.size()), GameConsole::eDEBUG);
}
