#include <QFile>

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/movementtablemanager.h"
#include "resource_management/gamemanager.h"

#include "game/terrain.h"

#include "game/unit.h"

#include "game/player.h"

#include "game/gameanimationfactory.h"

const QString GameMap::m_JavascriptName = "map";
const QString GameMap::m_GameAnimationFactory = "GameAnimationFactory";
const qint32 GameMap::frameTime = 125;
GameMap* GameMap::m_pInstance = nullptr;



GameMap::GameMap(qint32 width, qint32 heigth)
    : width(width),
      heigth(heigth)
{
    loadMapData();

    for (qint32 y = 0; y < heigth; y++)
    {
        fields.append(new QVector<spTerrain>());
        for (qint32 x = 0; x < width; x++)
        {
            spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y);
            this->addChild(pTerrain);
            fields[y]->append(pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        }
    }
    updateTerrainSprites();
    centerMap(width / 2, heigth / 2);
    // add two players to a default map :)
    for (quint32 i = 0; i < 2; i++)
    {
        players.append(new Player(i));
    }
}


GameMap::GameMap(QString map, bool gamestart)
{
    loadMapData();
    QFile file(map);
    file.open(QIODevice::ReadOnly);
    QDataStream pStream(&file);
    deserialize(pStream);


}

void GameMap::loadMapData()
{
    m_pInstance = this;
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    pInterpreter->setGlobal(m_JavascriptName, pInterpreter->newQObject(this));
    pInterpreter->setGlobal(m_GameAnimationFactory, pInterpreter->newQObject(GameAnimationFactory::getInstance()));

    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    pTerrainManager->loadAll();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    pBuildingSpriteManager->loadAll();
    UnitSpriteManager* pUnitspritemanager = UnitSpriteManager::getInstance();
    pUnitspritemanager->loadAll();
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    pMovementTableManager->loadAll();
    GameManager* pGameManager = GameManager::getInstance();
    pGameManager->loadAll();
}

GameMap::~GameMap()
{
    m_pInstance = nullptr;
    // remove us from the interpreter again
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    pInterpreter->deleteObject(m_JavascriptName);
    // clean up session
    for (qint32 y = 0; y < heigth; y++)
    {
        //
        fields.at(y)->clear();
        delete fields.at(y);
    }
    fields.clear();
}

spTerrain GameMap::getSpTerrain(qint32 x, qint32 y)
{
    if (onMap(x, y))
    {
        return fields.at(y)->at(x);
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
        return fields.at(y)->at(x).get();
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

void GameMap::updateTerrainSprites(qint32 xInput, qint32 yInput)
{
    if ((xInput < 0) && (yInput < 0))
    {
        // update terrain sprites
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                fields.at(y)->at(x)->loadSprites();
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
                    fields.at(y)->at(x)->loadSprites();
                }
            }
        }
    }
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            fields.at(y)->at(x)->syncAnimation();
        }
    }
}

qint32 GameMap::getMapWidth() const
{
    return width;
}

qint32 GameMap::getMapHeight() const
{
    return heigth;
}

