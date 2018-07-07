#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "resource_management/terrainmanager.h"

#include "game/terrain.h"

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

    for (qint32 y = 0; y < heigth; y++)
    {
        fields.append(new QVector<spTerrain>());
        for (qint32 x = 0; x < width; x++)
        {
            // test code

            spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y);
            this->addChild(pTerrain);

            fields[y]->append(pTerrain);
            pTerrain->setPosition(x * 24.0f, y * 24.0f);
        }
    }
    updateTerrainSprites();
    centerMap(width / 2, heigth / 2);
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

void GameMap::updateTerrainSprites()
{
    // update terrain sprites
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            fields.at(x)->at(y)->loadSprites();
        }
    }
}

qint32 GameMap::getWidth() const
{
    return width;
}

void GameMap::setWidth(const qint32 &value)
{
    width = value;
}

qint32 GameMap::getHeigth() const
{
    return heigth;
}

void GameMap::setHeigth(const qint32 &value)
{
    heigth = value;
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
    this->setPosition(pApp->getSettings()->getWidth() / 2.0f - x * m_zoom * Imagesize - Imagesize / 2.0f,
                      pApp->getSettings()->getHeight() / 2.0f - y * m_zoom * Imagesize - Imagesize / 2.0f);
}

void GameMap::moveMap(qint32 x, qint32 y)
{
    // draw point
    qint32 resX = this->getPosition().x + x;
    qint32 resY = this->getPosition().y + y;
    static const qint32 minVisible = 10;
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
