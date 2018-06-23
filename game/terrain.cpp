#include "terrain.h"

#include "coreengine/console.h"
#include "coreengine/mainapp.h"
#include "resource_management/terrainmanager.h"

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

        function = "loadBaseTerrainSprite";
        pApp->getInterpreter()->doFunction(terrainID, function, args);
    }
    else
    {
        Console::print(tr("Unable to load Terrain ") + terrainID, Console::eFATAL);
    }
}

void Terrain::loadBaseTerrainSprite(QString spriteID)
{
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    if (spriteID != "")
    {
        if (Animated)
        {

        }
        else
        {
            oxygine::spSprite sprite = new oxygine::Sprite();
            oxygine::Actor::addChild(sprite);
            sprite->setResAnim(pTerrainManager->getResAnim(spriteID.toStdString().c_str()));
        }
    }
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
