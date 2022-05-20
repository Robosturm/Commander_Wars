#include <QFile>
#include <QGuiApplication>

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"
#include "coreengine/globalutils.h"
#include "coreengine/userdata.h"

#include "ai/coreai.h"

#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/movementtablemanager.h"
#include "resource_management/weaponmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/gamerulemanager.h"

#include "game/unit.h"
#include "game/player.h"
#include "game/co.h"
#include "game/gameanimation/gameanimationfactory.h"
#include "game/gameanimation/gameanimationnextday.h"
#include "game/building.h"
#include "game/gamemap.h"
#include "game/terrainfindingsystem.h"

#include "gameinput/humanplayerinput.h"

#include "menue/gamemenue.h"
#include "menue/basegamemenu.h"
#include "menue/movementplanner.h"

#include "objects/loadingscreen.h"

const QString GameMap::m_GameAnimationFactory = "GameAnimationFactory";
const qint32 GameMap::frameTime = 100;
const char* const GameMap::PLAINS = "PLAINS";
static constexpr qint32 loadingScreenSize = 900;
qint32 GameMap::m_imagesize = GameMap::defaultImageSize;

qint32 GameMap::getFrameTime()
{
    return frameTime;
}

GameMap::GameMap(qint32 width, qint32 heigth, qint32 playerCount)
    : m_CurrentPlayer(nullptr),
      m_Rules(spGameRules::create(this))
{
    setObjectName("GameMap");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    m_headerInfo.m_mapAuthor = Settings::getUsername();
    loadMapData();
    newMap(width, heigth, playerCount);
    m_loaded = true;
}

GameMap::GameMap(QDataStream& stream, bool savegame)
    : m_CurrentPlayer(nullptr),
      m_Rules(spGameRules::create(this)),
      m_savegame(savegame)
{
    setObjectName("GameMap");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    loadMapData();
    GameMap::deserializeObject(stream);
    m_loaded = true;
}

GameMap::GameMap(QString map, bool onlyLoad, bool fast, bool savegame)
    : m_CurrentPlayer(nullptr),
      m_Rules(spGameRules::create(this)),
      m_savegame(savegame)
{
    setObjectName("GameMap");
    CONSOLE_PRINT("Loading map: " + map, Console::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    loadMapData();
    loadMap(map, onlyLoad, fast, savegame);
}

void GameMap::loadMap(QString map, bool onlyLoad, bool fast, bool savegame)
{
    clearMap();
    m_savegame = savegame;
    QFile file(map);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream pStream(&file);
        deserializer(pStream, fast);
        setMapNameFromFilename(map);
        m_loaded = true;
        if (!onlyLoad)
        {
            updateSprites();
            qint32 heigth = getMapHeight();
            qint32 width = getMapWidth();
            centerMap(width / 2, heigth / 2);
        }
    }
}

void GameMap::setMapNameFromFilename(QString filename)
{
    if (m_headerInfo.m_mapName.isEmpty())
    {
        QStringList items = filename.split("/");
        if (items.size() > 0)
        {
            items = items[items.size() - 1].split(".");
            if (items.size() > 0)
            {
                m_headerInfo.m_mapName = items[0];
            }
        }
    }
}

void GameMap::loadMapData()
{
    Interpreter::setCppOwnerShip(this);
    registerMapAtInterpreter();
    if (Mainapp::getInstance()->devicePixelRatio() < 2.0f && !Settings::getUseHighDpi())
    {
        setZoom(1);
    }
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Map));

     m_markedFieldsLayer = oxygine::spActor::create();
     m_markedFieldsLayer->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
     addChild(m_markedFieldsLayer);
     m_moveArrowLayer = oxygine::spActor::create();
     m_moveArrowLayer->setPriority(static_cast<qint32>(Mainapp::ZOrder::MoveArrow));
     addChild(m_moveArrowLayer);
     m_unitsLayer = oxygine::spActor::create();
     m_unitsLayer->setPriority(static_cast<qint32>(Mainapp::ZOrder::Units));
     addChild(m_unitsLayer);

    connect(this, &GameMap::sigZoomChanged, this, &GameMap::zoomChanged, Qt::QueuedConnection);
}

void GameMap::setIsHumanMatch(bool newIsHumanMatch)
{
    m_isHumanMatch = newIsHumanMatch;
}

bool GameMap::getIsHumanMatch() const
{
    return m_isHumanMatch;
}

void GameMap::registerMapAtInterpreter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(m_GameAnimationFactory, pInterpreter->newQObject(GameAnimationFactory::getInstance()));
}

qint32 GameMap::getUniqueIdCounter()
{
    m_headerInfo.m_uniqueIdCounter++;
    // gurantee that the counter is never 0
    if (m_headerInfo.m_uniqueIdCounter == 0)
    {
        m_headerInfo.m_uniqueIdCounter++;
    }
    return m_headerInfo.m_uniqueIdCounter;
}

