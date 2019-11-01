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
const QString BattleAnimationSprite::standingFiredAnimation = "loadStandingFiredAnimation";

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

    connect(this, &BattleAnimationSprite::sigDetachChild, this, &BattleAnimationSprite::detachChild, Qt::QueuedConnection);
}

void BattleAnimationSprite::loadAnimation(QString animationType)
{
    loadAnimation(animationType, m_pUnit);
}

void BattleAnimationSprite::loadAnimation(QString animationType, Unit* pUnit, Unit* pDefender, qint32 attackerWeapon, bool clearSprite)
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
    QJSValue obj3 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj3;
    args1 << attackerWeapon;
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
    if (maxUnitCount > 0)
    {
        return maxUnitCount;
    }
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

void BattleAnimationSprite::setMaxUnitCount(const qint32 &value)
{
    maxUnitCount = value;
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
                                       qint32 loops, float scale, short priority, qint32 showDelay,
                                       bool _invertFlipX, bool deleteAfter)
{
    loadMovingSprite(spriteID, addPlayerColor, maxUnitCount, offset,
                     QPoint(0, 0), 0, deleteAfter,
                     loops, scale, priority, showDelay,
                     _invertFlipX);
}

qint32 BattleAnimationSprite::getUnitCount(qint32 maxUnitCount)
{
    return Mainapp::roundUp(hpRounded / 10.0f * maxUnitCount);
}

void BattleAnimationSprite::loadMovingSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset,
                                             QPoint movement, qint32 moveTime, bool deleteAfter,
                                             qint32 loops, float scale, short priority, qint32 showDelay,
                                             bool _invertFlipX)
{    
    qint32 value = getUnitCount(maxUnitCount);
    for (qint32 i = maxUnitCount; i >= maxUnitCount - value + 1; i--)
    {
        QPoint position(0, 0);
        if (maxUnitCount > 1)
        {
            position = getUnitPosition(i, maxUnitCount);
        }
        QPoint posOffset = getUnitPositionOffset(i);
        loadSingleMovingSprite(spriteID, addPlayerColor, offset + position + posOffset, movement, moveTime, deleteAfter,
                               loops, scale, i + priority, showDelay, _invertFlipX);
    }
}

void BattleAnimationSprite::loadSingleMovingSprite(QString spriteID, bool addPlayerColor, QPoint offset,
                QPoint movement, qint32 moveTime, bool deleteAfter,
                qint32 loops, float scale, short priority, qint32 showDelay,
                bool _invertFlipX)
{
    BattleAnimationManager* pBattleAnimationManager = BattleAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pBattleAnimationManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), loops, false, oxygine::timeMS(static_cast<qint64>(showDelay / Settings::getBattleAnimationSpeed())));
            pSprite->addTween(tween);
            if (deleteAfter && moveTime <= 0)
            {
                tween->addDoneCallback([=](oxygine::Event * pEvent)
                {
                    oxygine::spActor pTarget = dynamic_cast<oxygine::Actor*>(pEvent->target.get());
                    if (pTarget.get() != nullptr)
                    {
                        emit sigDetachChild(pTarget);
                    }
                });
            }
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        // repaint the unit?
        if (addPlayerColor)
        {
            QColor color = m_pUnit->getOwner()->getColor();
            pSprite->setColor(color);
        }
        pSprite->setPriority(priority);
        pSprite->setScale(scale);
        pSprite->setInvertFlipX(_invertFlipX);

        qint32 xPos = offset.x();
        if (isFlippedX())
        {
            qint32 width = pAnim->getWidth() * scale;
            xPos = 127 - xPos - width;
        }
        qint32 yPos = 192 - offset.y() - pAnim->getHeight() * scale;
        pSprite->setPosition(xPos , yPos);
        if (moveTime > 0)
        {
            qint32 endX = xPos + movement.x();
            if (isFlippedX())
            {
                endX = xPos - movement.x();
            }
            oxygine::spTween moveTween = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(endX, yPos - movement.y())), oxygine::timeMS(static_cast<qint64>(moveTime / Settings::getBattleAnimationSpeed())), loops, false, oxygine::timeMS(static_cast<qint64>(showDelay / Settings::getBattleAnimationSpeed())));
            if (deleteAfter)
            {
                moveTween->addDoneCallback([=](oxygine::Event * pEvent)
                {
                    oxygine::spActor pTarget = dynamic_cast<oxygine::Actor*>(pEvent->target.get());
                    if (pTarget.get() != nullptr)
                    {
                        emit sigDetachChild(pTarget);
                    }
                });
            }
            pSprite->addTween(moveTween);
        }
        if (showDelay > 0)
        {
            oxygine::spTween visibileTween = oxygine::createTween(TweenToggleVisibility(0.9f, 1.0f), oxygine::timeMS(static_cast<qint64>(showDelay / Settings::getBattleAnimationSpeed())), loops);
            pSprite->addTween(visibileTween);
        }
        m_Actor->addChild(pSprite);
    }
}

void BattleAnimationSprite::detachChild(oxygine::spActor pActor)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    pActor->detach();
    pApp->continueThread();
}

qint32 BattleAnimationSprite::getHpRounded() const
{
    return hpRounded;
}

void BattleAnimationSprite::setHpRounded(const qint32 &value)
{
    hpRounded = value;
}

void BattleAnimationSprite::loadSound(QString file, qint32 loops, QString folder, qint32 delay)
{
    Mainapp* pApp = Mainapp::getInstance();
    AudioThread* pAudio = pApp->getAudioThread();
    m_Sounds.append(std::tuple<QString, QString>(file, folder));
    pAudio->playSound(file, loops, folder, delay);
}

void BattleAnimationSprite::stopSound()
{
    Mainapp* pApp = Mainapp::getInstance();
    AudioThread* pAudio = pApp->getAudioThread();
    for (qint32 i = 0; i < m_Sounds.size(); i++)
    {
        pAudio->stopSound(std::get<0>(m_Sounds[i]), std::get<1>(m_Sounds[i]));
    }
}
