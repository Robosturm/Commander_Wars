#include "terrain.h"

#include "coreengine/console.h"

#include "coreengine/mainapp.h"

#include "resource_management/terrainmanager.h"

#include "game/gamemap.h"

#include <QFileInfo>

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
        obj = pApp->getInterpreter()->newQObject(this);
        args << obj;
        pApp->getInterpreter()->doFunction(terrainID, function, args);

        // load sprite of the base terrain
        function = "loadBaseTerrain";
        pApp->getInterpreter()->doFunction(terrainID, function, args);
    }
    else
    {
        Console::print(tr("Unable to load Terrain ") + terrainID, Console::eFATAL);
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
    QString function = "loadBaseSprite";
    QJSValueList args;
    QJSValue obj = pApp->getInterpreter()->getGlobal(terrainID);
    obj = pApp->getInterpreter()->newQObject(this);
    args << obj;
    pApp->getInterpreter()->doFunction(terrainID, function, args);
    function = "loadOverlaySprite";
    pApp->getInterpreter()->doFunction(terrainID, function, args);
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
    if (Animated)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnim);
    }
    this->addChild(pSprite);
}

QString Terrain::getSurroundings(QString list, bool blacklist = false, qint32 searchType)
{
    QStringList searchList = list.split(",");
    QString ret = "";
    for (qint32 i = 0; i < 8; i++)
    {
        qint32 curX = x;
        qint32 curY = y;
        // get our x, y coordinates
        GameMap::getField(curX, curY, i);
        QString neighbourID =
        switch (searchType)
        {
            case SearchTypes::All:
            {

                break;
            }
            case SearchTypes::Direct:
            {
                if ((i == GameMap::Directions::North) ||
                    (i == GameMap::Directions::East) ||
                    (i == GameMap::Directions::South) ||
                    (i == GameMap::Directions::West))
                {
                    if (blacklist)
                    {
                        if (!searchList.contains())
                    }
                    else
                    {

                    }
                }
                break;
            }
            case SearchTypes::Diagnonal:
            {
                break;
            }
            default:
            {
                // do nothing
            }
        }
    }
}

void Terrain::loadOverlaySprite(spriteID)
{

}

bool Terrain::getAnimated() const
{
    return Animated;
}

void Terrain::setAnimated(bool value)
{
    Animated = value;
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
