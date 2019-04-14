#include "game/gameanimationcapture.h"

#include "resource_management/gameanimationmanager.h"

#include "game/gamemap.h"

#include "coreengine/console.h"

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
    Interpreter::setCppOwnerShip(this);
}

void GameAnimationCapture::addBuildingSprite(QString spriteID, QColor startColor, QColor capturedColor, bool addColor)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID.toStdString());
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
        qint32 jumpingDuration = GameMap::frameTime * jumpSprites * jumpingCount + GameMap::frameTime * ayeAyeSprites;
        // dummy tween doing nothing except sync the animation
        oxygine::spTween dummyTween = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f - startPercent), jumpingDuration, 1);
        queueAnimating->add(dummyTween);
        oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f - percentDone), capturingFactor * GameMap::frameTime, 1);
        queueAnimating->add(tween);
        // dummy tween doing nothing except sync the animation
        oxygine::spTween dummyTween2 = oxygine::createTween(oxygine::Actor::TweenY(startPosition), jumpingDuration, 1);
        queueMoving->add(dummyTween2);
        oxygine::spTween tween2 = oxygine::createTween(oxygine::Actor::TweenY(buildingOffsetY + percentDone * movingHeight), capturingFactor * GameMap::frameTime, 1);
        queueMoving->add(tween2);
        if (m_endPoints == 0)
        {
            if (!addColor)
            {
                capturedColor.setRgb(255, 255, 255);
            }
            oxygine::Sprite::TweenColor tweenColor(oxygine::Color(static_cast<quint8>(capturedColor.red()), static_cast<quint8>(capturedColor.green()), static_cast<quint8>(capturedColor.blue()), 255));
            oxygine::Sprite::TweenColor tweenColor1(oxygine::Color(static_cast<quint8>(capturedColor.red()), static_cast<quint8>(capturedColor.green()), static_cast<quint8>(capturedColor.blue()), 255));
            oxygine::spTween tween5 = oxygine::createTween(tweenColor, 1);
            oxygine::spTween tween6 = oxygine::createTween(tweenColor1, 1);
            queueAnimating->add(tween5);
            queueMoving->add(tween6);
            oxygine::spTween tween3 = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f), capturingFactor * GameMap::frameTime, 1, false);
            queueAnimating->add(tween3);

            oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(buildingOffsetY), capturingFactor * GameMap::frameTime, 1, false);
            queueMoving->add(tween4);
        }



        pSprite->addTween(queueAnimating);
        pSprite->addTween(queueMoving);

        if (addColor)
        {
            oxygine::Sprite::TweenColor tweenColor(oxygine::Color(static_cast<quint8>(startColor.red()), static_cast<quint8>(startColor.green()), static_cast<quint8>(startColor.blue()), 255));
            oxygine::spTween tween7 = oxygine::createTween(tweenColor, 1);
            pSprite->addTween(tween7);
        }


        pSprite->setPriority(10);


        this->addChild(pSprite);
    }
}

void GameAnimationCapture::addSoldierSprite(QString spriteID, QColor color, bool addColor)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID.toStdString());
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
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, 0, jumpSprites - 1), jumpSprites * GameMap::frameTime, jumpingCount);
            queueAnimating->add(tween);
            // cool aye aye sir
            oxygine::spTween tween2 = oxygine::createTween(oxygine::TweenAnim(pAnim, jumpSprites, jumpSprites + ayeAyeSprites - 1), ayeAyeSprites * GameMap::frameTime, 1);
            queueAnimating->add(tween2);
            // going down on the building
            float endPosition = unitOffsetY + percentDone * buildingHigh;
            oxygine::spTween tween3 = oxygine::createTween(oxygine::Actor::TweenY(endPosition), capturingFactor * GameMap::frameTime, 1);
            queueAnimating->add(tween3);
            // maybe going up again as well
            if (m_endPoints == 0)
            {
                oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(unitOffsetY), capturingFactor * GameMap::frameTime, 1);
                queueAnimating->add(tween4);
                oxygine::spTween tween5 = oxygine::createTween(oxygine::Actor::TweenY(unitOffsetY), GameMap::frameTime, 1);
                queueAnimating->add(tween5);
                tween5->addDoneCallback([=](oxygine::Event *)->void
                {
                    emit sigFinished();
                });
            }
            else
            {
                tween3->addDoneCallback([=](oxygine::Event *)->void
                {
                    emit sigFinished();
                });
            }

            pSprite->addTween(queueAnimating);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }

        if (addColor)
        {
            oxygine::Sprite::TweenColor tweenColor(oxygine::Color(static_cast<quint8>(color.red()), static_cast<quint8>(color.green()), static_cast<quint8>(color.blue()), 255));
            oxygine::spTween tween = oxygine::createTween(tweenColor, 1);
            pSprite->addTween(tween);
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
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID.toStdString());
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