bool GameMap::isInArea(const QRect& area, std::function<bool (Unit* pUnit)> checkFunction)
{
    for (qint32 x = area.x(); x < area.x() + area.width(); x++)
    {
        for (qint32 y = area.y(); y < area.y() + area.height(); y++)
        {
            if (onMap(x, y))
            {
                Unit* pUnit = getTerrain(x, y)->getUnit();
                if (pUnit != nullptr &&
                    checkFunction(pUnit))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameMap::anyPlayerAlive()
{
    qint32 count = 0;
    for (const auto & player : qAsConst(m_players))
    {
        if (!player->getIsDefeated())
        {
            ++count;
        }
        if (count >= 2)
        {
            return true;
        }
    }
    return false;
}

QmlVectorPoint* GameMap::getVisionCircle(qint32 x, qint32 y, qint32 minVisionRange, qint32 maxVisionRange, qint32 visionHigh)
{

    QmlVectorPoint* pRet = new QmlVectorPoint();
    if (maxVisionRange > 0)
    {
        if (visionHigh < 0)
        {
            visionHigh = 0;
        }
        std::vector<QRect> m_LineSight;
        std::vector<QRect> m_LineSightEvaluated;
        m_LineSight.push_back(QRect(x - 1, y, 0, 2));
        m_LineSight.push_back(QRect(x - 1, y, 0, 3));
        m_LineSight.push_back(QRect(x + 1, y, 1, 2));
        m_LineSight.push_back(QRect(x + 1, y, 1, 3));
        m_LineSight.push_back(QRect(x, y - 1, 2, 0));
        m_LineSight.push_back(QRect(x, y - 1, 2, 1));
        m_LineSight.push_back(QRect(x, y + 1, 3, 0));
        m_LineSight.push_back(QRect(x, y + 1, 3, 1));

        QPoint pos(x, y);
        if (0 >= minVisionRange && 0 <= maxVisionRange)
        {
            pRet->append(QPoint(0, 0));
        }
        while (m_LineSight.size() > 0)
        {
            QRect current = m_LineSight.front();
            m_LineSight.erase(m_LineSight.cbegin());
            m_LineSightEvaluated.push_back(current);
            if (onMap(current.x(), current.y()))
            {
                qint32 distance = GlobalUtils::getDistance(QPoint(current.x(), current.y()), pos);
                if (distance >= minVisionRange && distance <= maxVisionRange)
                {
                    QPoint nextPos(current.x() - x, current.y() - y);
                    if (!pRet->contains(nextPos))
                    {
                        pRet->append(nextPos);
                    }
                }
                Terrain* pTerrain = getTerrain(current.x(), current.y());
                qint32 currentHeigth = pTerrain->getTotalVisionHigh();
                // we can see over the terrain continue vision range
                if (currentHeigth <= visionHigh && distance + 1 <= maxVisionRange)
                {
                    for (qint32 i = 0; i < 4; i++)
                    {
                        qint32 nextX;
                        qint32 nextY;
                        if (i != current.width() && i != current.height())
                        {
                            switch (i)
                            {
                                case 0:
                                {
                                    nextX = current.x() + 1;
                                    nextY = current.y();
                                    break;
                                }
                                case 1:
                                {
                                    nextX = current.x() - 1;
                                    nextY = current.y();
                                    break;
                                }
                                case 2:
                                {
                                    nextX = current.x();
                                    nextY = current.y() + 1;
                                    break;
                                }
                                case 3:
                                {
                                    nextX = current.x();
                                    nextY = current.y() - 1;
                                    break;
                                }
                            }
                            // not evaluated yet
                            if (onMap(nextX, nextY))
                            {
                                QRect next(nextX, nextY, current.width(), current.height());
                                bool notIncluded = true;
                                for (const auto & item : m_LineSightEvaluated)
                                {
                                    if (item.x() == nextX && item.y() == nextY)
                                    {
                                        notIncluded = false;
                                        break;
                                    }
                                }
                                if (notIncluded)
                                {
                                    m_LineSight.push_back(next);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return pRet;
}

bool GameMap::isUnitInArea(const QRect& area, qint32 unitID)
{
    return isInArea(area, [=](Unit* pUnit)
    {
        return (pUnit->getUniqueID() == unitID);
    });
}

bool GameMap::isPlayerUnitInArea(const QRect& area, qint32 playerID)
{
    return isInArea(area, [=](Unit* pUnit)
    {
        return (pUnit->getOwner()->getPlayerID() == playerID);
    });
}

bool GameMap::isPlayersUnitInArea(const QRect& area, QList<qint32> & playerIDs)
{
    return isInArea(area, [=](Unit* pUnit)
    {
        qint32 owner = pUnit->getOwner()->getPlayerID();
        return playerIDs.contains(owner);
    });
}

GameMap::~GameMap()
{
    CONSOLE_PRINT("deleting map.", Console::eDEBUG);
    // clean up session
    for (qint32 y = 0; y < m_fields.size(); ++y)
    {
        m_rowSprites[y]->detach();
        for (qint32 x = 0; x < m_fields[y].size(); ++x)
        {
            m_fields[y].at(x)->detach();
        }
        m_fields[y].clear();
    }
    m_rowSprites.clear();
    m_fields.clear();
}

QStringList GameMap::getAllUnitIDs()
{
    return UnitSpriteManager::getInstance()->getLoadedRessources();
}

spGameAction GameMap::createAction()
{
    return spGameAction::create(this);
}

void GameMap::queueAction(spGameAction pAction)
{
    emit sigQueueAction(pAction);
}

spTerrain GameMap::getSpTerrain(qint32 x, qint32 y)
{
    if (onMap(x, y))
    {
        return m_fields[y][x];
    }
    else
    {
        return spTerrain();
    }
}

Terrain* GameMap::getTerrain(qint32 x, qint32 y)
{
    if (onMap(x, y))
    {
        return m_fields[y][x].get();
    }
    else
    {
        return nullptr;
    }
}

QString GameMap::getMapTagsText()
{
    QString ret = tr("Tags\n\n");
    if (GameEnums::MapFilterFlags_Bases & m_headerInfo.m_mapFlags)
    {
        ret += tr("Bases\n");
    }
    if (GameEnums::MapFilterFlags_Airport & m_headerInfo.m_mapFlags)
    {
        ret += tr("Airports\n");
    }
    if (GameEnums::MapFilterFlags_Harbour & m_headerInfo.m_mapFlags)
    {
        ret += tr("Harbours\n");
    }
    if (GameEnums::MapFilterFlags_Tower & m_headerInfo.m_mapFlags)
    {
        ret += tr("Towers\n");
    }
    if (GameEnums::MapFilterFlags_Teleport & m_headerInfo.m_mapFlags)
    {
        ret += tr("Teleport\n");
    }
    if (GameEnums::MapFilterFlags_MixedBase & m_headerInfo.m_mapFlags)
    {
        ret += tr("Mixed bases\n");
    }
    if (GameEnums::MapFilterFlags_Predeployed & m_headerInfo.m_mapFlags)
    {
        ret += tr("Predeployed\n");
    }
    if (GameEnums::MapFilterFlags_Symetric & m_headerInfo.m_mapFlags)
    {
        ret += tr("Symetric\n");
    }
    if (GameEnums::MapFilterFlags_PvP & m_headerInfo.m_mapFlags)
    {
        ret += tr("PvP\n");
    }
    if (GameEnums::MapFilterFlags_FTA & m_headerInfo.m_mapFlags)
    {
        ret += tr("FTA\n");
    }
    if (GameEnums::MapFilterFlags_VsAi & m_headerInfo.m_mapFlags)
    {
        ret += tr("Vs AI\n");
    }
    if (GameEnums::MapFilterFlags_TeamPlay & m_headerInfo.m_mapFlags)
    {
        ret += tr("Team play\n");
    }
    if (GameEnums::MapFilterFlags_Historical & m_headerInfo.m_mapFlags)
    {
        ret += tr("Historical\n");
    }
    if (GameEnums::MapFilterFlags_LowFunds & m_headerInfo.m_mapFlags)
    {
        ret += tr("Low funds\n");
    }
    if (GameEnums::MapFilterFlags_HighFunds & m_headerInfo.m_mapFlags)
    {
        ret += tr("High funds\n");
    }
    if (GameEnums::MapFilterFlags_Naval & m_headerInfo.m_mapFlags)
    {
        ret += tr("Naval\n");
    }
    if (GameEnums::MapFilterFlags_Ground & m_headerInfo.m_mapFlags)
    {
        ret += tr("Ground\n");
    }
    if (GameEnums::MapFilterFlags_Air & m_headerInfo.m_mapFlags)
    {
        ret += tr("Air\n");
    }
    if (GameEnums::MapFilterFlags_Scripted & m_headerInfo.m_mapFlags)
    {
        ret += tr("Scripted\n");
    }
    return ret;
}

spPlayer GameMap::getspPlayer(qint32 player)
{
    if (player >= 0 && player < m_players.size())
    {
        return m_players[player];
    }
    else
    {
        return spPlayer();
    }
}

Player* GameMap::getPlayer(qint32 player)
{
    if (player >= 0 && player < m_players.size())
    {
        return m_players[player].get();
    }
    else
    {
        return nullptr;
    }
}

Player* GameMap::getCurrentPlayer()
{
    return m_CurrentPlayer.get();
}

spPlayer GameMap::getSpCurrentPlayer()
{
    return m_CurrentPlayer;
}

void GameMap::setCurrentPlayer(qint32 player)
{
    if ((player >= 0) && (player < m_players.size()))
    {
        m_CurrentPlayer = m_players[player];
    }
}

void GameMap::onWeatherChanged(Weather* pWeather)
{
    CONSOLE_PRINT("GameMap::onWeatherChanged()", Console::eDEBUG);
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            if (m_fields[y][x]->getBuilding() != nullptr)
            {
                m_fields[y][x]->getBuilding()->onWeatherChanged(pWeather);
            }
        }
    }
}

void GameMap::updateSprites(qint32 xInput, qint32 yInput, bool editor, bool showLoadingScreen)
{
    CONSOLE_PRINT("Update Sprites x=" + QString::number(xInput) + " y=" + QString::number(yInput), Console::eDEBUG);
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    if (showLoadingScreen)
    {
        pLoadingScreen->show();
        pLoadingScreen->setProgress("Loading Map Sprites", 0);
    }
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    setWidth(width * GameMap::getImageSize());
    setHeight(heigth * GameMap::getImageSize());

    QVector<QPoint> flowPoints;
    if ((xInput < 0) && (yInput < 0))
    {
        // update terrain sprites
        for (qint32 y = 0; y < heigth; y++)
        {
            if (showLoadingScreen)
            {
                pLoadingScreen->setProgress(tr("Loading Map Row ") + QString::number(y) + tr(" of ") + QString::number(heigth), 0 + 50 * y / heigth);
            }
            for (qint32 x = 0; x < width; x++)
            {
                updateTileSprites(x, y, flowPoints, editor);
            }
        }
    }
    else
    {
        // more optimized for a single terrain :)
        for (qint32 y = yInput -3; y <= yInput + 3; y++)
        {
            for (qint32 x = xInput -3; x <= xInput + 3; x++)
            {
                if (onMap(x, y))
                {
                    updateTileSprites(x, y, flowPoints, editor);
                }
            }
        }
    }
    updateFlowTiles(flowPoints);
    syncTerrainAnimations(showLoadingScreen);
    finishUpdateSprites(showLoadingScreen);
}

void GameMap::updateSpritesOfTiles(const QVector<QPoint> & points, bool editor, bool showLoadingScreen)
{
    QVector<QPoint> flowPoints;
    QVector<QPoint> updatedPoints;
    for (const auto & point : points)
    {
        for (qint32 y = point.y() -3; y <= point.y() + 3; y++)
        {
            for (qint32 x = point.x() -3; x <= point.x() + 3; x++)
            {
                QPoint pos(x, y);
                if (!updatedPoints.contains(pos))
                {
                    updatedPoints.append(pos);
                    if (onMap(x, y))
                    {
                        updateTileSprites(x, y, flowPoints, editor);
                    }
                }
            }
        }
    }
    updateFlowTiles(flowPoints);
    syncTerrainAnimations(showLoadingScreen);
    finishUpdateSprites(showLoadingScreen);
}

void GameMap::updateTileSprites(qint32 x, qint32 y, QVector<QPoint> & flowPoints, bool editor)
{
    if (m_fields[y][x]->getHasFlowDirection() &&
        !m_fields[y][x]->getFixedSprite())
    {
        flowPoints.append(QPoint(x, y));
    }
    else
    {
        m_fields[y][x]->loadSprites();
    }
    if (m_fields[y][x]->getUnit() != nullptr)
    {
        m_fields[y][x]->getUnit()->updateSprites(editor);
    }
    if (m_fields[y][x]->getBuilding() != nullptr)
    {
        m_fields[y][x]->getBuilding()->updateBuildingSprites(false);
    }
}

void GameMap::updateFlowTiles(QVector<QPoint> & flowPoints)
{
    while (flowPoints.size() > 0)
    {
        QPoint pos = flowPoints[0];
        spTerrainFindingSystem pPfs = spTerrainFindingSystem::create(this, m_fields[pos.y()][pos.x()]->getFlowTiles(), pos.x(), pos.y());
        pPfs->explore();
        m_fields[pos.y()][pos.x()]->updateFlowSprites(pPfs.get());
        auto points = pPfs->getAllNodePointsFast();
        for (const auto & point : qAsConst(points))
        {
            flowPoints.removeAll(point);
        }
    }
}

void GameMap::finishUpdateSprites(bool showLoadingScreen)
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    if (showLoadingScreen)
    {
        pLoadingScreen->setProgress(tr("Loading weather for snowy times"), 95);
    }
    if (m_Rules.get() != nullptr)
    {
        m_Rules->createWeatherSprites();
    }
    BaseGamemenu* pMenu = BaseGamemenu::getInstance();
    if (pMenu != nullptr)
    {
        pMenu->updateSlidingActorSize();
    }
    if (showLoadingScreen)
    {
        pLoadingScreen->hide();
    }
}

void GameMap::syncTerrainAnimations(bool showLoadingScreen)
{
    CONSOLE_PRINT("synchronizing animations", Console::eDEBUG);
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    auto timeMs = oxygine::Stage::getStage()->getClock()->getTime();
    for (qint32 y = 0; y < heigth; y++)
    {
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Synchronizing Map Row ") + QString::number(y) + tr(" of ") + QString::number(heigth), 50 + 40 * y / heigth);
        }
        for (qint32 x = 0; x < width; x++)
        {
            spTerrain pTerrain = m_fields[y][x];
            pTerrain->syncAnimation(timeMs);
            pTerrain->detach();
            m_rowSprites[y]->addChild(pTerrain);
        }
    }
}

void GameMap::syncUnitsAndBuildingAnimations()
{
    CONSOLE_PRINT("Synchronizing units and building animations", Console::eDEBUG);
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    auto timeMs = oxygine::Stage::getStage()->getClock()->getTime();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            auto * pBuilding = m_fields[y][x]->getBuilding();
            if (pBuilding != nullptr)
            {
                pBuilding->syncAnimation(timeMs);
            }
            auto * pUnit = m_fields[y][x]->getUnit();
            if (pUnit != nullptr)
            {
                pUnit->syncAnimation(timeMs);
            }
        }
    }
}

void GameMap::killDeadUnits()
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            Unit* pUnit = m_fields[y][x]->getUnit();
            if (pUnit != nullptr &&
                pUnit->getHp() <= 0)
            {
                m_fields[y][x]->setUnit(spUnit());
            }
        }
    }
}

void GameMap::addScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs, qint32 shakePauseMs)
{
    oxygine::spTween tween = oxygine::createTween(TweenScreenshake(startIntensity, decay / Settings::getAnimationSpeed(), oxygine::timeMS(shakePauseMs)),
                                                  oxygine::timeMS(static_cast<qint64>(durationMs / Settings::getAnimationSpeed())), 1, false, oxygine::timeMS(static_cast<qint64>(delayMs / Settings::getAnimationSpeed())));
    oxygine::Stage::getStage()->addTween(tween);
}

bool GameMap::getSavegame() const
{
    return m_savegame;
}

QString GameMap::getMapMusic() const
{
    return m_mapMusic;
}

void GameMap::clearMapMusic()
{
    setMapMusic("");
}

void GameMap::setMapMusic(const QString &mapMusic, qint32 startLoopMs, qint32 endLoopMs)
{
    m_mapMusic = mapMusic;
    m_startLoopMs = startLoopMs;
    m_endLoopMs = endLoopMs;
    playMusic();
}

QString GameMap::getMapPath() const
{
    return m_mapPath;
}

void GameMap::setMapPath(const QString &mapPath)
{
    m_mapPath = mapPath;
}

void GameMap::setImagesize(const qint32 &imagesize)
{
    m_imagesize = imagesize;
}

void GameMap::removePlayer(qint32 index)
{
    m_players.removeAt(index);
}

Unit* GameMap::spawnUnit(qint32 x, qint32 y, const QString & unitID, Player* owner, qint32 range, bool ignoreMovement)
{
    CONSOLE_PRINT("spawning Unit", Console::eDEBUG);
    if (owner != nullptr)
    {
        qint32 heigth = getMapHeight();
        qint32 width = getMapWidth();
        if (range < 0)
        {
            range = width + heigth;
        }
        spPlayer pPlayer;
        for (qint32 i = 0; i < m_players.size(); i++)
        {
            if (owner == m_players[i].get())
            {
                pPlayer = m_players[i];
                break;
            }
            else if (i == m_players.size() - 1)
            {
                // cancel since we have no owner for the unit
                CONSOLE_PRINT("Invalid player selected. Didn't spawn unit " + unitID, Console::eERROR);
                return nullptr;
            }
        }

        qint32 unitLimit = m_Rules->getUnitLimit();
        qint32 unitCount = pPlayer->getUnitCount();
        if (unitLimit > 0 && unitCount >= unitLimit)
        {
            CONSOLE_PRINT("Didn't spawn unit " + unitID + " cause unit limit is reached", Console::eDEBUG);
            return nullptr;
        }
        spUnit pUnit = spUnit::create(unitID, pPlayer.get(), true, this);
        MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
        QString movementType = pUnit->getMovementType();
        if (onMap(x, y))
        {
            spTerrain pTerrain = spTerrain(getTerrain(x, y));
            if (pTerrain.get() != nullptr &&
                (pTerrain->getUnit() == nullptr) &&
                (ignoreMovement || pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
            {
                pTerrain->setUnit(pUnit);
                return pUnit.get();
            }
        }

        qint32 currentRadius = 0;
        qint32 x2 = 0;
        qint32 y2 = 0;
        bool found = false;
        while ((found == false) && (currentRadius <= range) && (range > 0))
        {
            currentRadius += 1;
            x2 = -currentRadius;
            y2 = 0;
            for (qint32 i = 0; i < currentRadius; i++)
            {
                x2 += 1;
                y2 += 1;
                if (onMap(x + x2, y + y2))
                {
                    spTerrain pTerrain = spTerrain(getTerrain(x + x2 - currentRadius, y + y2));
                    if (pTerrain.get() != nullptr &&
                        (pTerrain->getUnit() == nullptr) &&
                        (ignoreMovement || pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
                    {
                        pTerrain->setUnit(pUnit);
                        return pUnit.get();
                    }
                }
            }
            for (qint32 i = 0; i < currentRadius; i++)
            {
                x2 += 1;
                y2 -= 1;
                if (onMap(x + x2, y + y2))
                {
                    spTerrain pTerrain = spTerrain(getTerrain(x + x2, y + y2));
                    if ((pTerrain.get() != nullptr &&
                         pTerrain->getUnit() == nullptr) &&
                        (ignoreMovement || pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
                    {
                        pTerrain->setUnit(pUnit);
                        return pUnit.get();
                    }
                }
            }
            for (qint32 i = 0; i < currentRadius; i++)
            {
                x2 -= 1;
                y2 -= 1;
                if (onMap(x + x2, y + y2))
                {
                    spTerrain pTerrain = spTerrain(getTerrain(x + x2, y + y2));
                    if ((pTerrain.get() != nullptr &&
                         pTerrain->getUnit() == nullptr) &&
                        (ignoreMovement || pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
                    {
                        pTerrain->setUnit(pUnit);
                        return pUnit.get();
                    }
                }
            }
            for (qint32 i = 0; i < currentRadius; i++)
            {
                x2 -= 1;
                y2 += 1;
                if (onMap(x + x2, y + y2))
                {
                    spTerrain pTerrain = spTerrain(getTerrain(x + x2, y + y2));
                    if ((pTerrain.get() != nullptr &&
                         pTerrain->getUnit() == nullptr) &&
                        (ignoreMovement || pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
                    {
                        pTerrain->setUnit(pUnit);
                        return pUnit.get();
                    }
                }
            }
            if (currentRadius > getMapWidth() && currentRadius > getMapHeight())
            {
                break;
            }
        }
    }
    CONSOLE_PRINT("Didn't spawn unit " + unitID + " didn't find an empty field", Console::eDEBUG);
    return nullptr;
}

qint32 GameMap::getMapWidth() const
{
    if (m_fields.size() > 0)
    {
        return m_fields[0].size();
    }
    return 0;
}

qint32 GameMap::getMapHeight() const
{
    return m_fields.size();
}

qint32 GameMap::getBuildingCount(const QString & buildingID)
{
    qint32 ret = 0;
    qint32 width = getMapWidth();
    qint32 heigth = getMapHeight();
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            Building* pBuilding = getTerrain(x, y)->getBuilding();
            if (pBuilding != nullptr)
            {
                if (pBuilding->getBuildingID() == buildingID || buildingID.isEmpty())
                {
                    if (pBuilding->Building::getX() == x && pBuilding->Building::getY() == y)
                    {
                        ret++;
                    }
                }
            }
        }
    }
    return ret;
}

void GameMap::getField(qint32& x, qint32& y, GameEnums::Directions direction)
{
    switch (direction)
    {
        case GameEnums::Directions_North:
        {
            y--;
            break;
        }
        case GameEnums::Directions_NorthEast:
        {
            x++;
            y--;
            break;
        }
        case GameEnums::Directions_East:
        {
            x++;
            break;
        }
        case GameEnums::Directions_SouthEast:
        {
            y++;
            x++;
            break;
        }
        case GameEnums::Directions_South:
        {
            y++;
            break;
        }
        case GameEnums::Directions_SouthWest:
        {
            y++;
            x--;
            break;
        }
        case GameEnums::Directions_West:
        {
            x--;
            break;
        }
        case GameEnums::Directions_NorthWest:
        {
            y--;
            x--;
            break;
        }
        default:
        {
            // do nothing
        }
    }
}

bool GameMap::onMap(qint32 x, qint32 y)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    if ((x >= 0) &&
        (y >= 0) &&
        (x < width) &&
        (y < heigth))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GameMap::centerMap(qint32 x, qint32 y, bool updateMinimapPosition)
{
    if (onMap(x, y))
    {
        // draw point
        BaseGamemenu* pMenu = BaseGamemenu::getInstance();
        if (pMenu != nullptr)
        {
            oxygine::spSlidingActorNoClipRect pMapSliding = pMenu->getMapSliding();
            oxygine::spActor pMapSlidingActor = pMenu->getMapSlidingActor();
            if (pMapSliding.get() != nullptr &&
                pMapSlidingActor.get() != nullptr)
            {
                qint32 newX = pMapSliding->getWidth() / 2.0f - x * getZoom() * m_imagesize - m_imagesize / 2.0f;
                qint32 newY = pMapSliding->getHeight() / 2.0f - y * getZoom() * m_imagesize - m_imagesize / 2.0f;
                limitPosition(pMenu, newX, newY);
                pMapSlidingActor->setPosition(newX, newY);
                pMenu->getCursor()->setMapPoint(x, y);
            }
        }
        if (updateMinimapPosition)
        {
            emit sigMovedMap();
        }
    }
}

QPoint GameMap::getCenteredPosition()
{
    BaseGamemenu* pMenu = BaseGamemenu::getInstance();
    qint32 x = 0;
    qint32 y = 0;
    if (pMenu != nullptr)
    {
        oxygine::spSlidingActorNoClipRect pMapSliding = pMenu->getMapSliding();
        oxygine::spActor pMapSlidingActor = pMenu->getMapSlidingActor();
        x = -(pMapSlidingActor->getX() - pMapSliding->getWidth() / 2.0f + m_imagesize / 2.0f) / (getZoom() * m_imagesize);
        y = -(pMapSlidingActor->getY() - pMapSliding->getHeight() / 2.0f + m_imagesize / 2.0f) / (getZoom() * m_imagesize);
    }
    return QPoint(x, y);
}

void GameMap::moveMap(qint32 x, qint32 y)
{
    BaseGamemenu* pMenu = BaseGamemenu::getInstance();
    if (pMenu != nullptr)
    {
        // draw point
        oxygine::spActor pActor = pMenu->getMapSlidingActor();
        qint32 newX = pActor->getX() + x;
        qint32 newY = pActor->getY() + y;
        limitPosition(pMenu, newX, newY);
        pActor->setPosition(newX, newY);
        emit sigMovedMap();
    }
}

void GameMap::limitPosition(BaseGamemenu* pMenu, qint32 & newX, qint32 & newY)
{
    oxygine::RectF bounds = pMenu->getMapSliding()->getDragBounds();
    if (newX < bounds.getLeft())
    {
        newX = bounds.getLeft();
    }
    else if (newX > bounds.getRight())
    {
        newX = bounds.getRight();
    }
    if (newY < bounds.getTop())
    {
        newY = bounds.getTop();
    }
    else if (newY > bounds.getBottom())
    {
        newY = bounds.getBottom();
    }
}

void GameMap::setZoom(float zoom)
{
    float curZoom = getScaleX();
    if (zoom > 0)
    {
        curZoom *= 2.0f;
    }
    else
    {
        curZoom /= 2.0f;
    }
    // limit zoom

    float minLimit = 1.0f / 4.0f;
    if (Mainapp::getInstance()->devicePixelRatio() >= 2.0f && !Settings::getUseHighDpi())
    {
        minLimit = 1.0f / 8.0f;
    }
    if (curZoom > 16.0f)
    {
        curZoom = 16.0f;
    }
    else if (curZoom < minLimit)
    {
        curZoom = minLimit;
    }
    else
    {
        // all fine
    }
    setScale(curZoom);
    BaseGamemenu* pMenu = BaseGamemenu::getInstance();
    if (pMenu != nullptr)
    {
        pMenu->updateSlidingActorSize();
    }
    emit sigZoomChanged(curZoom);
}

void GameMap::zoomChanged()
{
    Interpreter::getInstance()->doFunction("onZoomLevelChanged");
}

void GameMap::replaceTerrainOnly(const QString & terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain, bool removeUnit)
{
    if (onMap(x, y))
    {
        spTerrain pTerrainOld = m_fields[y][x];
        if (pTerrainOld->getTerrainID() != terrainID)
        {
            pTerrainOld->removeBuilding();
            spUnit pUnit = spUnit(pTerrainOld->getUnit());

            spTerrain pTerrain = Terrain::createTerrain(terrainID, x, y, pTerrainOld->getTerrainID(), this);

            Interpreter* pInterpreter = Interpreter::getInstance();
            QString function1 = "useTerrainAsBaseTerrain";
            QJSValue terrainAsBaseTerrain = pInterpreter->doFunction(terrainID, function1);

            if (useTerrainAsBaseTerrain && terrainAsBaseTerrain.toBool() && canBePlaced(terrainID, x, y))
            {
                pTerrainOld->detach();
                pTerrain->setBaseTerrain(pTerrainOld);
                m_fields[y][x] = pTerrain;
                m_rowSprites[y]->addChild(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            }
            else
            {
                pTerrainOld->detach();
                m_fields[y][x] = pTerrain;
                m_rowSprites[y]->addChild(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            }
            if (!removeUnit)
            {
                pTerrain->setUnit(pUnit);
            }
            // force consistent rendering order for terrains
            qint32 mapWidth = getMapWidth();
            for (qint32 xPos = x + 1; xPos < mapWidth; xPos++)
            {
                spTerrain pTerrain = m_fields[y][xPos];
                pTerrain->detach();
                m_rowSprites[y]->addChild(pTerrain);
            }
        }
        else
        {
            pTerrainOld->setBuilding(spBuilding());
        }
    }
}

void GameMap::replaceTerrain(const QString & terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain, bool callUpdateSprites, bool checkPlacement)
{
    replaceTerrainOnly(terrainID, x, y, useTerrainAsBaseTerrain);
    if (checkPlacement)
    {
        updateTerrain(x, y);
    }
    if (callUpdateSprites)
    {
        updateSprites(x, y);
    }
}

void GameMap::replaceBuilding(const QString & buildingID, qint32 x, qint32 y)
{
    if (onMap(x, y))
    {
        spBuilding pBuilding = spBuilding::create(buildingID, this);
        Terrain* pTerrain = getTerrain(x, y);
        if (pBuilding->canBuildingBePlaced(pTerrain))
        {
            pTerrain->setBuilding(pBuilding);
            pBuilding->setOwner(nullptr);
            
        }
    }
}

void GameMap::updateTerrain(qint32 x, qint32 y)
{
    for (qint32 xPos = x - 1; xPos <= x + 1; xPos++)
    {
        for (qint32 yPos = y - 1; yPos <= y + 1; yPos++)
        {
            if (!((xPos == x) && (yPos == y)))
            {
                if (onMap(xPos, yPos))
                {
                    if (!canBePlaced(m_fields[yPos][xPos]->getTerrainID(), xPos, yPos))
                    {
                        replaceTerrain(m_fields[yPos][xPos]->getBaseTerrainID(), xPos, yPos, false, true);
                    }
                }
            }
        }
    }
}

bool GameMap::canBePlaced(const QString & terrainID, qint32 x, qint32 y)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "canBePlaced";
    QJSValueList args({x,
                       y,
                       pInterpreter->newQObject(this)});
    QJSValue placeable = pInterpreter->doFunction(terrainID, function, args);
    if (placeable.isBool())
    {
        return placeable.toBool();
    }
    else
    {
        return false;
    }
}

void GameMap::serializeObject(QDataStream& pStream) const
{
    CONSOLE_PRINT("storing map", Console::eDEBUG);
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    // store header
    pStream << getVersion();
    pStream << m_headerInfo.m_mapName;
    pStream << m_headerInfo.m_mapAuthor;
    pStream << m_headerInfo.m_mapDescription;
    pStream << width;
    pStream << heigth;
    pStream << m_headerInfo.m_uniqueIdCounter;
    pStream << getPlayerCount();
    updateMapFlags();
    pStream << static_cast<quint64>(m_headerInfo.m_mapFlags);
    qint32 currentPlayerIdx = 0;
    for (qint32 i = 0; i < m_players.size(); i++)
    {
        if (m_CurrentPlayer.get() == m_players[i].get())
        {
            currentPlayerIdx = i;
        }
        m_players[i]->serializeObject(pStream);
    }


    pStream << currentPlayerIdx;
    pStream << m_currentDay;
    // store map
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            // serialize
            m_fields[y][x]->serializeObject(pStream);
        }
    }
    m_Rules->serializeObject(pStream);
    m_Recorder->serializeObject(pStream);
    m_GameScript->serializeObject(pStream);
    if (m_Campaign.get() != nullptr)
    {
        pStream << true;
        m_Campaign->serializeObject(pStream);
    }
    else
    {
        pStream << false;
    }
    pStream << m_mapPath;
    pStream << m_mapMusic;
    pStream << m_startLoopMs;
    pStream << m_endLoopMs;
    pStream << m_isHumanMatch;
}

void GameMap::updateMapFlags() const
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            QString id = m_fields[y][x]->getID();
            if (id == "TELEPORTTILE")
            {
                m_headerInfo.m_mapFlags = static_cast<GameEnums::MapFilterFlags>(m_headerInfo.m_mapFlags | GameEnums::MapFilterFlags_Teleport);
            }
            else if (id == "FACTORY")
            {
                m_headerInfo.m_mapFlags = static_cast<GameEnums::MapFilterFlags>(m_headerInfo.m_mapFlags | GameEnums::MapFilterFlags_Bases);
            }
            else if (id == "AIRPORT")
            {
                m_headerInfo.m_mapFlags = static_cast<GameEnums::MapFilterFlags>(m_headerInfo.m_mapFlags | GameEnums::MapFilterFlags_Airport);
            }
            else if (id == "HARBOUR")
            {
                m_headerInfo.m_mapFlags = static_cast<GameEnums::MapFilterFlags>(m_headerInfo.m_mapFlags | GameEnums::MapFilterFlags_Harbour);
            }
            else if (id == "TOWER")
            {
                m_headerInfo.m_mapFlags = static_cast<GameEnums::MapFilterFlags>(m_headerInfo.m_mapFlags | GameEnums::MapFilterFlags_Tower);
            }
        }
    }
}

void GameMap::setMenu(GameMenue *newMenu)
{
    m_pMenu = newMenu;
}

oxygine::spActor GameMap::getUnitsLayer() const
{
    return m_unitsLayer;
}

oxygine::spActor GameMap::getMoveArrowLayer() const
{
    return m_moveArrowLayer;
}

oxygine::spActor GameMap::getMarkedFieldsLayer() const
{
    return m_markedFieldsLayer;
}

void GameMap::readMapHeader(QDataStream& pStream, MapHeaderInfo & headerInfo)
{
    pStream >> headerInfo.m_Version;
    if (headerInfo.m_Version > 1)
    {
        pStream >> headerInfo.m_mapName;
    }
    if (headerInfo.m_Version > 4)
    {
        pStream >> headerInfo.m_mapAuthor;
        pStream >> headerInfo.m_mapDescription;
    }
    pStream >> headerInfo.m_width;
    pStream >> headerInfo.m_heigth;
    if (headerInfo.m_Version > 6)
    {
        pStream >> headerInfo.m_uniqueIdCounter;
    }
    else
    {
        headerInfo.m_uniqueIdCounter = 0;
    }
    pStream >> headerInfo.m_playerCount;
    if (headerInfo.m_Version > 11)
    {
        quint64 flags;
        pStream >> flags;
        headerInfo.m_mapFlags = static_cast<GameEnums::MapFilterFlags>(flags);
    }
}

void GameMap::deserializer(QDataStream& pStream, bool fast)
{
    clearMap();
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    // restore map header
    readMapHeader(pStream, m_headerInfo);

    CONSOLE_PRINT("Loading map " + m_headerInfo.m_mapName + " Fast =" + (fast ? "true" : "false"), Console::eDEBUG);
    qint32 mapSize = m_headerInfo.m_width * m_headerInfo.m_heigth;
    setSize(m_headerInfo.m_width * GameMap::getImageSize(), m_headerInfo.m_heigth * GameMap::getImageSize());
    bool showLoadingScreen = (mapSize >= loadingScreenSize) && !fast;
    if (showLoadingScreen)
    {
        pLoadingScreen->show();
    }
    if (showLoadingScreen)
    {
        pLoadingScreen->setProgress(tr("Loading Players"), 5);
    }
    CONSOLE_PRINT("Loading players count: " + QString::number(m_headerInfo.m_playerCount), Console::eDEBUG);
    for (qint32 i = 0; i < m_headerInfo.m_playerCount; i++)
    {
        // create player
        m_players.append(spPlayer::create(this));
        // get player data from stream
        m_players[i]->deserializer(pStream, fast);
    }

    qint32 currentPlayerIdx = 0;
    if (m_headerInfo.m_Version > 1)
    {
        pStream >> currentPlayerIdx;
        pStream >> m_currentDay;
    }

    // restore map
    m_fields.reserve(m_headerInfo.m_heigth);
    m_rowSprites.reserve(m_headerInfo.m_heigth);
    for (qint32 y = 0; y < m_headerInfo.m_heigth; y++)
    {
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Loading Map Row ") + QString::number(y) + tr(" of ") + QString::number(m_headerInfo.m_heigth), 5 + 75 * y / m_headerInfo.m_heigth);
        }
        m_fields.push_back(std::vector<spTerrain>(m_headerInfo.m_width, spTerrain()));
        auto pActor = oxygine::spActor::create();
        pActor->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        m_rowSprites.push_back(pActor);
        addChild(pActor);
        for (qint32 x = 0; x < m_headerInfo.m_width; x++)
        {
            spTerrain pTerrain = Terrain::createTerrain("", x, y, "", this);
            m_fields[y][x] = pTerrain;
            pTerrain->deserializer(pStream, fast);
            if (pTerrain->isValid())
            {
                if (!fast)
                {
                    m_rowSprites[y]->addChild(pTerrain);
                    pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
                }
            }
            else
            {
                replaceTerrain(GameMap::PLAINS, x, y, false, false, false);
            }
        }
    }
    setCurrentPlayer(currentPlayerIdx);
    m_Rules = spGameRules::create(this);
    if (showLoadingScreen)
    {
        pLoadingScreen->setProgress(tr("Loading Rules"), 80);
    }
    if (m_headerInfo.m_Version > 2)
    {
        m_Rules->deserializer(pStream, fast);
    }
    if (!fast)
    {
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Loading Record"), 85);
        }
        if (m_headerInfo.m_Version > 3)
        {
            m_Recorder->deserializeObject(pStream);
        }
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Loading scripts"), 90);
        }
        if (m_headerInfo.m_Version > 5)
        {
            m_GameScript->deserializeObject(pStream);
        }
        else
        {
            m_GameScript = spGameScript::create(this);
        }
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Loading Campaign"), 95);
        }
        if (m_headerInfo.m_Version > 7)
        {
            bool exists = false;
            pStream >> exists;
            if (exists)
            {
                m_Campaign = spCampaign::create();
                m_Campaign->deserializeObject(pStream);
            }
        }
        if (m_headerInfo.m_Version > 8)
        {
            pStream >> m_mapPath;
        }
        if (m_headerInfo.m_Version > 9)
        {
            pStream >> m_mapMusic;
            pStream >> m_startLoopMs;
            pStream >> m_endLoopMs;
        }
        if (m_headerInfo.m_Version > 10)
        {
            pStream >> m_isHumanMatch;
        }
    }
    if (showLoadingScreen)
    {
        pLoadingScreen->hide();
    }
}

void GameMap::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void GameMap::exitGame()
{
    emit signalExitGame();
}

void GameMap::options()
{
    emit sigShowOptions();
}

void GameMap::changeSound()
{
    emit sigShowChangeSound();
}

void GameMap::surrenderGame()
{
    emit sigSurrenderGame();
}

void GameMap::nicknameUnit(qint32 x, qint32 y)
{
    emit sigShowNicknameUnit(x, y);
}

void GameMap::saveGame()
{
    emit signalSaveGame();
}

void GameMap::victoryInfo()
{
    emit sigVictoryInfo();
}

void GameMap::showCOInfo()
{
    emit signalShowCOInfo();
}

void GameMap::showGameInfo(qint32 player)
{
    emit sigShowGameInfo(player);
}

void GameMap::showAttackLog(qint32 player)
{
    emit sigShowAttackLog(player);
}

void GameMap::showUnitInfo(qint32 player)
{
    emit sigShowUnitInfo(player);
}

void GameMap::showWiki()
{
    emit sigShowWiki();
}

void GameMap::showRules()
{
    emit sigShowRules();
}

void GameMap::showUnitStatistics(qint32 player)
{
    emit sigShowUnitStatistics(player);
}

void GameMap::showMovementPlanner()
{
    emit sigShowMovementPlanner();
}

void GameMap::showDamageCalculator()
{
    emit sigShowDamageCalculator();
}

void GameMap::startGame()
{
    CONSOLE_PRINT("GameMap::startGame()", Console::eDEBUG);
    m_Recorder = spGameRecorder::create(this);
    for (qint32 y = 0; y < m_fields.size(); y++)
    {
        for (qint32 x = 0; x < m_fields[y].size(); x++)
        {
            Unit* pUnit = m_fields[y][x]->getUnit();
            if (pUnit != nullptr)
            {
                pUnit->applyMod();
            }
        }
    }
    Userdata* pUserdata = Userdata::getInstance();
    auto lockedUnits = pUserdata->getShopItemsList(GameEnums::ShopItemType_Unit, false);

    for (qint32 i = 0; i < m_players.size(); i++)
    {
        m_players[i]->loadVisionFields();
        m_players[i]->setBuildlistChanged(true);

        CoreAI* pAI = dynamic_cast<CoreAI*>(m_players[i]->getBaseGameInput());
        HumanPlayerInput* pHuman = dynamic_cast<HumanPlayerInput*>(m_players[i]->getBaseGameInput());
        if (pAI != nullptr)
        {
            pAI->setEnableNeutralTerrainAttack(m_Rules->getAiAttackTerrain());
            CONSOLE_PRINT("Player " + QString::number(i) + " uses ai " + QString::number(m_players[i]->getBaseGameInput()->getAiType()), Console::eDEBUG);
        }
        else if (pHuman != nullptr)
        {
            auto buildList = m_players[i]->getBuildList();
            for (const auto & unitId : lockedUnits)
            {
                buildList.removeAll(unitId);
            }
            m_players[i]->setBuildList(buildList);
            CONSOLE_PRINT("Player " + QString::number(i) + " uses ai " + QString::number(m_players[i]->getBaseGameInput()->getAiType()), Console::eDEBUG);
        }
    }
    QStringList mods = Settings::getMods();
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (const auto& mod : mods)
    {
        if (QFile::exists(mod + "/scripts/mapstart.js"))
        {
            pInterpreter->openScript(mod + "/scripts/mapstart.js", true);
            pInterpreter->doFunction("MapStart", "gameStart");
        }
    }
    if (Settings::getSyncAnimations())
    {
        syncUnitsAndBuildingAnimations();
    }
}

void GameMap::clearMap()
{
    for (qint32 y = 0; y < m_fields.size(); y++)
    {
        m_rowSprites[y]->detach();
        for (qint32 x = 0; x < m_fields[y].size(); x++)
        {
            m_fields[y][x]->detach();
        }
        m_fields[y].clear();
    }
    m_rowSprites.clear();
    m_fields.clear();
    m_players.clear();
    m_Rules->resetWeatherSprites();
    m_Rules->resetFogSprites();
    m_headerInfo = MapHeaderInfo();
}

QString GameMap::readMapName(QDataStream& pStream)
{
    QString name;
    // restore map header
    qint32 version = 0;
    pStream >> version;
    if (version > 1)
    {
        pStream >> name;
    }
    return name;
}

qint32 GameMap::getImageSize()
{
    return m_imagesize;
}

void GameMap::enableUnits(Player* pPlayer)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = m_fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                if (pUnit->getOwner() == pPlayer)
                {
                    pUnit->setHasMoved(false);
                }
            }
        }
    }
}

