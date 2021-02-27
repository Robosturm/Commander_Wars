#include <QFile>

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

#include "game/terrain.h"
#include "game/unit.h"
#include "game/player.h"
#include "game/co.h"
#include "game/gameanimationfactory.h"
#include "game/gameanimationnextday.h"
#include "game/building.h"
#include "game/gamemap.h"

#include "gameinput/humanplayerinput.h"

#include "menue/gamemenue.h"

#include "objects/loadingscreen.h"


const QString GameMap::m_JavascriptName = "map";
const QString GameMap::m_GameAnimationFactory = "GameAnimationFactory";
const qint32 GameMap::frameTime = 100;
static constexpr qint32 loadingScreenSize = 900;
qint32 GameMap::m_imagesize = 24;

spGameMap GameMap::m_pInstance = nullptr;

qint32 GameMap::getFrameTime()
{
    return frameTime;
}

GameMap::GameMap(qint32 width, qint32 heigth, qint32 playerCount)
    : m_CurrentPlayer(nullptr),
      m_Rules(new GameRules())
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    m_mapAuthor = Settings::getUsername();
    loadMapData();
    newMap(width, heigth, playerCount);
    loaded = true;
}

GameMap::GameMap(QDataStream& stream)
    : m_CurrentPlayer(nullptr),
      m_Rules(new GameRules())
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    loadMapData();
    deserializeObject(stream);
    loaded = true;
}

GameMap::GameMap(QString map, bool onlyLoad, bool fast)
    : m_CurrentPlayer(nullptr),
      m_Rules(new GameRules())
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    loadMapData();
    QFile file(map);
    file.open(QIODevice::ReadOnly);
    QDataStream pStream(&file);
    deserializer(pStream, fast);
    setMapNameFromFilename(map);
    loaded = true;
    if (!onlyLoad)
    {
        updateSprites();
        qint32 heigth = getMapHeight();
        qint32 width = getMapWidth();
        centerMap(width / 2, heigth / 2);
    }
}

void GameMap::setMapNameFromFilename(QString filename)
{
    if (m_mapName.isEmpty())
    {
        QStringList items = filename.split("/");
        if (items.size() > 0)
        {
            items = items[items.size() - 1].split(".");
            if (items.size() > 0)
            {
                m_mapName = items[0];
            }
        }
    }
}

void GameMap::loadMapData()
{
    deleteMap();
    m_pInstance = this;
    Interpreter::setCppOwnerShip(this);
    registerMapAtInterpreter();
    m_zoom = 2.0f;
    setScale(m_zoom);
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Map));
}

void GameMap::registerMapAtInterpreter()
{

    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(m_JavascriptName, pInterpreter->newQObject(this));
    pInterpreter->setGlobal(m_GameAnimationFactory, pInterpreter->newQObject(GameAnimationFactory::getInstance()));
}

