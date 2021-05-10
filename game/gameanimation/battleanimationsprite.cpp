#include "qcolor.h"

#include "coreengine/mainapp.h"
#include "coreengine/tweens/tweentogglevisibility.h"
#include "coreengine/audiothread.h"
#include "coreengine/globalutils.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/tweens/tweenscreenshake.h"

#include "resource_management/battleanimationmanager.h"

#include "game/player.h"
#include "game/gameanimation/battleanimationsprite.h"
#include "game/gameanimation/battleanimation.h"
#include "game/co.h"


const QString BattleAnimationSprite::standingAnimation = "loadStandingAnimation";
const QString BattleAnimationSprite::impactUnitOverlayAnimation = "loadImpactUnitOverlayAnimation";
const QString BattleAnimationSprite::impactAnimation = "loadImpactAnimation";
const QString BattleAnimationSprite::fireAnimation = "loadFireAnimation";
const QString BattleAnimationSprite::moveInAnimation = "loadMoveInAnimation";
const QString BattleAnimationSprite::standingFiredAnimation = "loadStandingFiredAnimation";
const QString BattleAnimationSprite::dyingAnimation = "loadDyingAnimation";
const QString BattleAnimationSprite::stopAnimation = "loadStopAnimation";

BattleAnimationSprite::BattleAnimationSprite(spUnit pUnit, Terrain* pTerrain, QString animationType, qint32 hp, bool playSound)
    : QObject(),
      m_pUnit(pUnit),
      m_pTerrain(pTerrain),
      m_hpRounded(hp),
      m_nextFrameTimer(this),
      m_playSound(playSound)
{
    setObjectName("BattleAnimationSprite");
    if (m_hpRounded < 0.0f)
    {
        m_hpRounded = pUnit->getHpRounded();
    }
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    // setup next frame timer
    m_nextFrameTimer.setSingleShot(true);
    setUnitFrameDelay(75);

    m_Actor = oxygine::spClipRectActor::create();
    m_Actor->setSize(127, 192);
    setSize(m_Actor->getWidth(), m_Actor->getHeight());
    addChild(m_Actor);
    loadAnimation(animationType);
    connect(this, &BattleAnimationSprite::sigDetachChild, this, &BattleAnimationSprite::detachChild, Qt::QueuedConnection);
    connect(&m_nextFrameTimer, &QTimer::timeout, this, &BattleAnimationSprite::startNextUnitFrames, Qt::QueuedConnection);
}

BattleAnimationSprite::~BattleAnimationSprite()
{
    stopSound();
}

void BattleAnimationSprite::clear()
{
    m_Actor->removeChildren();
    m_nextFrames.clear();
    m_currentFrame.clear();
}

void BattleAnimationSprite::flipActorsX(bool flippedX)
{
    if (m_nextFrames.length() > 0)
    {
        for (auto & frame : m_nextFrames[m_nextFrames.length() - 1])
        {
            for (auto & sprite : frame)
            {
                sprite->flipActorsX(flippedX);
            }
        }
    }
    oxygine::Sprite::flipActorsX(flippedX);
}

void BattleAnimationSprite::loadAnimation(QString animationType)
{
    loadAnimation(animationType, m_pUnit.get());
}