bool GameMap::nextPlayer()
{
    bool nextDay = false;
    bool found = false;
    qint32 start = m_CurrentPlayer->getPlayerID();
    m_CurrentPlayer->updatePlayerVision(true);
    for (qint32 i = start + 1; i < m_players.size(); i++)
    {
        m_CurrentPlayer = m_players[i];
        if (!m_CurrentPlayer->getIsDefeated())
        {
            found = true;
            break;
        }
        else
        {
            startOfTurn(m_players[i].get());
        }
    }
    if (!found)
    {
        nextDay = true;
        m_currentDay++;
        for (qint32 i = 0; i < m_players.size(); i++)
        {
            m_CurrentPlayer = m_players[i];
            if (!m_CurrentPlayer->getIsDefeated())
            {
                break;
            }
            else
            {
                startOfTurn(m_players[i].get());
            }
        }
    }
    return nextDay;
}

void GameMap::updateUnitIcons()
{

    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = m_fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                pUnit->updateIcons(getCurrentViewPlayer());
                
            }
        }
    }
}

qint32 GameMap::getWinnerTeam()
{
    qint32 winnerTeam = -1;
    for (qint32 i = 0; i < m_players.size(); i++)
    {
        if (!m_players[i]->getIsDefeated())
        {
            if (winnerTeam >= 0 &&
                winnerTeam != m_players[i]->getTeam())
            {
                winnerTeam = -1;
                break;
            }
            else if (winnerTeam < 0)
            {
                winnerTeam = m_players[i]->getTeam();
            }
        }
    }
    return winnerTeam;
}

