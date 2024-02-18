#include "awbwReplayReader/awbwreplayplayer.h"
#include "awbwReplayReader/awbwdatatypes.h"

#include "game/gamemap.h"

#include "coreengine/settings.h"
#include "coreengine/mainapp.h"

#include "menue/replaymenu.h"

AwbwReplayPlayer::AwbwReplayPlayer(ReplayMenu * pReplayMenu, GameMap* pMap)
    : m_pMap(pMap),
    m_pReplayMenu(pReplayMenu),
    m_actionParser(*this, pMap),
    m_recordCreator(pMap)
{
    connect(&m_mapDownloader, &AwbwMapDownloader::sigDownloadResult, this, &AwbwReplayPlayer::onDownloadResult, Qt::QueuedConnection);
}

bool AwbwReplayPlayer::loadRecord(const QString & filename)
{
    bool ret = false;
    m_replayReader.parseReplay(filename);
    auto const & states = m_replayReader.getGameStates();
    if (states.size() > 0)
    {
        m_mapDownloader.startMapDownload(states[0].mapId);
        ret = getMods() == Settings::getInstance()->getMods();
    }
    return ret;
}

QStringList AwbwReplayPlayer::getMods()
{
    return {"mods/awds_co", "mods/awds_unit"};
}

spGameAction AwbwReplayPlayer::nextAction()
{
    const auto & actions = m_replayReader.getActions();
    spGameAction action;
    qint32 actionIndex = 0;
    qint32 turnIndex = getCurrentTurnIndex(actionIndex);
    if (turnIndex < actions.size() &&
        actionIndex < actions[turnIndex].actionData.size())
    {
        bool fetchNextAction = true;
        while (fetchNextAction)
        {
            action = m_actionParser.getAction(actions[turnIndex].actionData[actionIndex], fetchNextAction);
            ++m_currentActionPos;
        }
    }
    return action;
}

qint32 AwbwReplayPlayer::getCurrentTurnIndex(qint32 & actionIndex) const
{
    const auto & actions = m_replayReader.getActions();
    actionIndex = m_currentActionPos;
    qint32 turnIndex = 0;
    while (turnIndex < actions.size() &&
           actionIndex >= actions[turnIndex].actionData.size())
    {
        actionIndex -= actions[turnIndex].actionData.size();
        ++turnIndex;
    }
    return turnIndex;
}

void AwbwReplayPlayer::onPostAction()
{
    m_actionParser.onPostAction();
}

qint32 AwbwReplayPlayer::getRecordSize()
{
    qint32 count = 0;
    const auto & actions = m_replayReader.getActions();
    for (const auto & action : actions)
    {
        count += action.actionData.size();
    }
    return count;
}

qint32 AwbwReplayPlayer::getProgess()
{
    return m_currentActionPos;
}

IReplayReader::DayInfo AwbwReplayPlayer::getDayFromPosition(qint32 count)
{
    DayInfo dayInfo;
    const auto & gameStates = m_replayReader.getGameStates();
    const auto & actions = m_replayReader.getActions();
    if (gameStates.size() > 0)
    {
        std::map<qint32, qint32> playerMapping;
        for (const auto & player : gameStates[0].players)
        {
            playerMapping[player.playerId] = player.playerIdx;
        }

        qint32 currentCount = 0;
        qint32 index = 0;
        dayInfo.day = gameStates[0].day;
        dayInfo.player = playerMapping[gameStates[0].turn];
        while (index < actions.size() - 1 &&
               index < gameStates.size() - 1 &&
               currentCount < count)
        {
            currentCount += actions[index].actionData.size();
            ++index;
            dayInfo.day = gameStates[index].day;
            dayInfo.player = playerMapping[gameStates[index].turn];
        }
    }
    return dayInfo;
}