qint32 GameMap::getUniqueIdCounter()
{
    m_UniqueIdCounter++;
    // gurantee that the counter is never 0
    if (m_UniqueIdCounter == 0)
    {
        m_UniqueIdCounter++;
    }
    return m_UniqueIdCounter;
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
    for (const auto & player : players)
    {
        if (!player->getIsDefeated())
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
        QVector<QRect> m_LineSight;
        QVector<QRect> m_LineSightEvaluated;
        m_LineSight.append(QRect(x - 1, y, 0, 2));
        m_LineSight.append(QRect(x - 1, y, 0, 3));
        m_LineSight.append(QRect(x + 1, y, 1, 2));
        m_LineSight.append(QRect(x + 1, y, 1, 3));
        m_LineSight.append(QRect(x, y - 1, 2, 0));
        m_LineSight.append(QRect(x, y - 1, 2, 1));
        m_LineSight.append(QRect(x, y + 1, 3, 0));
        m_LineSight.append(QRect(x, y + 1, 3, 1));

        QPoint pos(x, y);
        if (0 >= minVisionRange && 0 <= maxVisionRange)
        {
            pRet->append(QPoint(0, 0));
        }
        while (m_LineSight.size() > 0)
        {
            QRect current = m_LineSight.front();
            m_LineSight.pop_front();
            m_LineSightEvaluated.append(current);
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
                                    m_LineSight.append(next);
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

bool GameMap::isPlayersUnitInArea(const QRect& area, QList<qint32> playerIDs)
{
    return isInArea(area, [=](Unit* pUnit)
    {
        qint32 owner = pUnit->getOwner()->getPlayerID();
        return playerIDs.contains(owner);
    });
}

void GameMap::deleteMap()
{
    Console::print("deleteMap()", Console::eDEBUG);
    if (m_pInstance.get() != nullptr)
    {
        m_pInstance->detach();
    }
    m_pInstance = nullptr;
}

GameMap::~GameMap()
{
    Console::print("desctructing map.", Console::eDEBUG);
    // remove us from the interpreter again
    if (GameMap::getInstance() == nullptr)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->deleteObject(m_JavascriptName);
    }
    // clean up session
    for (qint32 y = 0; y < fields.size(); y++)
    {
        fields[y].clear();
    }
    fields.clear();
}

QStringList GameMap::getAllUnitIDs()
{
    return UnitSpriteManager::getInstance()->getLoadedRessources();
}

spGameAction GameMap::createAction()
{
    return new GameAction();
}

void GameMap::queueAction(spGameAction pAction)
{
    emit sigQueueAction(pAction);
}

spTerrain GameMap::getSpTerrain(qint32 x, qint32 y)
{
    if (onMap(x, y))
    {
        return fields[y][x];
    }
    else
    {
        return nullptr;
    }
}

Terrain* GameMap::getTerrain(qint32 x, qint32 y)
{
    if (onMap(x, y))
    {
        return fields[y][x].get();
    }
    else
    {
        return nullptr;
    }
}

spPlayer GameMap::getspPlayer(qint32 player)
{
    if (player >= 0 && player < players.size())
    {
        return players[player];
    }
    else
    {
        return nullptr;
    }
}

Player* GameMap::getPlayer(qint32 player)
{
    if (player >= 0 && player < players.size())
    {
        return players[player].get();
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
    if ((player >= 0) && (player < players.size()))
    {
        m_CurrentPlayer = players[player];
    }
}

void GameMap::updateSprites(qint32 xInput, qint32 yInput, bool editor, bool showLoadingScreen)
{
    Console::print("Update Sprites x=" + QString::number(xInput) + " y=" + QString::number(yInput), Console::eDEBUG);

    LoadingScreen* pLoadingScreen = LoadingScreen::getInstance();
    if (showLoadingScreen)
    {
        pLoadingScreen->show();
        pLoadingScreen->setProgress("Loading Map Sprites", 0);
    }
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    if ((xInput < 0) && (yInput < 0))
    {
        // update terrain sprites
        for (qint32 y = 0; y < heigth; y++)
        {
            if (showLoadingScreen)
            {
                pLoadingScreen->setProgress(QObject::tr("Loading Map Row ") + QString::number(y) + QObject::tr(" of ") + QString::number(heigth), 0 + 50 * y / heigth);
            }
            for (qint32 x = 0; x < width; x++)
            {
                fields[y][x]->loadSprites();
                if (fields[y][x]->getUnit() != nullptr)
                {
                    fields[y][x]->getUnit()->updateSprites(editor);
                }
                if (fields[y][x]->getBuilding() != nullptr)
                {
                    fields[y][x]->getBuilding()->updateBuildingSprites(false);
                }
                
            }
        }
    }
    else
    {
        // more optimized for a single terrain :)
        for (qint32 y = yInput -1; y <= yInput + 1; y++)
        {
            for (qint32 x = xInput -1; x <= xInput + 1; x++)
            {
                if (onMap(x, y))
                {
                    fields[y][x]->loadSprites();
                    if (fields[y][x]->getUnit() != nullptr)
                    {
                        fields[y][x]->getUnit()->updateSprites(editor);
                    }
                    if (fields[y][x]->getBuilding() != nullptr)
                    {
                        fields[y][x]->getBuilding()->updateBuildingSprites(false);
                    }
                    
                }
            }
        }
    }

    Console::print("synchronizing animations", Console::eDEBUG);
    for (qint32 y = 0; y < heigth; y++)
    {
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Synchronizing Map Row ") + QString::number(y) + tr(" of ") + QString::number(heigth), 50 + 40 * y / heigth);
        }
        for (qint32 x = 0; x < width; x++)
        {
            fields[y][x]->syncAnimation();
        }
    }
    
    if (showLoadingScreen)
    {
        pLoadingScreen->setProgress(tr("Loading weather for snowy times"), 95);
    }
    if (m_Rules.get() != nullptr)
    {
        m_Rules->createWeatherSprites();
    }
    if (showLoadingScreen)
    {
        pLoadingScreen->hide();
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
            Unit* pUnit = fields[y][x]->getUnit();
            if (pUnit != nullptr &&
                pUnit->getHp() <= 0)
            {
                fields[y][x]->setUnit(nullptr);
            }
        }
    }
    
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
    players.removeAt(index);
}

Unit* GameMap::spawnUnit(qint32 x, qint32 y, QString unitID, Player* owner, qint32 range)
{
    Console::print("spawning Unit", Console::eDEBUG);
    if (owner != nullptr)
    {
        qint32 heigth = getMapHeight();
        qint32 width = getMapWidth();
        if (range < 0)
        {
            range = width + heigth;
        }
        spPlayer pPlayer = nullptr;
        for (qint32 i = 0; i < players.size(); i++)
        {
            if (owner == players[i].get())
            {
                pPlayer = players[i];
                break;
            }
            else if (i == players.size() - 1)
            {
                // cancel since we have no owner for the unit
                Console::print("Invalid player selected. Didn't spawn unit " + unitID, Console::eERROR);
                return nullptr;
            }
        }

        qint32 unitLimit = m_Rules->getUnitLimit();
        qint32 unitCount = pPlayer->getUnitCount();
        if (unitLimit > 0 && unitCount >= unitLimit)
        {
            Console::print("Didn't spawn unit " + unitID + " cause unit limit is reached", Console::eDEBUG);
            return nullptr;
        }
        spUnit pUnit = new Unit(unitID, pPlayer.get(), true);
        MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
        QString movementType = pUnit->getMovementType();
        if (onMap(x, y))
        {
            spTerrain pTerrain = getTerrain(x, y);
            if (pTerrain.get() != nullptr &&
                (pTerrain->getUnit() == nullptr) &&
                (pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
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
                    spTerrain pTerrain = getTerrain(x + x2 - currentRadius, y + y2);
                    if (pTerrain.get() != nullptr &&
                        (pTerrain->getUnit() == nullptr) &&
                        (pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
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
                    spTerrain pTerrain = getTerrain(x + x2, y + y2);
                    if ((pTerrain.get() != nullptr &&
                         pTerrain->getUnit() == nullptr) &&
                        (pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
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
                    spTerrain pTerrain = getTerrain(x + x2, y + y2);
                    if ((pTerrain.get() != nullptr &&
                         pTerrain->getUnit() == nullptr) &&
                        (pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
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
                    spTerrain pTerrain = getTerrain(x + x2, y + y2);
                    if ((pTerrain.get() != nullptr &&
                         pTerrain->getUnit() == nullptr) &&
                        (pMovementTableManager->getBaseMovementPoints(movementType, pTerrain.get(), pTerrain.get(), pUnit.get()) > 0))
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
    Console::print("Didn't spawn unit " + unitID + " didn't find an empty field", Console::eDEBUG);
    return nullptr;
}

qint32 GameMap::getMapWidth() const
{
    if (fields.size() > 0)
    {
        return fields[0].size();
    }
    return 0;
}

qint32 GameMap::getMapHeight() const
{
    return fields.size();
}

qint32 GameMap::getBuildingCount(QString buildingID)
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
                    if (pBuilding->getX() == x && pBuilding->getY() == y)
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

void GameMap::centerMap(qint32 x, qint32 y)
{
    if (onMap(x, y))
    {
        // draw point
        this->setPosition(Settings::getWidth() / 2.0f - x * this->getZoom() * m_imagesize - m_imagesize / 2.0f,
                          Settings::getHeight() / 2.0f - y * this->getZoom() * m_imagesize - m_imagesize / 2.0f);
    }
}

void GameMap::moveMap(qint32 x, qint32 y)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    // draw point
    float resX = this->getPosition().x + x;
    float resY = this->getPosition().y + y;
    float minVisible = 16.0f / m_zoom;
    if (resX > Settings::getWidth()  - minVisible * m_zoom * m_imagesize)
    {
        resX = Settings::getWidth() - minVisible * m_zoom * m_imagesize;
    }
    if (resX < -m_zoom * m_imagesize * width + minVisible * m_zoom * m_imagesize)
    {
        resX = -m_zoom * m_imagesize * width + minVisible * m_zoom * m_imagesize;
    }
    if (resY > Settings::getHeight() - minVisible * m_zoom * m_imagesize)
    {
        resY = Settings::getHeight() - minVisible * m_zoom * m_imagesize;
    }
    if (resY < -m_zoom * m_imagesize * heigth + minVisible * m_zoom * m_imagesize)
    {
        resY = -m_zoom * m_imagesize * heigth + minVisible * m_zoom * m_imagesize;
    }

    this->setPosition(resX, resY);
}

void GameMap::zoom(float zoom)
{
    if (zoom > 0)
    {
        m_zoom *= 2.0f;
    }
    else
    {
        m_zoom /= 2.0f;
    }
    // limit zoom
    if (m_zoom > 16.0f)
    {
        m_zoom = 16.0f;
    }
    else if (m_zoom < 0.5f)
    {
        m_zoom = 0.5f;
    }
    else
    {
        // all fine
    }
    this->setScale(m_zoom);
    Interpreter::getInstance()->doFunction("onZoomLevelChanged");
}

void GameMap::replaceTerrainOnly(QString terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain)
{
    if (onMap(x, y))
    {
        spTerrain pTerrainOld = fields[y][x];
        if (pTerrainOld->getTerrainID() != terrainID)
        {
            pTerrainOld->removeBuilding();
            pTerrainOld->setUnit(nullptr);

            spTerrain pTerrain = Terrain::createTerrain(terrainID, x, y, pTerrainOld->getBaseTerrainID());

            Interpreter* pInterpreter = Interpreter::getInstance();
            QString function1 = "useTerrainAsBaseTerrain";
            QJSValue terrainAsBaseTerrain = pInterpreter->doFunction(terrainID, function1);

            if (useTerrainAsBaseTerrain && terrainAsBaseTerrain.toBool() && canBePlaced(terrainID, x, y))
            {
                pTerrainOld->detach();
                pTerrain->setBaseTerrain(pTerrainOld);
                fields[y].replace(x, pTerrain);
                this->addChild(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
                pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + static_cast<qint32>(y));
            }
            else
            {
                pTerrainOld->detach();
                fields[y].replace(x, pTerrain);
                this->addChild(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
                pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + static_cast<qint32>(y));
            }
        }
        else
        {
            pTerrainOld->setBuilding(nullptr);
        }
    }
}

void GameMap::replaceTerrain(QString terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain, bool updateSprites)
{
    replaceTerrainOnly(terrainID, x, y, useTerrainAsBaseTerrain);
    updateTerrain(x, y);
    if (updateSprites)
    {
        this->updateSprites(x, y);
    }
}

void GameMap::replaceBuilding(QString buildingID, qint32 x, qint32 y)
{
    if (onMap(x, y))
    {
        spBuilding pBuilding = new Building(buildingID);
        Terrain* pTerrain = getTerrain(x, y);
        if (pBuilding->canBuildingBePlaced(pTerrain))
        {
            pTerrain->setBuilding(pBuilding.get());
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
                    if (!canBePlaced(fields[yPos][xPos]->getTerrainID(), xPos, yPos))
                    {
                        replaceTerrain(fields[yPos][xPos]->getBaseTerrainID(), xPos, yPos, false, true);
                    }
                }
            }
        }
    }
}

bool GameMap::canBePlaced(QString terrainID, qint32 x, qint32 y)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "canBePlaced";
    QJSValueList args;
    args << QJSValue(x);
    args << QJSValue(y);
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
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    // store header
    pStream << getVersion();
    pStream << m_mapName;
    pStream << m_mapAuthor;
    pStream << m_mapDescription;
    pStream << width;
    pStream << heigth;
    pStream << m_UniqueIdCounter;
    pStream << getPlayerCount();
    qint32 currentPlayerIdx = 0;
    for (qint32 i = 0; i < players.size(); i++)
    {
        if (m_CurrentPlayer.get() == players[i].get())
        {
            currentPlayerIdx = i;
        }
        players[i]->serializeObject(pStream);
    }


    pStream << currentPlayerIdx;
    pStream << currentDay;
    // store map
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            // serialize
            fields[y][x]->serializeObject(pStream);
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
}

void GameMap::readMapHeader(QDataStream& pStream,
                            qint32 & version, QString & mapName,  QString & mapAuthor, QString & mapDescription,
                            qint32 & width, qint32 & heigth, qint32 & playerCount, qint32 & uniqueIdCounter)
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pStream >> version;
    if (version > 1)
    {
        pStream >> mapName;
    }
    if (version > 4)
    {
        pStream >> mapAuthor;
        pStream >> mapDescription;
    }
    pStream >> width;
    pStream >> heigth;
    if (version > 6)
    {
        pStream >> uniqueIdCounter;
    }
    else
    {
        uniqueIdCounter = 0;
    }
    pStream >> playerCount;
}

void GameMap::deserializer(QDataStream& pStream, bool fast)
{
    clearMap();
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    // restore map header
    qint32 version = 0;
    qint32 heigth = 0;
    qint32 width = 0;
    qint32 playerCount = 0;
    readMapHeader(pStream, version, m_mapName, m_mapAuthor, m_mapDescription,
                  width, heigth, playerCount, m_UniqueIdCounter);
    Console::print("Loading map " + m_mapName + " Fast =" + (fast ? "true" : "false"), Console::eDEBUG);
    qint32 mapSize = width * heigth;
    bool showLoadingScreen = (mapSize >= loadingScreenSize) && !fast;
    if (showLoadingScreen)
    {
        pLoadingScreen->show();
    }
    if (showLoadingScreen)
    {
        pLoadingScreen->setProgress(QObject::tr("Loading Players"), 5);
    }
    for (qint32 i = 0; i < playerCount; i++)
    {
        // create new player
        players.append(new Player());
        // get player data from stream
        players[i]->deserializer(pStream, fast);
    }

    qint32 currentPlayerIdx = 0;
    if (version > 1)
    {
        pStream >> currentPlayerIdx;
        pStream >> currentDay;
    }

    // restore map
    for (qint32 y = 0; y < heigth; y++)
    {
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Loading Map Row ") + QString::number(y) + tr(" of ") + QString::number(heigth), 5 + 75 * y / heigth);
        }
        fields.append(QVector<spTerrain>());
        for (qint32 x = 0; x < width; x++)
        {
            spTerrain pTerrain = Terrain::createTerrain("", x, y, "");
            fields[y].append(pTerrain);
            pTerrain->deserializer(pStream, fast);
            if (pTerrain->isValid())
            {
                if (!fast)
                {
                    this->addChild(pTerrain);
                    pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
                    pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + static_cast<qint32>(y));
                }
            }
            else
            {
                replaceTerrain("PLAINS", x, y, false, false);
            }
        }
    }
    setCurrentPlayer(currentPlayerIdx);
    m_Rules = new  GameRules();
    if (showLoadingScreen)
    {
        pLoadingScreen->setProgress(tr("Loading Rules"), 80);
    }
    if (version > 2)
    {
        m_Rules->deserializer(pStream, fast);
    }
    if (!fast)
    {
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Loading Record"), 85);
        }
        if (version > 3)
        {
            m_Recorder->deserializeObject(pStream);
        }
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Loading scripts"), 90);
        }
        if (version > 5)
        {
            m_GameScript->deserializeObject(pStream);
        }
        else
        {
            m_GameScript = new GameScript();
        }
        if (showLoadingScreen)
        {
            pLoadingScreen->setProgress(tr("Loading Campaign"), 95);
        }
        if (version > 7)
        {
            bool exists = false;
            pStream >> exists;
            if (exists)
            {
                m_Campaign = new Campaign();
                m_Campaign->deserializeObject(pStream);
            }
        }
        if (version > 8)
        {
            pStream >> m_mapPath;
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
    emit signalVictoryInfo();
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

void GameMap::startGame()
{
    m_Recorder = new GameRecorder();
    for (qint32 y = 0; y < fields.size(); y++)
    {
        for (qint32 x = 0; x < fields[y].size(); x++)
        {
            Unit* pUnit = fields[y][x]->getUnit();
            if (pUnit != nullptr)
            {
                pUnit->applyMod();
            }
        }
    }
    Userdata* pUserdata = Userdata::getInstance();
    auto lockedUnits = pUserdata->getItemsList(GameEnums::ShopItemType_Unit, true);

    for (qint32 i = 0; i < players.size(); i++)
    {
        players[i]->loadVisionFields();
        players[i]->setBuildlistChanged(true);

        CoreAI* pAI = dynamic_cast<CoreAI*>(players[i]->getBaseGameInput());
        if (pAI != nullptr)
        {
            pAI->setEnableNeutralTerrainAttack(m_Rules->getAiAttackTerrain());
        }
        HumanPlayerInput* pHuman = dynamic_cast<HumanPlayerInput*>(players[i]->getBaseGameInput());
        if (pHuman != nullptr)
        {
            auto buildList = players[i]->getBuildList();
            for (const auto & unitId : lockedUnits)
            {
                buildList.removeAll(unitId);
            }
            players[i]->setBuildList(buildList);
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
}

void GameMap::clearMap()
{
    for (qint32 y = 0; y < fields.size(); y++)
    {
        for (qint32 x = 0; x < fields[y].size(); x++)
        {
            Unit* pUnit = fields[y][x]->getUnit();
            if (pUnit != nullptr)
            {
                pUnit->detach();
            }
            fields[y][x]->detach();
        }
        fields[y].clear();
    }
    fields.clear();
    players.clear();
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
            spUnit pUnit = fields[y][x]->getSpUnit();
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
    for (qint32 i = start + 1; i < players.size(); i++)
    {
        m_CurrentPlayer = players[i];
        if (!m_CurrentPlayer->getIsDefeated())
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        nextDay = true;
        currentDay++;
        for (qint32 i = 0; i < players.size(); i++)
        {
            m_CurrentPlayer = players[i];
            if (!m_CurrentPlayer->getIsDefeated())
            {
                break;
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
            spUnit pUnit = fields[y][x]->getSpUnit();
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
    for (qint32 i = 0; i < players.size(); i++)
    {
        if (!players[i]->getIsDefeated())
        {
            if (winnerTeam >= 0 &&
                winnerTeam != players[i]->getTeam())
            {
                winnerTeam = -1;
                break;
            }
            else if (winnerTeam < 0)
            {
                winnerTeam = players[i]->getTeam();
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

QString GameMap::getMapDescription() const
{
    return m_mapDescription;
}

void GameMap::setMapDescription(const QString &value)
{
    m_mapDescription = value;
}

QString GameMap::getMapAuthor() const
{
    return m_mapAuthor;
}

void GameMap::setMapAuthor(const QString &value)
{
    m_mapAuthor = value;
}

qint32 GameMap::getCurrentDay() const
{
    return currentDay;
}

void GameMap::refillAll()
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = fields[y][x]->getSpUnit();
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
    spGameMenue pMenue = GameMenue::getInstance();
    if (pMenue.get() != nullptr)
    {
        return pMenue->getCurrentViewPlayer();
    }
    return getCurrentPlayer();
}

void GameMap::startOfTurn(Player* pPlayer)
{    
    if (pPlayer != nullptr)
    {
        pPlayer->startOfTurn();
        pPlayer->getBaseGameInput()->centerCameraOnAction(nullptr);
        startOfTurnPlayer(pPlayer);
    }
    else
    {
        startOfTurnNeutral();
    }
}

void GameMap::startOfTurnNeutral()
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spTerrain pTerrain = fields[y][x];
            pTerrain->startOfTurn();
            spBuilding pBuilding = pTerrain->getSpBuilding();
            if (pBuilding.get() != nullptr &&
                pBuilding->getOwner() == nullptr &&
                (pBuilding->getX() == x &&
                 pBuilding->getY() == y))
            {
                pBuilding->startOfTurn();
            }
        }
    }
}

void GameMap::startOfTurnPlayer(Player* pPlayer)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = fields[y][x]->getSpUnit();
            if (pUnit.get() != nullptr)
            {
                if (pUnit->getOwner() == pPlayer)
                {
                    pUnit->removeShineTween();
                    pUnit->startOfTurn();
                }
                pUnit->updateIcons(getCurrentViewPlayer());
            }
            spBuilding pBuilding = fields[y][x]->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->getOwner() == pPlayer &&
                    (pBuilding->getX() == x && pBuilding->getY() == y))
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
            spUnit pUnit = fields[y][x]->getSpUnit();
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
            spBuilding pBuilding = fields[y][x]->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->getOwner() == pPlayer &&
                    (pBuilding->getX() == x && pBuilding->getY() == y))
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
            spUnit pUnit = fields[y][x]->getSpUnit();
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
            Unit* pUnit = fields[y][x]->getUnit();
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
            spUnit pUnit = fields[y][x]->getSpUnit();
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

QmlVectorBuilding* GameMap::getBuildings(Player* pPlayer)
{
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    QmlVectorBuilding* ret = new QmlVectorBuilding();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spBuilding pBuilding = fields[y][x]->getSpBuilding();
            if (pBuilding.get() != nullptr && pBuilding->getTerrain() == fields[y][x].get())
            {
                if ((pBuilding->getOwner() == pPlayer))
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
    return m_mapName;
}

void GameMap::setMapName(const QString &value)
{
    m_mapName = value;
}

void GameMap::nextTurnPlayerTimeout()
{
    if (m_CurrentPlayer->getBaseGameInput()->getAiType() != GameEnums::AiTypes_ProxyAi)
    {
        nextTurn();
    }
}

void GameMap::nextTurn()
{
    m_Rules->checkVictory();
    enableUnits(m_CurrentPlayer.get());
    Mainapp* pApp = Mainapp::getInstance();
    pApp->getAudioThread()->clearPlayList();
    bool nextDay = nextPlayer();
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
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        pMenu->updatePlayerinfo();
    }


    m_CurrentPlayer->loadCOMusic();
    pApp->getAudioThread()->playRandom();
    bool permanent = false;
    bool found = false;
    if ((m_Rules->getDayToDayScreen() == GameRules::DayToDayScreen::Permanent ||
         m_Rules->getFogMode() != GameEnums::Fog::Fog_Off) &&
        m_CurrentPlayer->getBaseGameInput() != nullptr)
    {
        if (m_CurrentPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
        {
            // search for previous player
            qint32 currentPlayerID = m_CurrentPlayer->getPlayerID();
            for (qint32 i = currentPlayerID - 1; i >= 0; i--)
            {
                if (players[i]->getBaseGameInput() != nullptr &&
                    players[i]->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human &&
                    !players[i]->getIsDefeated())
                {
                    if (players[i]->getTeam() != m_CurrentPlayer->getTeam())
                    {
                        permanent = true;
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                for (qint32 i = players.size() - 1; i > currentPlayerID; i--)
                {
                    if (players[i]->getBaseGameInput() != nullptr &&
                        players[i]->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
                    {
                        if (players[i]->getTeam() != m_CurrentPlayer->getTeam())
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
        spGameMenue pMenu = GameMenue::getInstance();
        if (pMenu.get() != nullptr)
        {
            GameAnimationNextDay* pAnim = new GameAnimationNextDay(m_CurrentPlayer.get(), GameMap::frameTime, true);
            pMenu->addChild(pAnim);
        }
    }
    else
    {
        GameAnimationFactory::createGameAnimationNextDay(m_CurrentPlayer.get());
    }
}

void GameMap::initPlayersAndSelectCOs()
{
    QStringList usedCOs;
    bool singleCO = m_Rules->getSingleRandomCO();
    for (qint32 i = 0; i < getPlayerCount(); i++)
    {
        Player* pPlayer = GameMap::getInstance()->getPlayer(i);
        if (pPlayer->getCO(0) != nullptr)
        {
            usedCOs.append(pPlayer->getCO(0)->getCoID());
        }
        if (pPlayer->getCO(1) != nullptr)
        {
            usedCOs.append(pPlayer->getCO(1)->getCoID());
        }
    }
    QStringList bannList = m_Rules->getCOBannlist();
    Userdata* pUserdata = Userdata::getInstance();
    auto items = pUserdata->getItemsList(GameEnums::ShopItemType_CO, false);
    for (const auto & item : items)
    {
        bannList.removeAll(item);
    }
    // fix some stuff for the players based on our current input
    for (qint32 i = 0; i < getPlayerCount(); i++)
    {
        Player* pPlayer = GameMap::getInstance()->getPlayer(i);
        if (pPlayer->getBaseGameInput() == nullptr)
        {
            Console::print("Forcing AI for player " + QString::number(i) + " to human.", Console::eDEBUG);
            pPlayer->setBaseGameInput(new HumanPlayerInput());
        }
        // resolve CO 1 beeing set and CO 0 not
        if ((pPlayer->getCO(0) == nullptr) &&
            (pPlayer->getCO(1) != nullptr))
        {
            pPlayer->swapCOs();
        }
        // resolve random CO
        if (pPlayer->getCO(0) != nullptr && pPlayer->getCO(0)->getCoID() == "CO_RANDOM")
        {
            qint32 count = 0;
            QStringList perkList = pPlayer->getCO(0)->getPerkList();
            while (pPlayer->getCO(0)->getCoID() == "CO_RANDOM" || pPlayer->getCO(0)->getCoID().startsWith("CO_EMPTY_") ||
                   (singleCO && usedCOs.contains(pPlayer->getCO(0)->getCoID())))
            {
                pPlayer->setCO(bannList[GlobalUtils::randInt(0, bannList.size() - 1)], 0);
                pPlayer->getCO(0)->setCoStyleFromUserdata();
                count++;
                if (count > 2000 * bannList.size())
                {
                    Console::print("Unable determine random co 0 for player " + QString::number(i) + " setting co to none", Console::eDEBUG);
                    pPlayer->setCO("", 0);
                    break;
                }
            }
            if (pPlayer->getCO(0) != nullptr)
            {
                pPlayer->getCO(0)->setPerkList(perkList);
            }
        }
        if (pPlayer->getCO(0) != nullptr)
        {
            usedCOs.append(pPlayer->getCO(0)->getCoID());
        }
        if (pPlayer->getCO(1) != nullptr && (pPlayer->getCO(1)->getCoID() == "CO_RANDOM"))
        {
            QStringList bannList = m_Rules->getCOBannlist();
            qint32 count = 0;
            QStringList perkList = pPlayer->getCO(1)->getPerkList();
            while ((pPlayer->getCO(1)->getCoID() == "CO_RANDOM") ||
                   (pPlayer->getCO(1)->getCoID() == pPlayer->getCO(0)->getCoID()) ||
                   (pPlayer->getCO(1)->getCoID().startsWith("CO_EMPTY_")) ||
                   (singleCO && usedCOs.contains(pPlayer->getCO(1)->getCoID())))
            {
                pPlayer->setCO(bannList[GlobalUtils::randInt(0, bannList.size() - 1)], 1);
                pPlayer->getCO(1)->setCoStyleFromUserdata();
                if (count > 2000 * bannList.size())
                {
                    Console::print("Unable determine random co 0 for player " + QString::number(i) + " setting co to none", Console::eDEBUG);
                    pPlayer->setCO("", 1);
                    break;
                }
            }
            if (pPlayer->getCO(1) != nullptr)
            {
                pPlayer->getCO(1)->setPerkList(perkList);
            }
        }
        if (pPlayer->getCO(1) != nullptr)
        {
            usedCOs.append(pPlayer->getCO(1)->getCoID());
        }
        // define army of this player
        pPlayer->defineArmy();
        pPlayer->loadVisionFields();
    }
    initPlayers();
}

void GameMap::initPlayers()
{
    m_CurrentPlayer = players[players.size() - 1];
}