void GameMap::setCampaign(spCampaign campaign)
{
    m_Campaign = campaign;
}

spCampaign GameMap::getSpCampaign()
{
    return m_Campaign;
}

GameScript* GameMap::getGameScript()
{
    return m_GameScript.get();
}

Campaign* GameMap::getCampaign()
{
    return m_Campaign.get();
}

GameRecorder* GameMap::getGameRecorder()
{
    return m_Recorder.get();
}

QString GameMap::getMapDescription() const
{
    return m_headerInfo.m_mapDescription;
}

void GameMap::setMapDescription(const QString &value)
{
    m_headerInfo.m_mapDescription = value;
}

QString GameMap::getMapAuthor() const
{
    return m_headerInfo.m_mapAuthor;
}

void GameMap::setMapAuthor(const QString &value)
{
    m_headerInfo.m_mapAuthor = value;
}

qint32 GameMap::getCurrentDay() const
{
    return m_currentDay;
}

void GameMap::refillAll()
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = m_fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                pUnit->refill();
                refillTransportedUnits(pUnit.get());
            }
        }
    }
}

void GameMap::refillTransportedUnits(Unit* pUnit)
{
    if (pUnit != nullptr)
    {
        for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
        {
            pUnit->getLoadedUnit(i)->refill();
            refillTransportedUnits(pUnit->getLoadedUnit(i));
        }
    }
}

