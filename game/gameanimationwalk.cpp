#include "gameanimationwalk.h"

#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"

#include "coreengine/console.h"

#include "game/player.h"

#include "game/unit.h"
#include "game/co.h"
#include "game/gameanimationfactory.h"

GameAnimationWalk::GameAnimationWalk(Unit* pUnit, QVector<QPoint> movePath)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime)),
      m_pUnit(pUnit),
      m_movePath(movePath)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pUnit->setUnitVisible(false);
}

bool GameAnimationWalk::onFinished()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Player* pPlayer = GameMap::getInstance()->getCurrentViewPlayer();
    if (!m_pUnit->isStealthed(pPlayer))
    {
        m_pUnit->setUnitVisible(true);
    }
    bool ret = GameAnimation::onFinished();
    pApp->continueThread();
    return ret;
}

GameEnums::Directions GameAnimationWalk::getMovementDirection(qint32 x, qint32 y, qint32 x2, qint32 y2)
{
    if (x == x2)
    {
        if (y < y2)
        {
            return GameEnums::Directions_South;
        }
        else if (y > y2)
        {
            return GameEnums::Directions_North;
        }
    }
    else if (y == y2)
    {
        if (x < x2)
        {
            return GameEnums::Directions_East;
        }
        else if (x > x2)
        {
            return GameEnums::Directions_West;
        }
    }
    return GameEnums::Directions_None;
}

void GameAnimationWalk::loadSprite(QString spriteID, bool addPlayerColor, float scaling)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        Player* pPlayer = GameMap::getInstance()->getCurrentViewPlayer();
        oxygine::spSprite pSprite = new oxygine::Sprite();

        oxygine::spTweenQueue queueAnimating = new oxygine::TweenQueue();
        oxygine::spTweenQueue queueMoving = new oxygine::TweenQueue();
        pSprite->setPosition(m_pUnit->getX() * GameMap::Imagesize - static_cast<qint32>((pAnim->getWidth() * scaling - GameMap::Imagesize) / 2.0f),
                             m_pUnit->getY() * GameMap::Imagesize - static_cast<qint32>((pAnim->getHeight() * scaling - GameMap::Imagesize) / 2.0f));
        if (m_pUnit->isStealthed(pPlayer))
        {
            setVisible(false);
        }
        pSprite->setScale(scaling);
        if (m_movePath.size() > 1)
        {
            for (qint32 i = m_movePath.size() - 2; i >= 0; i--)
            {
                qint32 x = 0;
                qint32 y = 0;
                GameEnums::Directions direction = GameEnums::Directions_None;
                if (i == m_movePath.size() - 1)
                {
                    direction = getMovementDirection(m_pUnit->getX(), m_pUnit->getY(), m_movePath[i].x(), m_movePath[i].y());
                }
                else
                {
                    direction = getMovementDirection(m_movePath[i + 1].x(), m_movePath[i + 1].y(), m_movePath[i].x(), m_movePath[i].y());
                }
                x = m_movePath[i].x() * GameMap::Imagesize - static_cast<qint32>((pAnim->getWidth() * scaling - GameMap::Imagesize) / 2.0f);
                y = m_movePath[i].y() * GameMap::Imagesize - static_cast<qint32>((pAnim->getHeight() * scaling - GameMap::Imagesize) / 2.0f);

                oxygine::Tween* tween1 = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(x, y)), static_cast<qint32>(m_frameTime) * pAnim->getRows(), 1);
                // toggle visibility of the unit
                if (i - 1 >= 0)
                {
                    bool isVisible = true;
                    if (m_pUnit->isStealthed(pPlayer, false, m_movePath[i - 1].x(), m_movePath[i - 1].y()))
                    {
                        isVisible = false;
                    }
                    tween1->addDoneCallback([=](oxygine::Event *)
                    {
                        this->setVisible(isVisible);
                    });
                }

                queueMoving->add(tween1);

                int row = 0;
                switch (direction)
                {
                    case GameEnums::Directions_North:
                    {
                        row = 1;
                        break;
                    }
                    case GameEnums::Directions_South:
                    {
                        row = 0;
                        break;
                    }
                    case GameEnums::Directions_East:
                    {
                        row = 2;
                        break;
                    }
                    case GameEnums::Directions_West:
                    {
                        row = 3;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, row), m_frameTime * pAnim->getRows(), 1);
                queueAnimating->add(tween);
                if (i == 0)
                {
                    queueMoving->addDoneCallback([=](oxygine::Event *)->void
                    {
                        emit sigFinished();
                    });
                }
            }
        }
        //
        if (m_movePath.size() <= 1)
        {
            queueMoving->addDoneCallback([=](oxygine::Event *)->void
            {
                emit sigFinished();
            });
        }
        pSprite->addTween(queueMoving);
        pSprite->addTween(queueAnimating);

        // repaint the unit?
        if (addPlayerColor)
        {
            QColor color = m_pUnit->getOwner()->getColor();
            oxygine::Sprite::TweenColor tweenColor(oxygine::Color(static_cast<quint8>(color.red()), static_cast<quint8>(color.green()), static_cast<quint8>(color.blue()), 255));
            oxygine::spTween tween = oxygine::createTween(tweenColor, 1);

            pSprite->addTween(tween);
        }
        this->addChild(pSprite);
    }
    else
    {
        Console::print("Unable to load unit walk sprite: " + spriteID, Console::eERROR);
    }
}
