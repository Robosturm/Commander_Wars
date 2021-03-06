#include "game/gameanimationcapture.h"
#include "game/gamemap.h"

#include "resource_management/gameanimationmanager.h"

#include "coreengine/console.h"
#include "coreengine/audiothread.h"
#include "coreengine/mainapp.h"

const qint32 GameAnimationCapture::capturingFactor = 10;
const qint32 GameAnimationCapture::jumpSprites = 3;
const qint32 GameAnimationCapture::ayeAyeSprites = 2;
const qint32 GameAnimationCapture::jumpingCount = 3;

GameAnimationCapture::GameAnimationCapture(qint32 startPoints, qint32 endPoints, qint32 maxPoints)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime)),
      m_startPoints(startPoints),
      m_endPoints(endPoints),
      m_maxPoints(maxPoints)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_frameTime = GameMap::frameTime / Settings::getCaptureAnimationSpeed();
}

void GameAnimationCapture::addBuildingSprite(QString spriteID, Player* startPlayer, Player* capturedPlayer, GameEnums::Recoloring mode)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
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

        oxygine::spTweenQueue queueAnimating = new oxygine::TweenQueue();
        oxygine::spTweenQueue queueMoving = new oxygine::TweenQueue();
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
        oxygine::timeMS jumpingDuration = oxygine::timeMS(m_frameTime * jumpSprites * jumpingCount + m_frameTime * ayeAyeSprites);
        // dummy tween doing nothing except sync the animation
        oxygine::spTween dummyTween = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f - startPercent), jumpingDuration, 1);
        queueAnimating->add(dummyTween);
        oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f - percentDone), oxygine::timeMS(capturingFactor * m_frameTime), 1);
        queueAnimating->add(tween);
        // dummy tween doing nothing except sync the animation
        oxygine::spTween dummyTween2 = oxygine::createTween(oxygine::Actor::TweenY(startPosition), jumpingDuration, 1);
        queueMoving->add(dummyTween2);
        oxygine::spTween tween2 = oxygine::createTween(oxygine::Actor::TweenY(buildingOffsetY + percentDone * movingHeight), oxygine::timeMS(capturingFactor * m_frameTime), 1);
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
                }
                Mainapp::getInstance()->getAudioThread()->playSound("capture.wav");
            });
            oxygine::spTween tween3 = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f), oxygine::timeMS(capturingFactor * m_frameTime), 1, false);
            queueAnimating->add(tween3);

            oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(buildingOffsetY), oxygine::timeMS(capturingFactor * m_frameTime), 1, false);
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
        oxygine::spSprite pSprite = new oxygine::Sprite();
        if (pAnim->getTotalFrames() > 1)
        {
            // jumping
            oxygine::spTweenQueue queueAnimating = new oxygine::TweenQueue();
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, 0, jumpSprites - 1), oxygine::timeMS(jumpSprites * m_frameTime), jumpingCount);
            queueAnimating->add(tween);
            // cool aye aye sir
            oxygine::spTween tween2 = oxygine::createTween(oxygine::TweenAnim(pAnim, jumpSprites, jumpSprites + ayeAyeSprites - 1), oxygine::timeMS(ayeAyeSprites * m_frameTime), 1);
            queueAnimating->add(tween2);
            // going down on the building
            float endPosition = unitOffsetY + percentDone * buildingHigh;
            oxygine::spTween tween3 = oxygine::createTween(oxygine::Actor::TweenY(endPosition), oxygine::timeMS(capturingFactor * m_frameTime), 1);
            queueAnimating->add(tween3);
            // maybe going up again as well
            if (m_endPoints == 0)
            {
                oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(unitOffsetY), oxygine::timeMS(capturingFactor * m_frameTime), 1);
                queueAnimating->add(tween4);
                oxygine::spTween tween5 = oxygine::createTween(oxygine::Actor::TweenY(unitOffsetY), oxygine::timeMS(m_frameTime), 1);
                queueAnimating->add(tween5);
                if(!finishQueued)
                {
                    finishQueued = true;
                    tween5->addDoneCallback([=](oxygine::Event *)->void
                    {
                        emitFinished();
                    });
                }
            }
            else
            {
                if(!finishQueued)
                {
                    finishQueued = true;
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
        oxygine::spSprite pSprite = new oxygine::Sprite();
        pSprite->setResAnim(pAnim);
        pSprite->setPriority(0);
        this->addChild(pSprite);
    }
    else
    {
        Console::print("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}