Player* GameMap::getCurrentViewPlayer()
{
    if (m_pMenu != nullptr)
    {
        return m_pMenu->getCurrentViewPlayer();
    }
    return getCurrentPlayer();
}

void GameMap::startOfTurn(Player* pPlayer)
{    
    if (pPlayer != nullptr)
    {
        pPlayer->startOfTurn();
        if (!pPlayer->getIsDefeated())
        {
            pPlayer->getBaseGameInput()->centerCameraOnAction(nullptr);
        }
        startOfTurnPlayer(pPlayer);
    }
    else
    {
        startOfTurnNeutral();
    }
}

void GameMap::startOfTurnNeutral()
{
    CONSOLE_PRINT("Doing start of turn for neutrals", Console::eDEBUG);
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    auto xValues = GlobalUtils::getRandomizedArray(0, width - 1);
    auto yValues = GlobalUtils::getRandomizedArray(0, heigth - 1);
    for (auto y : yValues)
    {
        for (auto x : xValues)
        {
            spTerrain pTerrain = m_fields[y][x];
            pTerrain->startOfTurn();
            spBuilding pBuilding = pTerrain->getSpBuilding();
            if (pBuilding.get() != nullptr &&
                pBuilding->getOwner() == nullptr &&
                (pBuilding->Building::getX() == x &&
                 pBuilding->Building::getY() == y))
            {
                pBuilding->startOfTurn();
            }
        }
    }
}

