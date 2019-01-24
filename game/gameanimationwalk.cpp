#include "gameanimationwalk.h"

#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"

#include "coreengine/console.h"

#include "game/player.h"

#include "game/unit.h"

#include "game/gameanimationfactory.h"

GameAnimationWalk::GameAnimationWalk(Unit* pUnit, QVector<QPoint> movePath)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime)),
      m_pUnit(pUnit),
      m_movePath(movePath)
{
    m_pUnit->setVisible(false);
}

void GameAnimationWalk::onFinished()
{
    m_pUnit->setVisible(true);
    GameAnimation::onFinished();
}

GameMap::Directions GameAnimationWalk::getMovementDirection(qint32 x, qint32 y, qint32 x2, qint32 y2)
{
    if (x == x2)
    {
        if (y < y2)
        {
            return GameMap::Directions::South;
        }
        else if (y > y2)
        {
            return GameMap::Directions::North;
        }
    }
    else if (y == y2)
    {
        if (x < x2)
        {
            return GameMap::Directions::East;
        }
        else if (x > x2)
        {
            return GameMap::Directions::West;
        }
    }
    return GameMap::Directions::None;
}

void GameAnimationWalk::loadSprite(QString spriteID, bool addPlayerColor, float scaling)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();

        oxygine::spTweenQueue queueAnimating = new oxygine::TweenQueue();
        oxygine::spTweenQueue queueMoving = new oxygine::TweenQueue();

        for (qint32 i = m_movePath.size() - 1; i >= 0; i--)
        {
            qint32 x = 0;
            qint32 y = 0;
            GameMap::Directions direction = GameMap::Directions::None;
            if (i == m_movePath.size() - 1)
            {
                direction = getMovementDirection(m_pUnit->getX(), m_pUnit->getY(), m_movePath[i].x(), m_movePath[i].y());
            }
            else
            {
                direction = getMovementDirection(m_movePath[i + 1].x(), m_movePath[i + 1].y(), m_movePath[i].x(), m_movePath[i].y());
            }
            x = m_movePath[i].x() * GameMap::Imagesize;
            y = m_movePath[i].y() * GameMap::Imagesize;
            queueMoving->add(oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(x, y)), GameMap::frameTime * pAnim->getRows(), 1));

            int row = 0;
            switch (direction)
            {
                case GameMap::Directions::North:
                {
                    row = 1;
                    break;
                }
                case GameMap::Directions::South:
                {
                    row = 0;
                    break;
                }
                case GameMap::Directions::East:
                {
                    row = 2;
                    break;
                }
                case GameMap::Directions::West:
                {
                    row = 3;
                    break;
                }
                default:
                {
                    break;
                }
            }
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, row), GameMap::frameTime * pAnim->getRows(), 1);
            queueAnimating->add(tween);
            if (i == 0)
            {
                queueMoving->addDoneCallback([=](oxygine::Event *)->void
                {
                    emit sigFinished();
                });
            }
        }
        pSprite->setPosition(m_pUnit->getX() * GameMap::Imagesize, m_pUnit->getY() * GameMap::Imagesize);
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
        pSprite->setScale(scaling);
        pSprite->setPosition(-pSprite->getScaledWidth() / 2, -pSprite->getScaledHeight() / 2);
        this->addChild(pSprite);
    }
    else
    {
        Console::print("Unable to load unit walk sprite: " + spriteID, Console::eERROR);
    }
}
