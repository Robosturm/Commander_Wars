#include "awbwReplayReader/awbwreplayplayer.h"
#include "awbwReplayReader/awbwdatatypes.h"

#include "game/gamemap.h"

#include "coreengine/settings.h"

AwbwReplayPlayer::AwbwReplayPlayer(GameMap* pMap)
    : m_pMap(pMap)
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
        ret = getMods() == Settings::getInstance()->getMods();;
    }
    return ret;
}

QStringList AwbwReplayPlayer::getMods()
{
    return {"mods/awds_co", "mods/awds_unit"};
}

spGameAction AwbwReplayPlayer::nextAction()
{
    ++m_currentActionPos;
    // todo
    return spGameAction();
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

qint32 AwbwReplayPlayer::getDayFromPosition(qint32 count)
{
    qint32 day = 0;
    qint32 currentCount = 0;
    const auto & actions = m_replayReader.getActions();
    while (day < actions.size() && currentCount < count)
    {
        currentCount += actions[day].actionData.size();
        ++day;
    }
    return day;
}

void AwbwReplayPlayer::seekToDay(qint32 day)
{
    const auto & actions = m_replayReader.getActions();
    m_currentActionPos = 0;
    for (qint32 i = 0; i < day; ++i)
    {
        m_currentActionPos += actions[i].actionData.size();
    }
    loadMap(true, day);
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
    loadMap(false, 0);
    emit startReplay();
}

void AwbwReplayPlayer::loadMap(bool withOutUnits, qint32 day)
{
    m_pMap->setIsHumanMatch(false);
    m_mapDownloader.loadMap(m_pMap, withOutUnits, false);
    auto const & states = m_replayReader.getGameStates();
    if (day < states.size())
    {
        loadBuildings(states, day);
    }
    m_pMap->optimizePlayers();
    if (day < states.size())
    {
        loadUnits(states, day);

    }
    // swap out all ai's / or players with a proxy ai.
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes::AiTypes_ProxyAi));
    }
    m_pMap->setCurrentPlayer(0);
}

void AwbwReplayPlayer::loadBuildings(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 day)
{
    for (const auto & building : gameStates[day].buildings)
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

void AwbwReplayPlayer::loadUnits(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 day)
{
    for (const auto & unit : gameStates[day].units)
    {
        qint32 playerIdx = 0;
        for (qint32 player = 0; player < gameStates[day].players.size(); ++player)
        {
            if (gameStates[day].players[player].playerId == unit.playerId)
            {
                playerIdx = gameStates[day].players[player].playerIdx;
                break;
            }
        }
        loadUnit(unit, playerIdx);
    }
}

void AwbwReplayPlayer::loadUnit(const AwbwReplayerReader::UnitInfo & unit, qint32 player)
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

        }
        if (unit.loadedUnitId2 != 0)
        {

        }
    }
}

void AwbwReplayPlayer::updateCapturePoints(const QVector<AwbwReplayerReader::GameState> & gameStates, qint32 day)
{
    for (const auto & building : gameStates[day].buildings)
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
