#include "game/gameanimation/gameanimationcapture.h"
#include "game/gamemap.h"

#include "resource_management/gameanimationmanager.h"

#include "coreengine/console.h"
#include "coreengine/audiothread.h"
#include "coreengine/mainapp.h"

const qint32 GameAnimationCapture::m_capturingFactor = 7;
const qint32 GameAnimationCapture::m_jumpSprites = 3;
const qint32 GameAnimationCapture::m_ayeAyeSprites = 2;
const qint32 GameAnimationCapture::m_jumpingCount = 3;

GameAnimationCapture::GameAnimationCapture(qint32 startPoints, qint32 endPoints, qint32 maxPoints)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime)),
      m_startPoints(startPoints),
      m_endPoints(endPoints),
      m_maxPoints(maxPoints)
{
    setObjectName("GameAnimationCapture");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_frameTime = GameMap::frameTime / Settings::getCaptureAnimationSpeed();
}

void GameAnimationCapture::addBuildingSprite(QString spriteID, Player* startPlayer, Player* capturedPlayer, GameEnums::Recoloring mode)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    GameMap* pMap = GameMap::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        float endPoints = m_endPoints;
        if (m_endPoints == 0)
        {
            endPoints = m_maxPoints;
        }
        float startPercent = static_cast<float>(m_startPoints) / static_cast<float>(m_maxPoints);
        float percentDone = static_cast<float>(endPoints) / static_cast<float>(m_maxPoints);
        float buildingOffsetY = -12;
        float movingHeight = pAnim->getHeight();
        float startPosition = buildingOffsetY + (startPercent) * movingHeight;

        oxygine::spTweenQueue queueAnimating = oxygine::spTweenQueue::create();
        oxygine::spTweenQueue queueMoving = oxygine::spTweenQueue::create();
        // hit test to get building heigth
        pSprite->setResAnim(pAnim);
        pSprite->setPosition(0, startPosition);
        pSprite->setScaleY(1.0f - startPercent);
        float width = pAnim->getWidth();
        bool on = false;
        for (qint32 i = 0; i < pAnim->getHeight(); i++)
        {
            bool testErg = pSprite->isOn(oxygine::Vector2(width / 2.0f, i), 1.0f);
            if (!testErg)
            {
                if (on)
                {
                    m_BuildingEndPos = i - 3;
                }
                else
                {
                    m_BuildingStartPos = i;
                }
            }
            else
            {
                on = true;
            }
        }

        // going down of the building
        oxygine::timeMS jumpingDuration = oxygine::timeMS(m_frameTime * m_jumpSprites * m_jumpingCount + m_frameTime * m_ayeAyeSprites);
        // dummy tween doing nothing except sync the animation
        oxygine::spTween dummyTween = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f - startPercent), jumpingDuration, 1);
        dummyTween->setDoneCallback([=](oxygine::Event *)
        {
            if (mode == GameEnums::Recoloring_Table)
            {
                Mainapp::getInstance()->getAudioThread()->playSound("capture_down.wav");
            }
        });
        queueAnimating->add(dummyTween);
        oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f - percentDone), oxygine::timeMS(m_capturingFactor * m_frameTime), 1);
        queueAnimating->add(tween);
        // dummy tween doing nothing except sync the animation
        oxygine::spTween dummyTween2 = oxygine::createTween(oxygine::Actor::TweenY(startPosition), jumpingDuration, 1);
        queueMoving->add(dummyTween2);
        oxygine::spTween tween2 = oxygine::createTween(oxygine::Actor::TweenY(buildingOffsetY + percentDone * movingHeight), oxygine::timeMS(m_capturingFactor * m_frameTime), 1);
        queueMoving->add(tween2);
        if (m_endPoints == 0)
        {
            QColor capturedColor = capturedPlayer->getColor();
            if (mode != GameEnums::Recoloring_Mask)
            {
                capturedColor.setRgb(255, 255, 255);
            }
            oxygine::Sprite::TweenColor tweenColor(capturedColor);
            oxygine::Sprite::TweenColor tweenColor1(capturedColor);
            oxygine::spTween tween5 = oxygine::createTween(tweenColor, oxygine::timeMS(1));
            oxygine::spTween tween6 = oxygine::createTween(tweenColor1, oxygine::timeMS(1));
            queueAnimating->add(tween5);
            queueMoving->add(tween6);
            tween6->addDoneCallback([=](oxygine::Event *)
            {
                if (mode == GameEnums::Recoloring_Table)
                {
                    pSprite->setColorTable(capturedPlayer->getColorTableAnim());
                    if (pMap->getCurrentViewPlayer()->isEnemy(capturedPlayer))
                    {
                        Mainapp::getInstance()->getAudioThread()->playSound("capture_enemy.wav");
                    }
                    else
                    {
                        Mainapp::getInstance()->getAudioThread()->playSound("capture_ally.wav");
                    }
                }
            });
            oxygine::spTween tween3 = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f), oxygine::timeMS(m_capturingFactor * m_frameTime), 1, false);
            queueAnimating->add(tween3);

            oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(buildingOffsetY), oxygine::timeMS(m_capturingFactor * m_frameTime), 1, false);
            queueMoving->add(tween4);
        }
        pSprite->addTween(queueAnimating);
        pSprite->addTween(queueMoving);

        if (mode == GameEnums::Recoloring_Mask)
        {
            if (startPlayer == nullptr)
            {
                pSprite->setColor(QColor(255, 255, 255));
            }
            else
            {
                pSprite->setColor(startPlayer->getColor());
            }
        }
        else if (mode == GameEnums::Recoloring_Table)
        {
            if (startPlayer == nullptr)
            {
                pSprite->setColorTable(Player::getNeutralTableAnim());
            }
            else
            {
                pSprite->setColorTable(startPlayer->getColorTableAnim());
            }
        }


        pSprite->setPriority(10);


        this->addChild(pSprite);
    }
}

