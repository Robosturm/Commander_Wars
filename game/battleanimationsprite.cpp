#include "battleanimationsprite.h"

#include "coreengine/mainapp.h"

#include "coreengine/tweentogglevisibility.h"

#include "resource_management/battleanimationmanager.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "qcolor.h"

const QString BattleAnimationSprite::standingAnimation = "loadStandingAnimation";
const QString BattleAnimationSprite::impactAnimation = "loadImpactAnimation";
const QString BattleAnimationSprite::fireAnimation = "loadFireAnimation";
const QString BattleAnimationSprite::moveInAnimation = "loadMoveInAnimation";

BattleAnimationSprite::BattleAnimationSprite(Unit* pUnit, Terrain* pTerrain, QString animationType, qint32 hp)
    : QObject(),
      m_pUnit(pUnit),
      m_pTerrain(pTerrain),
      hpRounded(hp)
{
    if (hpRounded < 0.0f)
    {
        hpRounded = pUnit->getHpRounded();
    }
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    m_Actor = new oxygine::ClipRectActor();
    m_Actor->setSize(127, 192);
    setSize(m_Actor->getWidth(), m_Actor->getHeight());
    addChild(m_Actor);
    loadAnimation(animationType);
}

void BattleAnimationSprite::loadAnimation(QString animationType)
{
    loadAnimation(animationType, m_pUnit);
}

void BattleAnimationSprite::loadAnimation(QString animationType, Unit* pUnit, bool clearSprite)
{
    Mainapp* pApp = Mainapp::getInstance();
    if (clearSprite)
    {
        m_Actor->removeChildren();
    }
    QString function1 = animationType;
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj2;
    QJSValue erg = pApp->getInterpreter()->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1, args1);
}

QPoint BattleAnimationSprite::getUnitPositionOffset(qint32 unitIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getPositionOffset";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(m_pUnit);
    args1 << obj2;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(m_pTerrain);
    args1 << obj3;
    args1 << unitIdx;
    QJSValue erg = pApp->getInterpreter()->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1, args1);
    return erg.toVariant().toPoint();
}

qint32 BattleAnimationSprite::getMaxUnitCount()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMaxUnitCount";
    QJSValue erg = pApp->getInterpreter()->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 1;
    }
}

qint32 BattleAnimationSprite::getImpactDurationMS()
{
    return getImpactDurationMS(m_pUnit);
}

qint32 BattleAnimationSprite::getImpactDurationMS(Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getImpactDurationMS";
    QJSValue erg = pApp->getInterpreter()->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 1000;
    }
}

bool BattleAnimationSprite::hasMoveInAnimation()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "hasMoveInAnimation";
    QJSValue erg = pApp->getInterpreter()->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    else
    {
        return false;
    }
}

qint32 BattleAnimationSprite::getFireDurationMS()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getFireDurationMS";
    QJSValue erg = pApp->getInterpreter()->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 500;
    }
}

qint32 BattleAnimationSprite::getMoveInDurationMS()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMoveInDurationMS";
    QJSValue erg = pApp->getInterpreter()->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 500;
    }
}

QPoint BattleAnimationSprite::getUnitPosition(qint32 unitCount, qint32 maxUnitCount)
{
    qint32 x = (unitCount * 70) % 100;
    qint32 y = 20 * (maxUnitCount - unitCount);
    return QPoint(x, y);
}

void BattleAnimationSprite::loadSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset,
                                       qint32 loops, float scale, short priority, qint32 showDelay)
{
    loadMovingSprite(spriteID, addPlayerColor, maxUnitCount, offset,
                     QPoint(0, 0), QPoint(0,0), 0,
                     loops, scale, priority, showDelay);
}

void BattleAnimationSprite::loadMovingSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset,
                QPoint startPoint, QPoint endPoint, qint32 moveTime,
                qint32 loops, float scale, short priority, qint32 showDelay)
{
    BattleAnimationManager* pBattleAnimationManager = BattleAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pBattleAnimationManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        qint32 value = Mainapp::roundUp(hpRounded / 10.0f * maxUnitCount);
        for (qint32 i = maxUnitCount; i >= maxUnitCount - value + 1; i--)
        {
            QPoint position(0, 0);
            if (maxUnitCount > 1)
            {
                position = getUnitPosition(i, maxUnitCount);
            }
            oxygine::spSprite pSprite = new oxygine::Sprite();
            if (pAnim->getTotalFrames() > 1)
            {
                oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, loops);
                pSprite->addTween(tween);
            }
            else
            {
                pSprite->setResAnim(pAnim);
            }
            // repaint the unit?
            if (addPlayerColor)
            {
                QColor color = m_pUnit->getOwner()->getColor();
                oxygine::Sprite::TweenColor tweenColor(oxygine::Color(color.red(), color.green(), color.blue(), 255));
                oxygine::spTween tween = oxygine::createTween(tweenColor, 1);
                pSprite->addTween(tween);
            }
            QPoint posOffset = getUnitPositionOffset(i);
            qint32 xPos = position.x() + offset.x() + posOffset.x();
            qint32 yPos = 192 - position.y() - offset.y() - pAnim->getHeight() - posOffset.y();
            pSprite->setPosition(xPos + startPoint.x(), yPos + startPoint.y());
            if (moveTime > 0)
            {
                oxygine::spTween moveTween = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(xPos + endPoint.x(), yPos + endPoint.y())), moveTime / Settings::getAnimationSpeed(), loops, false, showDelay);
                pSprite->addTween(moveTween);
            }
            if (showDelay > 0)
            {
                oxygine::spTween visibileTween = oxygine::createTween(TweenToggleVisibility(0.9f, 1.0f), showDelay, loops);
                pSprite->addTween(visibileTween);
            }
            pSprite->setPriority(i + priority);
            pSprite->setScale(scale);
            m_Actor->addChild(pSprite);
        }
    }
}

qint32 BattleAnimationSprite::getHpRounded() const
{
    return hpRounded;
}

void BattleAnimationSprite::setHpRounded(const qint32 &value)
{
    hpRounded = value;
}