void AwbwReplayPlayer::seekToDay(IReplayReader::DayInfo dayInfo)
{
    m_currentActionPos = 0;

    qint32 gameStateIndex = 0;
    const auto & gameStates = m_replayReader.getGameStates();
    const auto & actions = m_replayReader.getActions();
    std::map<qint32, qint32> playerMapping;
    for (const auto & player : gameStates[0].players)
    {
        playerMapping[player.playerId] = player.playerIdx;
    }
    while (gameStateIndex < actions.size() &&
           gameStateIndex < gameStates.size())
    {
        if (dayInfo.day == gameStates[gameStateIndex].day &&
            dayInfo.player == playerMapping[gameStates[gameStateIndex].turn])
        {
            break;
        }
        else
        {
            m_currentActionPos += actions[gameStateIndex].actionData.size();
            ++gameStateIndex;
        }
    }

    loadMap(true, dayInfo);
}

void AwbwReplayPlayer::requestReplayStart()
{
    if (m_startReplay)
    {
        startReplayInternal();
    }
    else
    {
        m_startReplay = true;
    }
}

void AwbwReplayPlayer::onDownloadResult(bool success)
{
    if (m_startReplay && success)
    {
        startReplayInternal();
    }
    else
    {
        m_startReplay = true;
    }
}

void AwbwReplayPlayer::startReplayInternal()
{
    IReplayReader::DayInfo dayInfo;
    dayInfo.day = 1;
    loadMap(true, dayInfo);
    m_pMap->updateSprites();
    emit startReplay();
}

void AwbwReplayPlayer::loadMap(bool withOutUnits, IReplayReader::DayInfo dayInfo)
{
    Mainapp::getInstance()->pauseRendering();
    qint32 gameStateIndex = 0;

    const auto & gameStates = m_replayReader.getGameStates();
    const auto & actions = m_replayReader.getActions();
    std::map<qint32, qint32> playerMapping;
    for (const auto & player : gameStates[0].players)
    {
        playerMapping[player.playerId] = player.playerIdx;
    }
    while (gameStateIndex < actions.size() &&
           gameStateIndex < gameStates.size())
    {
        if (dayInfo.day == gameStates[gameStateIndex].day &&
            dayInfo.player == playerMapping[gameStates[gameStateIndex].turn])
        {
            break;
        }
        else
        {
            ++gameStateIndex;
        }
    }
    m_pMap->setIsHumanMatch(false);
    m_mapDownloader.loadMap(m_pMap, withOutUnits, false);
    auto const & states = m_replayReader.getGameStates();
    if (gameStateIndex < states.size())
    {
        loadBuildings(states, gameStateIndex);
    }
    m_pMap->optimizePlayers();    
    if (gameStateIndex < states.size())
    {
        loadPlayers(states, gameStateIndex);
        loadUnits(states, gameStateIndex);
        updateCapturePoints(states, gameStateIndex);        
    }
    // swap out all ai's / or players with a proxy ai.
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes::AiTypes_ProxyAi));        
        if (gameStateIndex < states.size())
        {
            m_pMap->getPlayer(i)->setFundsModifier(static_cast<float>(gameStates[gameStateIndex].fundsPerBuilding) / 1000.0f);
            m_pMap->getPlayer(i)->setFunds(gameStates[gameStateIndex].players[i].funds);
            // m_pMap->getPlayer(i)->setMenu(m_);
        }
    }
    m_pMap->setCurrentPlayer(dayInfo.player);
    m_pMap->setIsHumanMatch(false);
    if (gameStateIndex < states.size())
    {
        loadGameRules(states, gameStateIndex);
        m_pMap->setCurrentDay(dayInfo.day);
        qint32 playerIdx = 0;
        for (qint32 i = 0; i < gameStates[gameStateIndex].players.size(); ++i)
        {
            if (actions[gameStateIndex].playerId == gameStates[gameStateIndex].players[i].playerId)
            {
                playerIdx = i;
                break;
            }
        }
        m_actionParser.setCurrentPlayerData(actions[gameStateIndex].playerId, gameStates[gameStateIndex].players[playerIdx].team);
        m_recordCreator.loadRecordData(m_replayReader, gameStateIndex);
    }
    Mainapp::getInstance()->continueRendering();
}