void GameMap::startOfTurnPlayer(Player* pPlayer)
{
    CONSOLE_PRINT("Doing start of turn for player " + QString::number(pPlayer->getPlayerID()), Console::eDEBUG);
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    qint32 playerId = pPlayer->getPlayerID();
    auto xValues = GlobalUtils::getRandomizedArray(0, width - 1);
    auto yValues = GlobalUtils::getRandomizedArray(0, heigth - 1);
    // update icons
    for (auto y : yValues)
    {
        for (auto x : xValues)
        {
            spUnit pUnit = m_fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                if (pUnit->getOwner() == pPlayer)
                {
                    pUnit->removeShineTween();
                    pUnit->updateUnitStatus();
                }
                pUnit->updateStatusDurations(playerId);
            }
        }
    }
    // update start of turn
    for (auto y : yValues)
    {
        for (auto x : xValues)
        {
            spUnit pUnit = m_fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                if (pUnit->getOwner() == pPlayer)
                {
                    pUnit->startOfTurn();
                }
                pUnit->updateIcons(getCurrentViewPlayer());
            }
            spBuilding pBuilding = m_fields[y][x]->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->getOwner() == pPlayer &&
                    (pBuilding->Building::getX() == x && pBuilding->Building::getY() == y))
                {
                    pBuilding->startOfTurn();
                }
            }
        }
    }
}