void BattleAnimationSprite::loadAnimation(QString animationType, Unit* pUnit, Unit* pDefender, qint32 attackerWeapon, bool clearSprite, bool start)
{
    QVector<QVector<oxygine::spSprite>> buffer;
    if (!clearSprite && m_nextFrames.length() > 0)
    {
        for (auto & unitFrame : m_nextFrames[m_nextFrames.length() - 1])
        {
            buffer.append(QVector<oxygine::spSprite>());
            for (auto & sprite : unitFrame)
            {
                buffer[buffer.length() - 1].append(sprite);
            }
        }
        m_nextFrames.removeAt(m_nextFrames.length() - 1);
    }
    else if (!clearSprite)
    {
        for (auto & unitFrame : m_currentFrame)
        {
            buffer.append(QVector<oxygine::spSprite>());
            for (auto & sprite : unitFrame)
            {
                buffer[buffer.length() - 1].append(sprite);
            }
        }
    }
    bool startFrame = false;
    if (m_nextFrames.length() == 0)
    {
        startFrame = true;
        m_frameIterator = 0;
    }
    if ((m_nextFrames.length() == 0 ||
        m_nextFrames[m_nextFrames.length() - 1].length() > 0))
    {
        m_nextFrames.append(QVector<QVector<oxygine::spSprite>>());
    }
    if (!clearSprite)
    {
        for (qint32 i = 0; i <  buffer.size(); ++i)
        {
            if (i >= m_nextFrames[m_nextFrames.length() - 1].size())
            {
                m_nextFrames[m_nextFrames.length() - 1].append(QVector<oxygine::spSprite>());
            }
        }
        for (qint32 i = buffer.size() - 1; i >= 0 ; --i)
        {
            for (auto & sprite : buffer[i])
            {
                m_nextFrames[m_nextFrames.length() - 1][i].prepend(sprite);
            }
        }
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = animationType;
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pDefender);
    args1 << obj3;
    args1 << attackerWeapon;
    pInterpreter->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1, args1);
    if (m_nextFrames.length() > 0 && !clearSprite)
    {
        for (qint32 i = m_currentFrame.length() - 1; i >= 0; --i)
        {
            if (i >= m_nextFrames[0].size())
            {
                for (auto & sprite : m_currentFrame[i])
                {
                    sprite->detach();
                }
                m_currentFrame[i].clear();
            }
        }
    }
    if (startFrame && start)
    {
        startNextUnitFrames();
    }
}

qint32 BattleAnimationSprite::loadDyingFadeOutAnimation(qint32 fadeoutTime)
{
    qint32 maxUnitCount = getMaxUnitCount();
    qint32 startCount = getUnitCount(maxUnitCount, GlobalUtils::roundUp(m_dyingStartHp));
    qint32 endCount = getUnitCount(maxUnitCount, GlobalUtils::roundUp(m_dyingEndHp));
    qint32 frameDelay = 75;
    qint32 count = 0;
    for (qint32 i = startCount; i > endCount; --i)
    {
        if (i - 1 < m_currentFrame.size())
        {
            auto & sprites = m_currentFrame[i - 1];
            for (auto & sprite : sprites)
            {
                oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenAlpha(0),
                                                              oxygine::timeMS(fadeoutTime  / static_cast<qint32>(Settings::getBattleAnimationSpeed())), 1, false,
                                                              oxygine::timeMS(count * frameDelay / static_cast<qint32>(Settings::getBattleAnimationSpeed())));
                sprite->addTween(tween);
            }
            loadSound("dying_fadeout.wav", 1, "resources/sounds/", count * frameDelay);
            ++count;
        }
    }
    return count * frameDelay;
}

QPoint BattleAnimationSprite::getUnitPositionOffset(qint32 unitIdx)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getPositionOffset";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(m_pUnit.get());
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(m_pTerrain);
    args1 << obj3;
    args1 << unitIdx;
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1, args1);
    return erg.toVariant().toPoint();
}

qint32 BattleAnimationSprite::getMaxUnitCount()
{
    if (m_maxUnitCount > 0)
    {
        return m_maxUnitCount;
    }
    return getAnimationUnitCount();
}

qint32 BattleAnimationSprite::getAnimationUnitCount()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMaxUnitCount";
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 1;
    }
}

qint32 BattleAnimationSprite::getImpactDurationMS(Unit* pUnit, Unit* pDefender, qint32 attackerWeapon)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getImpactDurationMS";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pDefender);
    args1 << obj3;
    args1 << attackerWeapon;
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 1000;
    }
}

bool BattleAnimationSprite::hasMoveInAnimation(Unit* pUnit, Unit* pDefender, qint32 attackerWeapon)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "hasMoveInAnimation";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pDefender);
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1, args1);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    else
    {
        return false;
    }
}