void AwbwReplayPlayer::loadGameRules(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex)
{
    GameRules* pRule = m_pMap->getGameRules();
    if (gameStates[gameStateIndex].fog)
    {
        pRule->setFogMode(GameEnums::Fog_OfWar);
    }
    else
    {
        pRule->setFogMode(GameEnums::Fog_Off);
    }
    if (gameStates[gameStateIndex].currentWeather.toLower() == "c")
    {
        pRule->changeWeather("WEATHER_1SUN", m_pMap->getPlayerCount());
    }
    else if (gameStates[gameStateIndex].currentWeather.toLower() == "r")
    {
        pRule->changeWeather("WEATHER_RAIN", m_pMap->getPlayerCount());
    }
    else if (gameStates[gameStateIndex].currentWeather.toLower() == "s")
    {
        pRule->changeWeather("WEATHER_SNOW", m_pMap->getPlayerCount());
    }
    pRule->setEnableDayToDayCoAbilities(gameStates[gameStateIndex].usePowers);
    pRule->setRandomWeather(false);
    pRule->setVictory(false);
    pRule->setMoveVision(false);
    pRule->setRankingSystem(false);
    pRule->setParallelCos(false);
    pRule->setDamageFormula(GameEnums::DamageFormula_AdvanceWars1_3);
    pRule->setShipBridges(false);
    pRule->setTransporterRefresh(false);
    pRule->setSpecialDestruction(false);
}

ReplayMenu * AwbwReplayPlayer::getReplayMenu() const
{
    return m_pReplayMenu;
}

const AwbwReplayerReader & AwbwReplayPlayer::getReplayReader() const
{
    return m_replayReader;
}

qint32 AwbwReplayPlayer::getCurrentActionPos() const
{
    return m_currentActionPos;
}

void AwbwReplayPlayer::loadBuildings(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex)
{
    for (const auto & building : gameStates[gameStateIndex].buildings)
    {
        loadBuilding(building);
    }
}

void AwbwReplayPlayer::loadBuilding(const AwbwReplayerReader::BuildingInfo & building)
{
    const auto & buildingData = AwbwDataTypes::TERRAIN_BUILDING_DATA[building.terrainId];
    spBuilding pBuilding = MemoryManagement::create<Building>(buildingData.id, m_pMap);
    if (buildingData.owner == AwbwDataTypes::NEUTRAL_OWNER)
    {
        pBuilding->setOwner(nullptr);
    }
    else
    {


        pBuilding->setOwner(m_pMap->getPlayer(buildingData.owner));
    }
    m_pMap->getTerrain(building.x, building.y)->setBuilding(pBuilding);
}

void AwbwReplayPlayer::loadUnits(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex)
{
    for (const auto & unit : gameStates[gameStateIndex].units)
    {
        qint32 playerIdx = 0;
        for (qint32 player = 0; player < gameStates[gameStateIndex].players.size(); ++player)
        {
            if (gameStates[gameStateIndex].players[player].playerId == unit.playerId)
            {
                playerIdx = gameStates[gameStateIndex].players[player].playerIdx;
                break;
            }
        }
        loadUnit(unit, playerIdx, gameStates[gameStateIndex].units);
    }
}