void GameMap::centerOnPlayer(Player* pPlayer)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    QPoint unitWarp = QPoint(-1, -1);
    QPoint hqWarp = QPoint(-1, -1);
    QPoint buildingWarp = QPoint(-1, -1);
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = m_fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                if (pUnit->getOwner() == pPlayer)
                {
                    if (unitWarp.x() <  0)
                    {
                        unitWarp = QPoint(x, y);
                    }
                }
            }
            spBuilding pBuilding = m_fields[y][x]->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->getOwner() == pPlayer &&
                    (pBuilding->Building::getX() == x && pBuilding->Building::getY() == y))
                {
                    if (pBuilding->getBuildingID() == "HQ" &&
                        hqWarp.x() < 0)
                    {
                        hqWarp = QPoint(x, y);
                    }
                    else if (buildingWarp.x() < 0)
                    {
                        buildingWarp = QPoint(x, y);
                    }
                }

            }
        }
    }
    if (onMap(hqWarp.x(), hqWarp.y()))
    {
        centerMap(hqWarp.x(), hqWarp.y());
    }
    else if (onMap(unitWarp.x(), unitWarp.y()))
    {
        centerMap(unitWarp.x(), unitWarp.y());
    }
    else if (onMap(buildingWarp.x(), buildingWarp.y()))
    {
        centerMap(buildingWarp.x(), buildingWarp.y());
    }
}

void GameMap::checkFuel(Player* pPlayer)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = m_fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                if ((pUnit->getOwner() == pPlayer) &&
                    (pUnit->getFuel() < 0) &&
                    (pUnit->getMaxFuel() > 0))
                {
                    pUnit->killUnit();
                }
            }
        }
    }
}

Unit* GameMap::getUnit(qint32 uniqueID)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            Unit* pUnit = m_fields[y][x]->getUnit();
            if (pUnit != nullptr)
            {
                if (pUnit->getUniqueID() == uniqueID)
                {
                    return pUnit;
                }
                else if (pUnit->getLoadedUnitCount() > 0)
                {
                    pUnit = getUnitFromTansportUnit(pUnit, uniqueID);
                    if (pUnit != nullptr)
                    {
                        return pUnit;
                    }
                }
            }
        }
    }
    return nullptr;
}

Unit* GameMap::getUnitFromTansportUnit(Unit* pUnit, qint32 uniqueID)
{
    if (pUnit != nullptr)
    {
        for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
        {
            Unit* pLoadedUnit = pUnit->getLoadedUnit(i);
            if (pLoadedUnit->getUniqueID() == uniqueID)
            {
                return pLoadedUnit;
            }
            else
            {
                Unit* pUnit2 = getUnitFromTansportUnit(pLoadedUnit, uniqueID);
                if (pUnit2 != nullptr)
                {
                    return pUnit2;
                }
            }
        }
    }
    return nullptr;
}

QmlVectorUnit* GameMap::getUnits(Player* pPlayer)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    QmlVectorUnit* ret = new QmlVectorUnit();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = m_fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                if ((pUnit->getOwner() == pPlayer))
                {
                    ret->append(pUnit.get());
                }
            }
        }
    }
    return ret;
}

QmlVectorBuilding* GameMap::getBuildings(Player* pPlayer, QString id)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    QmlVectorBuilding* ret = new QmlVectorBuilding();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spBuilding pBuilding = m_fields[y][x]->getSpBuilding();
            if (pBuilding.get() != nullptr && pBuilding->getTerrain() == m_fields[y][x].get())
            {
                if (pBuilding->getOwner() == pPlayer &&
                    (id.isEmpty() || pBuilding->getBuildingID() == id))
                {
                    ret->append(pBuilding.get());
                }
            }
        }
    }
    return ret;
}

QString GameMap::getMapName() const
{
    return m_headerInfo.m_mapName;
}

void GameMap::setMapName(const QString &value)
{
    m_headerInfo.m_mapName = value;
}

void GameMap::nextTurn(quint32 dayToDayUptimeMs)
{
    if (anyPlayerAlive())
    {
        CONSOLE_PRINT("GameMap::nextTurn", Console::eDEBUG);
        m_Rules->checkVictory();
        enableUnits(m_CurrentPlayer.get());
        bool nextDay = nextPlayer();
        bool permanent = false;
        bool found = false;
        auto* baseGameInput = m_CurrentPlayer->getBaseGameInput();
        if ((m_Rules->getDayToDayScreen() == GameRules::DayToDayScreen::Permanent ||
             m_Rules->getFogMode() != GameEnums::Fog::Fog_Off) &&
            baseGameInput != nullptr)
        {
            if (baseGameInput->getAiType() == GameEnums::AiTypes_Human)
            {
                // search for previous player
                qint32 currentPlayerID = m_CurrentPlayer->getPlayerID();
                for (qint32 i = currentPlayerID - 1; i >= 0; i--)
                {
                    if (m_players[i]->getBaseGameInput() != nullptr &&
                        m_players[i]->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human &&
                        !m_players[i]->getIsDefeated())
                    {
                        if (m_players[i]->getTeam() != m_CurrentPlayer->getTeam())
                        {
                            permanent = true;
                        }
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    for (qint32 i = m_players.size() - 1; i > currentPlayerID; i--)
                    {
                        if (m_players[i]->getBaseGameInput() != nullptr &&
                            m_players[i]->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
                        {
                            if (m_players[i]->getTeam() != m_CurrentPlayer->getTeam())
                            {
                                permanent = true;
                            }
                            break;
                        }
                    }
                }
            }
        }
        if (permanent)
        {
            if (m_pMenu != nullptr)
            {
                spGameAnimationNextDay pAnim = spGameAnimationNextDay::create(this, m_CurrentPlayer.get(), GameMap::frameTime, true);
                m_pMenu->addChild(pAnim);
            }
        }
        else
        {
            GameAnimationFactory::createGameAnimationNextDay(this, m_CurrentPlayer.get(), GameMap::frameTime, dayToDayUptimeMs);
        }
        if (nextDay)
        {
            startOfTurn(nullptr);
            m_Recorder->newDay();
        }
        m_Rules->startOfTurn(nextDay);
        m_CurrentPlayer->earnMoney();
        startOfTurn(m_CurrentPlayer.get());
        checkFuel(m_CurrentPlayer.get());
        m_Recorder->updatePlayerData(m_CurrentPlayer->getPlayerID());
        m_Rules->initRoundTime();
        if (m_pMenu != nullptr)
        {
            m_pMenu->updatePlayerinfo();
            m_pMenu->updateMinimap();
        }
        playMusic();
        if (baseGameInput->getAiType() == GameEnums::AiTypes_Human)
        {
            Mainapp* pApp = Mainapp::getInstance();
            pApp->getAudioThread()->playSound("player_turn.wav");
        }
    }
}

void GameMap::playMusic()
{
    if (m_mapMusic.isEmpty())
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->getAudioThread()->clearPlayList();
        m_CurrentPlayer->loadCOMusic();
        pApp->getAudioThread()->playRandom();
    }
    else if (m_loadedMapMusic != m_mapMusic)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->getAudioThread()->clearPlayList();
        pApp->getAudioThread()->addMusic(m_mapMusic, m_startLoopMs, m_endLoopMs);
        m_loadedMapMusic = m_mapMusic;
        pApp->getAudioThread()->playRandom();
    }
}