qint32 BattleAnimationSprite::getDyingDurationMS(Unit* pUnit, Unit* pDefender, qint32 attackerWeapon)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDyingDurationMS";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pDefender);
    args1 << obj3;
    args1 << attackerWeapon;
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

bool BattleAnimationSprite::hasDyingAnimation()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "hasDyingAnimation";
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + m_pUnit->getUnitID(), function1);
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
    m_maxUnitCount = value;
}

qint32 BattleAnimationSprite::getFireDurationMS(Unit* pUnit, Unit* pDefender, qint32 attackerWeapon)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFireDurationMS";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pDefender);
    args1 << obj3;
    args1 << attackerWeapon;
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 500;
    }
}

qint32 BattleAnimationSprite::getMoveInDurationMS(Unit* pUnit, Unit* pDefender, qint32 attackerWeapon)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMoveInDurationMS";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pDefender);
    args1 << obj3;
    args1 << attackerWeapon;
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 500;
    }
}

qint32 BattleAnimationSprite::getStopDurationMS(Unit* pUnit, Unit* pDefender, qint32 attackerWeapon)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getStopDurationMS";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pDefender);
    args1 << obj3;
    args1 << attackerWeapon;
    QJSValue erg = pInterpreter->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1, args1);
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
    QPoint ret = QPoint((unitCount * 70) % 100,
                        20 * (maxUnitCount - unitCount));
    return ret;
}

void BattleAnimationSprite::loadSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset,
                                       qint32 loops, float scale, short priority, qint32 showDelay,
                                       bool _invertFlipX, bool deleteAfter, qint32 frameTime, qint32 frames)
{
    loadMovingSprite(spriteID, addPlayerColor, maxUnitCount, offset,
                     QPoint(0, 0), 0, deleteAfter,
                     loops, scale, priority, showDelay,
                     _invertFlipX, frameTime, frames);
}

void BattleAnimationSprite::loadSpriteV2(QString spriteID, GameEnums::Recoloring mode, qint32 maxUnitCount, QPoint offset,
                                         qint32 loops, float scale, short priority, qint32 showDelay,
                                         bool _invertFlipX, bool deleteAfter, qint32 frameTime, qint32 frames)
{
    loadMovingSpriteV2(spriteID, mode, maxUnitCount, offset,
                       QPoint(0, 0), 0, deleteAfter,
                       loops, scale, priority, showDelay,
                       _invertFlipX, frameTime, frames);
}

qint32 BattleAnimationSprite::getUnitCount(qint32 maxUnitCount)
{
    return getUnitCount(maxUnitCount, m_hpRounded);
}

qint32 BattleAnimationSprite::getUnitCount(qint32 maxUnitCount, qint32 hp)
{
    qint32 count = GlobalUtils::roundUp(hp / Unit::MAX_UNIT_HP * maxUnitCount);
    if (count < 0)
    {
        count = 0;
    }
    return count;
}

void BattleAnimationSprite::loadMovingSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset,
                                             QPoint movement, qint32 moveTime, bool deleteAfter,
                                             qint32 loops, float scale, short priority, qint32 showDelay,
                                             bool _invertFlipX, qint32 frameTime, qint32 frames)
{    
    if (addPlayerColor)
    {
        loadMovingSpriteV2(spriteID, GameEnums::Recoloring_Mask, maxUnitCount, offset, movement,
                           moveTime, deleteAfter, loops, scale, priority, showDelay, _invertFlipX, frameTime, frames);
    }
    else
    {
        loadMovingSpriteV2(spriteID, GameEnums::Recoloring_None, maxUnitCount, offset, movement,
                           moveTime, deleteAfter, loops, scale, priority, showDelay, _invertFlipX, frameTime, frames);
    }
}

