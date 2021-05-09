#include "resource_management/unitspritemanager.h"

#include "coreengine/console.h"
#include "coreengine/audiothread.h"

#include "game/player.h"
#include "game/gameanimation/gameanimationwalk.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "game/co.h"
#include "game/gameanimation/gameanimationfactory.h"

GameAnimationWalk::GameAnimationWalk(Unit* pUnit, QVector<QPoint> movePath)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime)),
      m_pUnit(pUnit),
      m_movePath(movePath)
{
    setObjectName("GameAnimationWalk");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pUnit->setUnitVisible(false);
    m_frameTime = static_cast<quint32>(GameMap::frameTime / Settings::getWalkAnimationSpeed());
}

bool GameAnimationWalk::onFinished(bool skipping)
{    
    Player* pPlayer = GameMap::getInstance()->getCurrentViewPlayer();
    Mainapp::getInstance()->getAudioThread()->stopAllSounds();
    if (!m_pUnit->isStealthed(pPlayer))
    {
        m_pUnit->setUnitVisible(true);
    }
    bool ret = GameAnimation::onFinished(skipping);
    
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
    return GameEnums::Directions_East;
}

void GameAnimationWalk::loadSprite(QString spriteID, bool addPlayerColor, float scaling)
{
    if (addPlayerColor)
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_Mask, scaling);
    }
    else
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_None, scaling);
    }
}

void GameAnimationWalk::loadSpriteV2(QString spriteID, GameEnums::Recoloring mode, float scaling)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        Player* pPlayer = GameMap::getInstance()->getCurrentViewPlayer();
        oxygine::spSprite pSprite = oxygine::spSprite::create();

        oxygine::spTweenQueue queueAnimating = oxygine::spTweenQueue::create();
        oxygine::spTweenQueue queueMoving = oxygine::spTweenQueue::create();
        pSprite->setPosition(m_pUnit->Unit::getX() * GameMap::getImageSize() - static_cast<qint32>((pAnim->getWidth() * scaling - GameMap::getImageSize()) / 2.0f),
                             m_pUnit->Unit::getY() * GameMap::getImageSize() - static_cast<qint32>((pAnim->getHeight() * scaling - GameMap::getImageSize()) / 2.0f));
        setSize(pAnim->getSize());
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
                    direction = getMovementDirection(m_pUnit->Unit::getX(), m_pUnit->Unit::getY(), m_movePath[i].x(), m_movePath[i].y());
                }
                else
                {
                    direction = getMovementDirection(m_movePath[i + 1].x(), m_movePath[i + 1].y(), m_movePath[i].x(), m_movePath[i].y());
                }
                x = m_movePath[i].x() * GameMap::getImageSize() - static_cast<qint32>((pAnim->getWidth() * scaling - GameMap::getImageSize()) / 2.0f);
                y = m_movePath[i].y() * GameMap::getImageSize() - static_cast<qint32>((pAnim->getHeight() * scaling - GameMap::getImageSize()) / 2.0f);

                oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(x, y)), oxygine::timeMS(m_frameTime * pAnim->getRows()), 1);
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

                qint32 row = 0;
                if (pAnim->getRows() > 1)
                {
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
                }
                oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, row), oxygine::timeMS(m_frameTime * pAnim->getRows()), 1);
                queueAnimating->add(tween);
                if (i == 0)
                {
                    if(!m_finishQueued)
                    {
                        m_finishQueued = true;
                        queueMoving->addDoneCallback([=](oxygine::Event *)->void
                        {
                            emitFinished();
                        });
                    }
                }
            }
        }
        //
        if (m_movePath.size() <= 1)
        {
            if(!m_finishQueued)
            {
                m_finishQueued = true;
                queueMoving->addDoneCallback([=](oxygine::Event *)->void
                {
                    emitFinished();
                });
            }
        }
        pSprite->addTween(queueMoving);
        pSprite->addTween(queueAnimating);

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

        this->addChild(pSprite);
    }
    else
    {
        Console::print("Unable to load unit walk sprite: " + spriteID, Console::eERROR);
        emitFinished();
    }
}
