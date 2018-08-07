#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"

#include "game/terrain.h"

#include "game/player.h"

const QString GameMap::m_JavascriptName = "map";
const qint32 GameMap::frameTime = 150;
GameMap* GameMap::m_pInstance = nullptr;


GameMap::GameMap(qint32 width, qint32 heigth)
    : width(width),
      heigth(heigth)
{
    m_pInstance = this;
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    pInterpreter->setGlobal(m_JavascriptName, pInterpreter->newQObject(this));

    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    pTerrainManager->loadAll();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    pBuildingSpriteManager->loadAll();

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
    for (qint32 i = 0; i < 18; i++)
    {
        players.append(new Player(i));
    }
}

GameMap::GameMap(QString map)
{

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
    static const qint32 minVisible = 10;
    Mainapp* pApp = Mainapp::getInstance();
    if (resX > pApp->getSettings()->getWidth()  - minVisible * this->getZoom() * Imagesize)
    {
        resX = pApp->getSettings()->getWidth() - minVisible * this->getZoom() * Imagesize;
    }
    if (resX < -m_zoom * Imagesize * width + minVisible * this->getZoom() * Imagesize)
    {
        resX = -m_zoom * Imagesize * width + minVisible * this->getZoom() * Imagesize;
    }
    if (resY > pApp->getSettings()->getHeight() - minVisible * this->getZoom() * Imagesize)
    {
        resY = pApp->getSettings()->getHeight() - minVisible * this->getZoom() * Imagesize;
    }
    if (resY < -m_zoom * Imagesize * heigth + minVisible * this->getZoom() * Imagesize)
    {
        resY = -m_zoom * Imagesize * heigth + minVisible * this->getZoom() * Imagesize;
    }

    this->setPosition(resX, resY);
}

void GameMap::zoom(float zoom)
{
    m_zoom += zoom / 10.0f;
    // limit zoom
    if (m_zoom > 1.5f)
    {
        m_zoom = 1.5f;
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
        else
        {
               fields.at(y)->at(x)->loadSprites();
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