void BattleAnimationSprite::loadMovingSpriteV2(QString spriteID, GameEnums::Recoloring mode, qint32 maxUnitCount, QPoint offset,
                                               QPoint movement, qint32 moveTime, bool deleteAfter,
                                               qint32 loops, float scale, short priority, qint32 showDelay,
                                               bool _invertFlipX, qint32 frameTime, qint32 frames)
{
    qint32 value = getUnitCount(maxUnitCount);

    while (m_nextFrames[m_nextFrames.length() - 1].length() < value)
    {
        m_nextFrames[m_nextFrames.length() - 1].append(QVector<oxygine::spSprite>());
    }

    for (qint32 i = maxUnitCount; i >= maxUnitCount - value + 1; i--)
    {
        QPoint position(0, 0);
        if (maxUnitCount > 1)
        {
            qint32 offset = 0;
            if (m_invertStartPosition)
            {
                qint32 livingCount = getUnitCount(getAnimationUnitCount(), m_dyingStartHp);
                if (value < livingCount)
                {
                    offset = livingCount - value;
                }
            }
            position = getUnitPosition(i - offset, maxUnitCount);
        }
        QPoint posOffset = getUnitPositionOffset(i);
        loadSingleMovingSpriteV2(spriteID, mode, offset + position + posOffset, movement, moveTime, deleteAfter,
                                 loops, scale, i + priority, showDelay, _invertFlipX, frameTime, frames);

        m_nextFrames[m_nextFrames.length() - 1][maxUnitCount - i].append(m_lastLoadedSprite);
        m_lastLoadedSprite->detach();
    }
}

void BattleAnimationSprite::loadSingleMovingSpriteV2(QString spriteID, GameEnums::Recoloring mode, QPoint offset,
                                                     QPoint movement, qint32 moveTime, bool deleteAfter,
                                                     qint32 loops, float scale, short priority, qint32 showDelay,
                                                     bool _invertFlipX, qint32 frameTime, qint32 frames)
{    
    BattleAnimationManager* pBattleAnimationManager = BattleAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pBattleAnimationManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim->getTotalFrames() > 1)
        {
            if (frames < 0)
            {
                frames = pAnim->getColumns() - 1;
            }
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, 0, frames), oxygine::timeMS(pAnim->getTotalFrames() * frameTime), loops, false, oxygine::timeMS(static_cast<qint64>(showDelay / Settings::getBattleAnimationSpeed())));
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
        if (mode == GameEnums::Recoloring_Mask)
        {
            QColor color = m_pUnit->getOwner()->getColor();
            pSprite->setColor(color);
        }
        else if (mode == GameEnums::Recoloring_Table)
        {
            pSprite->setColorTable(m_pUnit->getOwner()->getColorTableAnim());
        }
        pSprite->setPriority(priority);
        pSprite->setScale(scale);
        pSprite->setSize(pAnim->getSize());
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
            oxygine::spTween moveTween = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(endX, yPos - movement.y())), oxygine::timeMS(static_cast<qint64>(moveTime / Settings::getBattleAnimationSpeed())), 1, false, oxygine::timeMS(static_cast<qint64>(showDelay / Settings::getBattleAnimationSpeed())));
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
        m_lastLoadedSprite = pSprite;
    }
    
}

void BattleAnimationSprite::loadSingleMovingSprite(QString spriteID, bool addPlayerColor, QPoint offset,
                                                   QPoint movement, qint32 moveTime, bool deleteAfter,
                                                   qint32 loops, float scale, short priority, qint32 showDelay,
                                                   bool _invertFlipX, qint32 frameTime, qint32 frames)
{
    if (addPlayerColor)
    {
        loadSingleMovingSpriteV2(spriteID, GameEnums::Recoloring_Mask, offset, movement,
                                 moveTime, deleteAfter, loops, scale, priority, showDelay, _invertFlipX, frameTime, frames);
    }
    else
    {
        loadSingleMovingSpriteV2(spriteID, GameEnums::Recoloring_None, offset, movement,
                                 moveTime, deleteAfter, loops, scale, priority, showDelay, _invertFlipX, frameTime, frames);
    }
}

