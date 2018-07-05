#include "terrain.h"

#include "coreengine/console.h"

#include "coreengine/mainapp.h"

#include "resource_management/terrainmanager.h"

#include "game/gamemap.h"

#include <QFileInfo>

Terrain* Terrain::createTerrain(const QString& terrainID, qint32 x, qint32 y)
{
    Terrain* pTerrain = new Terrain(terrainID, x, y);
    pTerrain->createBaseTerrain();
    return pTerrain;
}

Terrain::Terrain(const QString& terrainID, qint32 x, qint32 y)
    : terrainID(terrainID),
      x(x),
      y(y)
{
    Mainapp* pApp = Mainapp::getInstance();
    QJSValue obj = pApp->getInterpreter()->getGlobal(terrainID);
    // check if the js-script was loaded already
    // otherwise do load it
    bool terrainExists = true;
    if (!obj.isObject())
    {
        terrainExists = TerrainManager::getInstance()->loadTerrain(terrainID);
    }
    if (terrainExists)
    {
        QString function = "init";
        QJSValueList args;
        QJSValue objArg = pApp->getInterpreter()->newQObject(this);
        args << objArg;
        pApp->getInterpreter()->doFunction(terrainID, function, args);
    }
    else
    {
        Console::print(tr("Unable to load Terrain ") + terrainID, Console::eFATAL);
    }
}

QString Terrain::getTerrainName() const
{
    return terrainName;
}

void Terrain::setTerrainName(const QString &value)
{
    terrainName = value;
}

Terrain::~Terrain()
{



}

void Terrain::createBaseTerrain()
{
    Mainapp* pApp = Mainapp::getInstance();
    QJSValueList args;
    QJSValue obj = pApp->getInterpreter()->newQObject(this);
    args << obj;
    // load sprite of the base terrain
    QString function = "loadBaseTerrain";
    pApp->getInterpreter()->doFunction(terrainID, function, args);
    if (m_pBaseTerrain != nullptr)
    {
        m_pBaseTerrain->createBaseTerrain();
    }
}

void Terrain::loadSprites()
{
    // unload old stuff
    if (m_pTerrainSprite.get() != nullptr)
    {
        this->removeChild(m_pTerrainSprite);
        m_pTerrainSprite = nullptr;
    }
    if (m_pOverlaySprites.size() > 0)
    {
        for (qint32 i = 0; i < m_pOverlaySprites.size(); i++)
        {
            this->removeChild(m_pOverlaySprites[i]);
        }
        m_pOverlaySprites.clear();
    }
    // load sub terrain
    if (m_pBaseTerrain != nullptr)
    {
        m_pBaseTerrain->loadSprites();
    }
    // load main terrain
    Mainapp* pApp = Mainapp::getInstance();
    if (m_FixedSprite)
    {
        loadBaseSprite(m_terrainSpriteName);
    }
    else
    {
        QString function1 = "loadBaseSprite";
        QJSValueList args1;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
        args1 << obj1;
        pApp->getInterpreter()->doFunction(terrainID, function1, args1);
    }
    // next call starting by 0 again
    QString function2 = "loadOverlaySprite";
    QJSValueList args2;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args2 << obj2;
    pApp->getInterpreter()->doFunction(terrainID, function2, args2);
}

void Terrain::loadBaseTerrain(QString terrainID)
{
    m_pBaseTerrain = new Terrain(terrainID, x, y);
    this->addChild(m_pBaseTerrain);
}

void Terrain::loadBaseSprite(QString spriteID)
{

    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(spriteID.toStdString());
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
    pSprite->setPosition(-(pAnim->getWidth() - GameMap::Imagesize) / 2, -(pAnim->getHeight() - GameMap::Imagesize));
    this->addChild(pSprite);
    m_terrainSpriteName = spriteID;
    m_pTerrainSprite = pSprite;
}

QString Terrain::getSurroundings(QString list, bool useBaseTerrainID, bool blacklist, qint32 searchType)
{
    QStringList searchList = list.split(",");
    QString ret = "";
    for (qint32 i = 0; i < 8; i++)
    {
        qint32 curX = x;
        qint32 curY = y;
        // get our x, y coordinates
        GameMap::getField(curX, curY, static_cast<GameMap::Directions>(i));
        GameMap* pGameMap = GameMap::getInstance();
        if (pGameMap->onMap(curX, curY))
        {
            QString neighbourID = "";
            if (useBaseTerrainID)
            {
                neighbourID = pGameMap->getTerrain(curX, curY)->getBaseTerrainID();
            }
            else
            {
                neighbourID = pGameMap->getTerrain(curX, curY)->getTerrainID();
            }
            QString addString = "";
            // load compare value
            GameMap::Directions compareValue = GameMap::Directions::None;
            if (searchType == GameMap::Directions::All)
            {
                compareValue = static_cast<GameMap::Directions>(i);
            }
            else if (searchType == GameMap::Directions::Direct)
            {
                switch (i)
                {
                    case GameMap::Directions::North:
                    case GameMap::Directions::East:
                    case GameMap::Directions::West:
                    case GameMap::Directions::South:
                    {
                        compareValue = static_cast<GameMap::Directions>(i);
                        break;
                    }
                    default:
                    {
                        // do nothing
                        compareValue = GameMap::Directions::None;
                        break;
                    }
                }
            }
            else if (searchType == GameMap::Directions::Diagnonal)
            {
                switch (i)
                {
                    case GameMap::Directions::NorthEast:
                    case GameMap::Directions::NorthWest:
                    case GameMap::Directions::SouthWest:
                    case GameMap::Directions::SouthEast:
                    {
                        compareValue = static_cast<GameMap::Directions>(i);
                        break;
                    }
                    default:
                    {
                        compareValue = GameMap::Directions::None;
                        // do nothing
                        break;
                    }
                }
            }
            else if (searchType == i)
            {
                compareValue = static_cast<GameMap::Directions>(i);
            }
            else
            {
                // you asshole reached unreachable code :D
            }
            // check for compare value to find string
            if (compareValue == GameMap::Directions::North)
            {
                addString = "+N";
            }
            else if (compareValue == GameMap::Directions::East)
            {
                addString = "+E";
            }
            else if (compareValue == GameMap::Directions::South)
            {
                addString = "+S";
            }
            else if (compareValue == GameMap::Directions::West)
            {
                addString = "+W";
            }
            else if (compareValue == GameMap::Directions::NorthEast)
            {
                addString = "+NE";
            }
            else if (compareValue == GameMap::Directions::SouthEast)
            {
                addString = "+SE";
            }
            else if (compareValue == GameMap::Directions::SouthWest)
            {
                addString = "+SW";
            }
            else if (compareValue == GameMap::Directions::NorthWest)
            {
                addString = "+NW";
            }
            else
            {
                // do nothing
            }
            if (blacklist)
            {
                if (!searchList.contains(neighbourID))
                {
                    ret += addString;
                }
            }
            else
            {
                if (searchList.contains(neighbourID))
                {
                    ret += addString;
                }
            }
        }
    }
    return ret;
}

void Terrain::loadOverlaySprite(QString spriteID)
{
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    oxygine::ResAnim* pAnim = pTerrainManager->getResAnim(spriteID.toStdString());
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
    this->addChild(pSprite);
    m_pOverlaySprites.append(pSprite);
}

QString Terrain::getTerrainID() const
{
    return terrainID;
}

qint32 Terrain::getX() const
{
    return x;
}

void Terrain::setX(const qint32 &value)
{
    x = value;
}

qint32 Terrain::getY() const
{
    return y;
}

void Terrain::setY(const qint32 &value)
{
    y = value;
}