void AwbwReplayPlayer::loadUnit(const AwbwReplayerReader::UnitInfo & unit, qint32 player, const QVector<AwbwReplayerReader::UnitInfo> & units)
{
    if (!unit.carried)
    {
        m_pMap->spawnUnit(unit.x, unit.y, AwbwDataTypes::UNIT_ID_ID_MAP[unit.name], m_pMap->getPlayer(player), 0, true);
        auto* pUnit = m_pMap->getTerrain(unit.x, unit.y)->getUnit();
        pUnit->setFuel(unit.fuel);
        pUnit->setAmmo1(unit.ammo);
        pUnit->setHp(unit.hp);
        pUnit->setHidden(unit.stealthed.toLower() == "y");
        if (unit.loadedUnitId1 != 0)
        {
            loadLoadedUnit(unit.loadedUnitId1, pUnit, units);
        }
        if (unit.loadedUnitId2 != 0)
        {
            loadLoadedUnit(unit.loadedUnitId1, pUnit, units);
        }
    }
}

void AwbwReplayPlayer::loadLoadedUnit(qint32 unitId, Unit* pTransporter, const QVector<AwbwReplayerReader::UnitInfo> & units)
{
    for (const auto & unit : units)
    {
        if (unit.unitId == unitId)
        {
            auto* pUnit = pTransporter->spawnUnit(AwbwDataTypes::UNIT_ID_ID_MAP[unit.name]);
            pUnit->setFuel(unit.fuel);
            pUnit->setAmmo1(unit.ammo);
            pUnit->setHp(unit.hp);
            pUnit->setHasMoved(true);
            break;
        }
    }
}

void AwbwReplayPlayer::updateCapturePoints(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex)
{
    for (const auto & building : gameStates[gameStateIndex].buildings)
    {
        if (building.capture < Unit::MAX_CAPTURE_POINTS)
        {
            auto* pUnit = m_pMap->getTerrain(building.x, building.y)->getUnit();
            if (pUnit != nullptr)
            {
                pUnit->setCapturePoints(Unit::MAX_CAPTURE_POINTS - building.capture);
            }
        }
    }
}

void AwbwReplayPlayer::loadPlayers(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 gameStateIndex)
{
    for (const auto & player : gameStates[gameStateIndex].players)
    {
        loadPlayer(player);
    }
}

void AwbwReplayPlayer::loadPlayer(const AwbwReplayerReader::PlayerInfo & player)
{
    auto* pPlayer = m_pMap->getPlayer(player.playerIdx);
    pPlayer->setFunds(player.funds);
    pPlayer->setMenu(m_pReplayMenu);
    pPlayer->setIsDefeated(player.eliminated);
    pPlayer->setPlayerNameId(QString(tr("Player ")) + QString::number(player.playerIdx + 1));
    loadCo(player.coData, pPlayer, 0);
    loadCo(player.tagCoData, pPlayer, 0);
    auto* pCo0 = pPlayer->getCO(0);
    if (pCo0 != nullptr)
    {
        pCo0->setMenu(m_pReplayMenu);
        if (player.powerOn.toLower() == "n")
        {
            pCo0->setPowerMode(GameEnums::PowerMode_Off);
        }
        else if (player.powerOn.toLower() == "y")
        {
            pCo0->setPowerMode(GameEnums::PowerMode_Power);
        }
        else if (player.powerOn.toLower() == "s")
        {
            pCo0->setPowerMode(GameEnums::PowerMode_Superpower);
        }
    }
    pCo0 = pPlayer->getCO(1);
    if (pCo0 != nullptr)
    {
        pCo0->setMenu(m_pReplayMenu);
    }
}

void AwbwReplayPlayer::loadCo(const AwbwReplayerReader::CoInfo & coInfo, Player* pPlayer, qint32 coIdx)
{
    if (!AwbwDataTypes::CO_MAP[coInfo.coId].isEmpty())
    {
        pPlayer->setCO(AwbwDataTypes::CO_MAP[coInfo.coId], coIdx);
        auto* pCO = pPlayer->getCO(coIdx);
        auto powerStars = pCO->getPowerStars();
        if (powerStars > 0)
        {
            pCO->setPowerFilled(static_cast<double>(coInfo.power) / (static_cast<double>(coInfo.maxPower) / static_cast<double>(powerStars)));
        }
    }
}
