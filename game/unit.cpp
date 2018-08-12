#include "unit.h"

#include <QColor>

#include "game/player.h"

#include "resource_management/unitspritemanager.h"

#include "game/gamemap.h"

#include "coreengine/console.h"

Unit::Unit(QString unitID, spPlayer pOwner)
    : QObject(),
      m_UnitID(unitID),
      m_Owner(pOwner)
{
    if (!m_UnitID.isEmpty())
    {
        updateSprites();
    }
}

void Unit::setOwner(spPlayer pOwner)
{
    // change ownership
    m_Owner = pOwner;
    // update sprites :)
    updateSprites();
}

 spPlayer Unit::getSpOwner()
 {
    return m_Owner;
 }

 void Unit::setTerrain(spTerrain pTerrain)
 {
     m_Terrain = pTerrain;
 }

void Unit::loadSprite(QString spriteID, bool addPlayerColor)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
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
        // repaint the unit?
        if (addPlayerColor)
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
        m_pUnitSprites.append(pSprite);

    }
    else
    {
        Console::print("Unable to load unit sprite: " + spriteID, Console::eERROR);
    }
}

Player* Unit::getOwner()
{
    return m_Owner.get();
}

void Unit::updateSprites()
{
    Mainapp* pApp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_pUnitSprites.size(); i++)
    {
        this->removeChild(m_pUnitSprites[i]);
    }
    // call the js loader function to do the rest
    m_pUnitSprites.clear();
    QString function1 = "loadSprites";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
}

QString Unit::getMovementType()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementType";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

QString Unit::getUnitID()
{
    return m_UnitID;
}

qint32 Unit::getX() const
{
    return m_Terrain->getX();
}

qint32 Unit::getY() const
{
    return m_Terrain->getY();
}

void Unit::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_UnitID.toStdString().c_str();
    pStream << m_Hp;
    pStream << m_Ammo1;
    pStream << m_Ammo2;
    pStream << m_Fuel;
    pStream << m_Rank;
    pStream << m_Owner->getPlayerID();
}

void Unit::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    char* id;
    pStream >> id;
    m_UnitID = id;
    pStream >> m_Hp;
    pStream >> m_Ammo1;
    pStream >> m_Ammo2;
    pStream >> m_Fuel;
    pStream >> m_Rank;
    quint32 playerID = 0;
    pStream >> playerID;
    m_Owner = GameMap::getInstance()->getspPlayer(playerID);
    updateSprites();
}
