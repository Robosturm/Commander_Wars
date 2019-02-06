
#include "building.h"

#include "game/player.h"

#include "game/terrain.h"

#include "coreengine/console.h"

#include "game/gamemap.h"

#include "game/unit.h"

#include "resource_management/buildingspritemanager.h"

Building::Building(const QString& BuildingID)
    : m_BuildingID(BuildingID),
      m_Owner(nullptr),
      m_Terrain(nullptr)
{

}

void Building::setUnitOwner(Unit* pUnit)
{
    setOwner(pUnit->getSpOwner());
}

void Building::setOwner(spPlayer pOwner)
{
    // change ownership
    m_Owner = pOwner;
    // update sprites :)
    updateBuildingSprites();
}

spPlayer Building::getSpOwner()
{
    return m_Owner;
}

Player* Building::getOwner()
{
    return m_Owner.get();
}

qint32 Building::getOwnerID()
{
    if (m_Owner.get() != nullptr)
    {
        return m_Owner->getPlayerID();
    }
    return -1;
}

void Building::loadSprite(QString spriteID, bool addPlayerColor)
{
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pBuildingSpriteManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime * 2, -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        // repaint the building?
        if (addPlayerColor && m_Owner.get() != nullptr)
        {
            QColor color = m_Owner->getColor();
            for (qint32 i = 0; i < 5; i++)
            {
                oxygine::Sprite::TweenColor tweenColor(oxygine::Color(color.red(), color.green(), color.blue(), 255));
                oxygine::spTween tween = oxygine::createTween(tweenColor, 1);
                //pSprite->setBlendMode(oxygine::blend_disabled);
                pSprite->addTween(tween);
            }
        }
        pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
        pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
        this->addChild(pSprite);
        m_pBuildingSprites.append(pSprite);

    }
    else
    {
        Console::print("Unable to load terrain sprite: " + spriteID, Console::eERROR);
    }
}

void Building::updateBuildingSprites()
{
    Mainapp* pApp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_pBuildingSprites.size(); i++)
    {
        this->removeChild(m_pBuildingSprites[i]);
    }
    // call the js loader function to do the rest
    m_pBuildingSprites.clear();
    QString function1 = "loadSprites";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
}

bool Building::canBuildingBePlaced(QString terrainID)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "canBuildingBePlaced";
    QJSValueList args1;
    args1 << terrainID;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    else
    {
        return false;
    }
}

quint32 Building::getBaseIncome() const
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBaseIncome";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toUInt();
    }
    else
    {
        return 0;
    }
}

qint32 Building::getX() const
{
    return m_Terrain->getX();
}

qint32 Building::getY() const
{
    return m_Terrain->getY();
}

void Building::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_BuildingID.toStdString().c_str();
    if (m_Owner.get() == nullptr)
    {
        pStream << static_cast<qint32>(-1);
    }
    else
    {
        pStream << static_cast<qint32>(m_Owner->getPlayerID());
    }
}

QStringList Building::getActionList()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getActions";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_BuildingID, function1, args1);
    if (ret.isString())
    {
        return ret.toString().split(",");
    }
    else
    {
        return QStringList();
    }
}

void Building::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    char* id;
    pStream >> id;
    m_BuildingID = id;
    qint32 playerID = -1;
    pStream >> playerID;
    m_Owner = GameMap::getInstance()->getspPlayer(playerID);
    updateBuildingSprites();
}