void BattleAnimationSprite::addMoveTweenToLastLoadedSprites(qint32 deltaX, qint32 deltaY, qint32 moveTime, qint32 delayPerUnitMs, qint32 loops, bool scaleWithAnimationSpeed)
{
    if (m_nextFrames.size() > 0)
    {
        qint32 count = 0;
        for (auto & sprites : m_nextFrames[m_nextFrames.size() - 1])
        {
            if (sprites.size() > 0)
            {
                auto & sprite = sprites[sprites.size() - 1];
                qint64 time = moveTime;
                if (scaleWithAnimationSpeed)
                {
                    time /= Settings::getBattleAnimationSpeed();
                }
                oxygine::spTween moveTween = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(sprite->getX() + deltaX, sprite->getY() + deltaY)),
                                                                  oxygine::timeMS(time),
                                                                  loops, true);
                sprite->addTween(moveTween);
                moveTween->setElapsed(oxygine::timeMS(delayPerUnitMs * count));
                ++count;
            }
        }
    }
}


void BattleAnimationSprite::loadColorOverlayForLastLoadedFrame(QColor color, qint32 time, qint32 loops, qint32 showDelayMs)
{
    QVector<QVector<oxygine::spSprite>>* frame;
    if (m_nextFrames.length() > 0 && m_nextFrames[m_nextFrames.length() - 1].length() > 0)
    {
        frame = &m_nextFrames[m_nextFrames.length() - 1];
    }
    else
    {
        frame = &m_currentFrame;
    }
    qint32 value = getUnitCount(m_maxUnitCount);
    for (qint32 i = m_maxUnitCount; i >= m_maxUnitCount - value + 1; i--)
    {
        if (i - 1 < frame->length() && i > 0)
        {
            for (auto & sprite : (*frame)[i - 1])
            {
                // add impact image
                oxygine::ColorRectSprite::TweenColor tweenColor2(color);
                oxygine::spTween colorTween2 = oxygine::createTween(tweenColor2, oxygine::timeMS(static_cast<qint64>(time / Settings::getBattleAnimationSpeed())), loops, true,
                                                                    oxygine::timeMS(static_cast<qint64>((showDelayMs + m_nextFrameTimer.interval() * (i - m_maxUnitCount)) / Settings::getBattleAnimationSpeed())));
                sprite->addTween(colorTween2);
            }
        }
    }
}

void BattleAnimationSprite::detachChild(oxygine::spActor pActor)
{
    
    pActor->detach();
    
}

qint32 BattleAnimationSprite::getHpRounded() const
{
    return m_hpRounded;
}

void BattleAnimationSprite::setHpRounded(const qint32 &value)
{
    m_hpRounded = value;
}