void GameAnimationCapture::addSoldierSprite(QString spriteID, Player*  pPlayer, GameEnums::Recoloring mode)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        float endPoints = m_endPoints;
        if (m_endPoints == 0)
        {
            endPoints = m_maxPoints;
        }
        float unitOffsetY = -pAnim->getHeight() - 8 + m_BuildingStartPos;
        float buildingHigh = m_BuildingEndPos - m_BuildingStartPos;
        float percentDone = static_cast<float>(endPoints) / static_cast<float>(m_maxPoints);
        float startPosition = unitOffsetY + (static_cast<float>(m_startPoints) / static_cast<float>(m_maxPoints)) * buildingHigh;
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim->getTotalFrames() > 1)
        {
            // jumping
            oxygine::spTweenQueue queueAnimating = oxygine::spTweenQueue::create();
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, 0, m_jumpSprites - 1), oxygine::timeMS(m_jumpSprites * m_frameTime), m_jumpingCount);
            for (qint32 i = 0; i < m_jumpingCount; ++i)
            {
                addSound("capture_jump.wav", 1, "resources/sounds/", m_jumpSprites * m_frameTime * i);
            }
            queueAnimating->add(tween);
            // cool aye aye sir
            oxygine::spTween tween2 = oxygine::createTween(oxygine::TweenAnim(pAnim, m_jumpSprites, m_jumpSprites + m_ayeAyeSprites - 1), oxygine::timeMS(m_ayeAyeSprites * m_frameTime), 1);
            queueAnimating->add(tween2);
            // going down on the building
            float endPosition = unitOffsetY + percentDone * buildingHigh;
            oxygine::spTween tween3 = oxygine::createTween(oxygine::Actor::TweenY(endPosition), oxygine::timeMS(m_capturingFactor * m_frameTime), 1);
            queueAnimating->add(tween3);
            // maybe going up again as well
            if (m_endPoints == 0)
            {
                oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(unitOffsetY), oxygine::timeMS(m_capturingFactor * m_frameTime), 1);
                queueAnimating->add(tween4);
                oxygine::spTween tween5 = oxygine::createTween(oxygine::Actor::TweenY(unitOffsetY), oxygine::timeMS(m_frameTime), 1);
                queueAnimating->add(tween5);
                if(!m_finishQueued)
                {
                    m_finishQueued = true;
                    tween5->addDoneCallback([=](oxygine::Event *)->void
                    {
                        emitFinished();
                    });
                }
            }
            else
            {
                if(!m_finishQueued)
                {
                    m_finishQueued = true;
                    tween3->addDoneCallback([=](oxygine::Event *)->void
                    {
                        emitFinished();
                    });
                }
            }

            pSprite->addTween(queueAnimating);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }

        if (mode == GameEnums::Recoloring_Mask)
        {
            pSprite->setColor(pPlayer->getColor());
        }
        else if (mode == GameEnums::Recoloring_Table)
        {
            pSprite->setColorTable(pPlayer->getColorTableAnim());
        }
        pSprite->setPosition(12, startPosition);
        pSprite->setPriority(20);
        this->addChild(pSprite);
    }
    else
    {
        Console::print("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}

void GameAnimationCapture::addBackgroundSprite(QString spriteID)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        pSprite->setResAnim(pAnim);
        pSprite->setPriority(0);
        this->addChild(pSprite);
    }
    else
    {
        Console::print("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}