void GameMap::getField(qint32& x, qint32& y, Directions direction)
{
    switch (direction)
    {
        case GameMap::Directions::North:
        {
            y--;
            break;
        }
        case GameMap::Directions::NorthEast:
        {
            x++;
            y--;
            break;
        }
        case GameMap::Directions::East:
        {
            x++;
            break;
        }
        case GameMap::Directions::SouthEast:
        {
            y++;
            x++;
            break;
        }
        case GameMap::Directions::South:
        {
            y++;
            break;
        }
        case GameMap::Directions::SouthWest:
        {
            y++;
            x--;
            break;
        }
        case GameMap::Directions::West:
        {
            x--;
            break;
        }
        case GameMap::Directions::NorthWest:
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
    Mainapp* pApp = Mainapp::getInstance();
    // draw point
    this->setPosition(pApp->getSettings()->getWidth() / 2.0f - x * this->getZoom() * Imagesize - Imagesize / 2.0f,
                      pApp->getSettings()->getHeight() / 2.0f - y * this->getZoom() * Imagesize - Imagesize / 2.0f);
}

void GameMap::moveMap(qint32 x, qint32 y)
{
    // draw point
    qint32 resX = this->getPosition().x + x;
    qint32 resY = this->getPosition().y + y;
    qint32 minVisible = 16 / m_zoom;
    Mainapp* pApp = Mainapp::getInstance();
    if (resX > pApp->getSettings()->getWidth()  - minVisible * m_zoom * Imagesize)
    {
        resX = pApp->getSettings()->getWidth() - minVisible * m_zoom * Imagesize;
    }
    if (resX < -m_zoom * Imagesize * width + minVisible * m_zoom * Imagesize)
    {
        resX = -m_zoom * Imagesize * width + minVisible * m_zoom * Imagesize;
    }
    if (resY > pApp->getSettings()->getHeight() - minVisible * m_zoom * Imagesize)
    {
        resY = pApp->getSettings()->getHeight() - minVisible * m_zoom * Imagesize;
    }
    if (resY < -m_zoom * Imagesize * heigth + minVisible * m_zoom * Imagesize)
    {
        resY = -m_zoom * Imagesize * heigth + minVisible * m_zoom * Imagesize;
    }

    this->setPosition(resX, resY);
}

void GameMap::zoom(float zoom)
{
    m_zoom += zoom * 0.125;
    // limit zoom
    if (m_zoom > 4.0f)
    {
        m_zoom = 4.0f;
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
    moveMap(0, 0);
}

void GameMap::replaceTerrain(const QString& terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain, bool updateSprites)
{
    if (onMap(x, y))
    {
        spTerrain pTerrain = Terrain::createTerrain(terrainID, x, y);
        if (useTerrainAsBaseTerrain)
        {
            spTerrain pTerrainOld = fields.at(y)->at(x);
            this->removeChild(pTerrainOld);
            pTerrain->setBaseTerrain(pTerrainOld);
            fields.at(y)->replace(x, pTerrain);
            this->addChild(pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        }
        else
        {
            spTerrain pTerrainOld = fields.at(y)->at(x);
            this->removeChild(pTerrainOld);
            fields.at(y)->replace(x, pTerrain);
            this->addChild(pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        }
        if (updateSprites)
        {
            updateTerrain(x, y);
            updateTerrainSprites(x, y);
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
                    if (!canBePlaced(fields.at(yPos)->at(xPos)->getTerrainID(), xPos, yPos))
                    {
                        replaceTerrain(fields.at(yPos)->at(xPos)->getBaseTerrainID(), xPos, yPos, false, true);
                    }
                }
            }
        }
    }
}

bool GameMap::canBePlaced(const QString& terrainID, qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function = "canBePlaced";
    QJSValueList args;
    args << QJSValue(x);
    args << QJSValue(y);
    QJSValue placeable = pApp->getInterpreter()->doFunction(terrainID, function, args);
    if (placeable.isBool())
    {
        return placeable.toBool();
    }
    else
    {
        return false;
    }
}

void GameMap::serialize(QDataStream& pStream)
{
    // store header
    pStream << getVersion();
    pStream << width;
    pStream << heigth;
    pStream << getPlayerCount();
    for (qint32 i = 0; i < players.size(); i++)
    {
        players[i]->serialize(pStream);
    }

    // store map
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            // serialize
            fields.at(y)->at(x)->serialize(pStream);
        }
    }
}

void GameMap::deserialize(QDataStream& pStream)
{
    // delete all data
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            this->removeChild(fields.at(y)->at(x));
        }
        fields.at(y)->clear();
    }
    fields.clear();
    players.clear();

    // restore map header
    qint32 version = 0;
    pStream >> version;
    pStream >> width;
    pStream >> heigth;
    qint32 playerCount = 0;
    pStream >> playerCount;
    for (qint32 i = 0; i < playerCount; i++)
    {
        // create new player
        players.append(new Player(i));
        // get player data from stream
        players[i]->deserialize(pStream);
    }

    // restore map
    for (qint32 y = 0; y < heigth; y++)
    {
        fields.append(new QVector<spTerrain>());
        for (qint32 x = 0; x < width; x++)
        {
            spTerrain pTerrain = Terrain::createTerrain("", x, y);
            fields[y]->append(pTerrain);
            pTerrain->deserialize(pStream);
            this->addChild(pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        }
    }
    updateTerrainSprites();
    centerMap(width / 2, heigth / 2);
}

qint32 GameMap::getImageSize()
{
    return Imagesize;
}

void GameMap::enableUnits(Player* pPlayer)
{
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            spUnit pUnit = fields.at(y)->at(x)->getSpUnit();
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

void GameMap::nextPlayer()
{
    for (qint32 i = 0; i < players.size(); i++)
    {
        if (players.at(i).get() == m_CurrentPlayer.get())
        {
            if (i == players.size() - 1)
            {
                m_CurrentPlayer = players[0];
                break;
            }
            else
            {
                m_CurrentPlayer = players[i + 1];
                break;
            }
        }
    }
}

void GameMap::nextTurn()
{
    enableUnits(m_CurrentPlayer.get());
    nextPlayer();
    m_CurrentPlayer->earnMoney();
}