void BattleAnimationSprite::loadSound(QString file, qint32 loops, QString folder, qint32 delay, float volume)
{
    if (m_playSound)
    {
        Mainapp* pApp = Mainapp::getInstance();
        AudioThread* pAudio = pApp->getAudioThread();
        m_Sounds.append(std::tuple<QString, QString>(file, folder));
        pAudio->playSound(file, loops, folder, delay / static_cast<qint32>(Settings::getBattleAnimationSpeed()), volume);
    }
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

void BattleAnimationSprite::setUnitFrameDelay(qint32 delay)
{
    m_nextFrameTimer.setInterval(delay / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
}

void BattleAnimationSprite::startNextFrame()
{
    startNextUnitFrames();
}

void BattleAnimationSprite::startNextUnitFrames()
{
    Console::print("Progressing next battle frame", Console::eDEBUG);
    if (m_currentFrame.size() == 0 && !m_startWithFraming)
    {
        for (auto & unitFrame : m_nextFrames[0])
        {
            m_currentFrame.append(QVector<oxygine::spSprite>());
            for (auto & sprite : unitFrame)
            {
                m_Actor->addChild(sprite);
                m_currentFrame[m_currentFrame.length() - 1].append(sprite);
            }
        }
        m_nextFrames.removeFirst();
    }
    else
    {
        if (m_frameIterator < m_currentFrame.size())
        {
            for (auto & sprite : m_currentFrame[m_frameIterator])
            {
                sprite->detach();
            }
            m_currentFrame[m_frameIterator].clear();
        }
        if (m_currentFrame.length() <= m_frameIterator)
        {
            m_currentFrame.append(QVector<oxygine::spSprite>());
        }
        if (m_nextFrames.length() > 0)
        {
            if (m_frameIterator < m_nextFrames[0].length())
            {
                for (auto & sprite : m_nextFrames[0][m_frameIterator])
                {
                    m_currentFrame[m_frameIterator].append(sprite);
                    m_Actor->addChild(sprite);
                }
                m_nextFrames[0][m_frameIterator].clear();
            }
        }
    }

    if (m_nextFrames.length() > 0)
    {
        ++m_frameIterator;
        if (m_frameIterator >= m_nextFrames[0].size())
        {
            Console::print("Progressing next battle animation", Console::eDEBUG);
            m_frameIterator = 0;
            m_nextFrames.removeFirst();
        }
        else if (m_nextFrames.size() > 0)
        {
            if (m_frameIterator < m_nextFrames[0].size())
            {
                m_nextFrameTimer.start();
            }
        }
        else
        {
            m_frameIterator = 0;
        }
    }
}

bool BattleAnimationSprite::getInvertStartPosition() const
{
    return m_invertStartPosition;
}

void BattleAnimationSprite::setInvertStartPosition(bool invertStartPosition)
{
    m_invertStartPosition = invertStartPosition;
}

float BattleAnimationSprite::getDyingEndHp() const
{
    return m_dyingEndHp;
}

void BattleAnimationSprite::setDyingEndHp(float dyingEndHp)
{
    m_dyingEndHp = dyingEndHp;
}

float BattleAnimationSprite::getDyingStartHp() const
{
    return m_dyingStartHp;
}

void BattleAnimationSprite::setDyingStartHp(float dyingStartHp)
{
    m_dyingStartHp = dyingStartHp;
}

bool BattleAnimationSprite::getStartWithFraming() const
{
    return m_startWithFraming;
}

void BattleAnimationSprite::setStartWithFraming(bool startWithFraming)
{
    m_startWithFraming = startWithFraming;
}

void BattleAnimationSprite::addScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs, qint32 shakePauseMs)
{
    auto pOwner = dynamic_cast<BattleAnimation*>(getParent());
    if (pOwner != nullptr)
    {
        pOwner->addScreenshake(startIntensity, decay * Settings::getAnimationSpeed() / Settings::getBattleAnimationSpeed(),
                               durationMs  * Settings::getAnimationSpeed() / Settings::getBattleAnimationSpeed(),
                               delayMs  * Settings::getAnimationSpeed() / Settings::getBattleAnimationSpeed(), shakePauseMs);
    }
}


void BattleAnimationSprite::addSpriteScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs, qint32 shakePauseMs)
{
    oxygine::spTween tween = oxygine::createTween(TweenScreenshake(startIntensity, decay / Settings::getBattleAnimationSpeed(), oxygine::timeMS(shakePauseMs)),
                                                                   oxygine::timeMS(static_cast<qint64>(durationMs / Settings::getBattleAnimationSpeed())), 1, false, oxygine::timeMS(static_cast<qint64>(delayMs / Settings::getBattleAnimationSpeed())));
    getParent()->addTween(tween);
}

void BattleAnimationSprite::addBattleViewScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs, qint32 shakePauseMs)
{
    auto pOwner = dynamic_cast<BattleAnimation*>(getParent());
    if (pOwner != nullptr)
    {
        pOwner->addBattleViewScreenshake(startIntensity, decay, durationMs, delayMs, shakePauseMs);
    }
}