void GameMap::initPlayersAndSelectCOs()
{
    bool singleCO = m_Rules->getSingleRandomCO();
    QStringList bannList = m_Rules->getCOBannlist();
    for (qint32 i = 0; i < getPlayerCount(); i++)
    {
        Player* pPlayer = getPlayer(i);
        if (pPlayer->getCO(0) != nullptr)
        {
            bannList.removeAll(pPlayer->getCO(0)->getCoID());
        }
        if (pPlayer->getCO(1) != nullptr)
        {
            bannList.removeAll(pPlayer->getCO(1)->getCoID());
        }
    }
    Userdata* pUserdata = Userdata::getInstance();
    auto items = pUserdata->getShopItemsList(GameEnums::ShopItemType_CO, false);
    for (const auto & item : items)
    {
        bannList.removeAll(item);
    }
    // fix some stuff for the players based on our current input
    for (qint32 i = 0; i < getPlayerCount(); i++)
    {
        Player* pPlayer = getPlayer(i);
        if (pPlayer->getBaseGameInput() == nullptr)
        {
            CONSOLE_PRINT("Forcing AI for player " + QString::number(i) + " to human.", Console::eDEBUG);
            pPlayer->setBaseGameInput(spHumanPlayerInput::create(this));
        }
        // resolve CO 1 beeing set and CO 0 not
        if ((pPlayer->getCO(0) == nullptr) &&
            (pPlayer->getCO(1) != nullptr))
        {
            pPlayer->swapCOs();
        }
        // resolve random CO
        if (pPlayer->getCO(0) != nullptr && pPlayer->getCO(0)->getCoID() == CO::CO_RANDOM)
        {
            qint32 count = 0;
            QStringList perkList = pPlayer->getCO(0)->getPerkList();
            while (pPlayer->getCO(0)->getCoID() == CO::CO_RANDOM ||
                   pPlayer->getCO(0)->getCoID().startsWith("CO_EMPTY_"))
            {
                pPlayer->setCO(bannList[GlobalUtils::randInt(0, bannList.size() - 1)], 0);
                pPlayer->getCO(0)->setCoStyleFromUserdata();
                count++;
                if (count > 2000 * bannList.size())
                {
                    CONSOLE_PRINT("Unable determine random co 0 for player " + QString::number(i) + " setting co to none", Console::eDEBUG);
                    pPlayer->setCO("", 0);
                    break;
                }
            }
            if (pPlayer->getCO(0) != nullptr)
            {
                pPlayer->getCO(0)->setPerkList(perkList);
            }
        }
        if (pPlayer->getCO(0) != nullptr && singleCO)
        {
            bannList.removeAll(pPlayer->getCO(0)->getCoID());
        }
        if (pPlayer->getCO(1) != nullptr && (pPlayer->getCO(1)->getCoID() == CO::CO_RANDOM))
        {
            qint32 count = 0;
            QStringList perkList = pPlayer->getCO(1)->getPerkList();
            while ((pPlayer->getCO(1)->getCoID() == CO::CO_RANDOM) ||
                   (pPlayer->getCO(1)->getCoID() == pPlayer->getCO(0)->getCoID()) ||
                   (pPlayer->getCO(1)->getCoID().startsWith("CO_EMPTY_")))
            {
                pPlayer->setCO(bannList[GlobalUtils::randInt(0, bannList.size() - 1)], 1);
                pPlayer->getCO(1)->setCoStyleFromUserdata();
                if (count > 2000 * bannList.size())
                {
                    CONSOLE_PRINT("Unable determine random co 0 for player " + QString::number(i) + " setting co to none", Console::eDEBUG);
                    pPlayer->setCO("", 1);
                    break;
                }
            }
            if (pPlayer->getCO(1) != nullptr)
            {
                pPlayer->getCO(1)->setPerkList(perkList);
            }
        }
        if (pPlayer->getCO(1) != nullptr && singleCO)
        {
            bannList.removeAll(pPlayer->getCO(1)->getCoID());

        }
        // define army of this player
        pPlayer->defineArmy();
        pPlayer->loadVisionFields();
    }
    initPlayers();
}

void GameMap::initPlayers()
{
    m_CurrentPlayer = m_players[m_players.size() - 1];
}

void GameMap::showGrid(bool show)
{
    for (auto & sprite : m_gridSprites)
    {
        sprite->detach();
    }
    m_gridSprites.clear();
    if (show)
    {
        qint32 mapWidth = getMapWidth();
        qint32 mapHeight = getMapHeight();
        QColor gridColor = getGridColor();
        m_gridSprites.reserve(mapWidth + mapHeight + 1);
        oxygine::spColorRectSprite pActor = oxygine::spColorRectSprite::create();
        pActor->setPriority(static_cast<qint32>(Mainapp::ZOrder::GridLayout));
        for (qint32 x = 1; x < mapWidth; ++x)
        {
            oxygine::spColorRectSprite pSprite = oxygine::spColorRectSprite::create();
            pSprite->setSize(1, mapHeight * m_imagesize);
            pSprite->setColor(gridColor);
            pSprite->setPosition(x * m_imagesize, 0);
            pActor->addChild(pSprite);
            m_gridSprites.append(pSprite);
        }
        for (qint32 y = 1; y < mapHeight; ++y)
        {
            oxygine::spColorRectSprite pSprite = oxygine::spColorRectSprite::create();
            pSprite->setSize(mapWidth * m_imagesize, 1);
            pSprite->setPosition(0, y * m_imagesize);
            pSprite->setColor(gridColor);
            pSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::GridLayout));
            pActor->addChild(pSprite);
            m_gridSprites.append(pSprite);
        }
        addChild(pActor);
        m_gridSprites.append(pActor);
    }
}

void GameMap::showMiddleCrossGrid(bool show)
{
    for (auto & sprite : m_middleCrossGridSprites)
    {
        sprite->detach();
    }
    m_middleCrossGridSprites.clear();
    if (show)
    {
        float mapWidth = getMapWidth();
        float mapHeight = getMapHeight();
        QColor gridColor = getGridColor();
        oxygine::spColorRectSprite pSprite = oxygine::spColorRectSprite::create();
        pSprite->setSize(3, mapHeight * m_imagesize);
        pSprite->setColor(gridColor);
        pSprite->setPosition(mapWidth * 0.5f * m_imagesize - 1, 0);
        pSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::GridLayout));
        addChild(pSprite);
        m_middleCrossGridSprites.append(pSprite);
        pSprite = oxygine::spColorRectSprite::create();
        pSprite->setSize(mapWidth * m_imagesize, 3);
        pSprite->setPosition(0, mapHeight * 0.5f * m_imagesize - 1);
        pSprite->setColor(gridColor);
        pSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::GridLayout));
        addChild(pSprite);
        m_middleCrossGridSprites.append(pSprite);
    }
}

GameEnums::MapFilterFlags GameMap::getMapFlags() const
{
    return m_headerInfo.m_mapFlags;
}

void GameMap::setMapFlags(GameEnums::MapFilterFlags flags)
{
    m_headerInfo.m_mapFlags = flags;
}

QColor GameMap::getGridColor()
{
    QColor gridColor = QColor(70, 70, 70, 180);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue ret = pInterpreter->doFunction("Global", "getGridColor");
    if (ret.isString())
    {
        gridColor = QColor(ret.toString());
    }
    return gridColor;
}
