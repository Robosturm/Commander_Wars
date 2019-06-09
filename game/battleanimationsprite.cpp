#include "battleanimationsprite.h"

#include "coreengine/mainapp.h"

#include "resource_management/battleanimationmanager.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "qcolor.h"

const QString BattleAnimationSprite::standingAnimation = "loadStandingAnimation";

BattleAnimationSprite::BattleAnimationSprite(Unit* pUnit, QString animationType)
    : QObject(),
      m_pUnit(pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    m_Actor = new oxygine::ClipRectActor();
    m_Actor->setSize(127, 192);
    addChild(m_Actor);

    QString function1 = animationType;
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj2;
    QJSValue erg = pApp->getInterpreter()->doFunction("BATTLEANIMATION_" + pUnit->getUnitID(), function1, args1);
}

QPoint BattleAnimationSprite::getUnitPosition(qint32 unitCount, qint32 maxUnitCount)
{
    qint32 x = (unitCount * 70) % 100;
    qint32 y = 17 * (maxUnitCount - unitCount);
    return QPoint(x, y);
}

void BattleAnimationSprite::loadSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset)
{
    BattleAnimationManager* pBattleAnimationManager = BattleAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pBattleAnimationManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        qint32 value = Mainapp::roundUp(m_pUnit->getHpRounded() / 10.0f * maxUnitCount);
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
                QColor color = m_pUnit->getOwner()->getColor();
                oxygine::Sprite::TweenColor tweenColor(oxygine::Color(color.red(), color.green(), color.blue(), 255));
                oxygine::spTween tween = oxygine::createTween(tweenColor, 1);
                pSprite->addTween(tween);
            }
            pSprite->setPosition(position.x() + offset.x(), 192 - position.y() - offset.y() - pAnim->getHeight());
            pSprite->setPriority(i);
            m_Actor->addChild(pSprite);
        }
    }
}
