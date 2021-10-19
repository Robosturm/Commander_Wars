#include "game/gameanimation/gameanimationcapture.h"
#include "game/gamemap.h"

#include "resource_management/gameanimationmanager.h"

#include "coreengine/console.h"
#include "coreengine/audiothread.h"
#include "coreengine/mainapp.h"

#include "spritingsupport/spritecreator.h"

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
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_frameTime = GameMap::frameTime / Settings::getCaptureAnimationSpeed();
}

void GameAnimationCapture::addBuildingSprite(QString spriteID, Player* startPlayer, Player* capturedPlayer, GameEnums::Recoloring mode)
{
    Mainapp* pApp = Mainapp::getInstance();
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID, oxygine::ep_ignore_error);
    if (mode == GameEnums::Recoloring_Mask)
    {
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "GameAnimationCapture::addBuildingSprite " + spriteID + " recloring mode mask not supported!");
    }
    if (pAnim != nullptr)
    {

        if (m_buildingSprites.get() == nullptr)
        {
            m_buildingSprites = oxygine::spSprite::create();
            m_buildingSprites->setDestRecModifier(oxygine::RectF(0.501f, 0.501f, 0.0f, 0.0f));
            addChild(m_buildingSprites);
            createBuildingAnimation(pAnim, startPlayer, capturedPlayer);
            m_buildingResAnim = oxygine::spSingleResAnim::create();
            m_captureBuildingResAnim = oxygine::spSingleResAnim::create();
        }
        QString path = Settings::getUserPath() + pAnim->getResPath();
        if (!QFile::exists(path))
        {
            path = oxygine::Resource::RCC_PREFIX_PATH + pAnim->getResPath();
        }
        QImage preCaptureImage(path);
        QImage captureImage(path);
        getRecoloredImage(startPlayer, capturedPlayer, mode,
                          preCaptureImage, captureImage);
        pApp->loadResAnim(m_buildingResAnim, m_buildingImage, 1, 1, 1.0f, false);
        pApp->loadResAnim(m_captureBuildingResAnim, m_captureBuildingImage, 1, 1, 1.0f, false);
        m_buildingSprites->setResAnim(m_buildingResAnim.get());
    }
    else
    {
        CONSOLE_PRINT("Unable to load building sprite: " + spriteID, Console::eDEBUG);
    }
}

void GameAnimationCapture::getRecoloredImage(Player* startPlayer, Player* capturedPlayer, GameEnums::Recoloring mode,
                                             QImage & preCaptureImage, QImage & captureImage)
{
    if (mode == GameEnums::Recoloring_Table ||
        mode == GameEnums::Recoloring_Matrix)
    {
        if (startPlayer == nullptr)
        {
            QImage table = Player::getNeutralTableImage();
            m_buildingImage = SpriteCreator::recolorImageWithTable(preCaptureImage, table);
        }
        else
        {
            QImage table = startPlayer->getColorTable();
            m_buildingImage = SpriteCreator::recolorImageWithTable(preCaptureImage, table);
        }
        if (capturedPlayer != nullptr)
        {
            QImage table = capturedPlayer->getColorTable();
            m_captureBuildingImage = SpriteCreator::recolorImageWithTable(captureImage, table);
        }
    }
    else if (mode == GameEnums::Recoloring_None)
    {
        m_buildingImage = SpriteCreator::mergeImages(m_buildingImage, preCaptureImage);
        m_captureBuildingImage = SpriteCreator::mergeImages(m_captureBuildingImage, captureImage);
    }
}

void GameAnimationCapture::createBuildingAnimation(oxygine::ResAnim* pAnim, Player* startPlayer, Player* capturedPlayer)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        float endPoints = m_endPoints;
        if (m_endPoints == 0)
        {
            endPoints = m_maxPoints;
        }

        m_BuildingStartPos = m_BuildingEndPos - pAnim->getHeight();
        m_buildingSprites->setY(m_BuildingStartPos);

        float startPercent = static_cast<float>(m_startPoints) / static_cast<float>(m_maxPoints);
        float percentDone = static_cast<float>(endPoints) / static_cast<float>(m_maxPoints + 1.5f);
        float movingHeight = pAnim->getHeight();
        float startPosition = (startPercent) * movingHeight + m_BuildingStartPos;
        m_buildingSprites->setPosition(0, startPosition);
        m_buildingSprites->setScaleY(1.0f - startPercent);
        oxygine::spTweenQueue queueAnimating = oxygine::spTweenQueue::create();
        oxygine::spTweenQueue queueMoving = oxygine::spTweenQueue::create();
        // going down of the building
        oxygine::timeMS jumpingDuration = oxygine::timeMS(m_frameTime * m_jumpSprites * m_jumpingCount + m_frameTime * m_ayeAyeSprites);
        // dummy tween doing nothing except sync the animation
        oxygine::spTween dummyTween = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f - startPercent), jumpingDuration, 1);
        dummyTween->setDoneCallback([=](oxygine::Event *)
        {
            Mainapp::getInstance()->getAudioThread()->playSound("capture_down.wav");
        });
        queueAnimating->add(dummyTween);
        oxygine::spTween tween = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f - percentDone), oxygine::timeMS(m_capturingFactor * m_frameTime), 1);
        queueAnimating->add(tween);

        // dummy tween doing nothing except sync the animation
        oxygine::spTween dummyTween2 = oxygine::createTween(oxygine::Actor::TweenY(startPosition), jumpingDuration, 1);
        queueMoving->add(dummyTween2);
        oxygine::spTween tween2 = oxygine::createTween(oxygine::Actor::TweenY(percentDone * movingHeight + m_BuildingStartPos), oxygine::timeMS(m_capturingFactor * m_frameTime), 1);
        queueMoving->add(tween2);

        if (m_endPoints == 0)
        {
            oxygine::Sprite* pSrite = m_buildingSprites.get();
            tween2->addDoneCallback([=](oxygine::Event *)
            {
                pSrite->setResAnim(m_captureBuildingResAnim.get());
                if (pMap->getCurrentViewPlayer()->isEnemy(capturedPlayer))
                {
                    Mainapp::getInstance()->getAudioThread()->playSound("capture_enemy.wav");
                }
                else
                {
                    Mainapp::getInstance()->getAudioThread()->playSound("capture_ally.wav");
                }
            });
            oxygine::spTween tween3 = oxygine::createTween(oxygine::Actor::TweenScaleY(1.0f), oxygine::timeMS(m_capturingFactor * m_frameTime), 1, false);
            queueAnimating->add(tween3);

            oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(m_BuildingStartPos), oxygine::timeMS(m_capturingFactor * m_frameTime), 1, false);
            queueMoving->add(tween4);
        }
        m_buildingSprites->addTween(queueAnimating);
        m_buildingSprites->addTween(queueMoving);
    }
}

void GameAnimationCapture::addSoldierSprite(QString spriteID, Player*  pPlayer, GameEnums::Recoloring mode)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID, oxygine::ep_ignore_error);
    if (pAnim != nullptr)
    {
        float endPoints = m_endPoints;
        if (m_endPoints == 0)
        {
            endPoints = m_maxPoints;
        }
        const float unitOffsetY = -pAnim->getHeight() + m_BuildingStartPos;
        const float percentDone = static_cast<float>(endPoints) / static_cast<float>(m_maxPoints + 1.5f);
        const float startPercentDone = (static_cast<float>(m_startPoints) / static_cast<float>(m_maxPoints));
        constexpr qint32 unitOffset = 6;
        float buildingHigh = m_BuildingEndPos - m_BuildingStartPos;
        float startPosition = unitOffsetY + qCeil(unitOffset * (1.0f - startPercentDone)) + qCeil(startPercentDone * buildingHigh);
        float endPosition = unitOffsetY + qCeil(unitOffset * ( 1.0f - percentDone)) + qCeil(percentDone * buildingHigh);

        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim->getTotalFrames() > 1)
        {
            // jumping
            oxygine::spTweenQueue queueAnimating = oxygine::spTweenQueue::create();
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, 0, m_jumpSprites - 1), oxygine::timeMS(m_jumpSprites * m_frameTime), m_jumpingCount);
            for (qint32 i = 0; i < m_jumpingCount; ++i)
            {
                addSound("capture_jump.wav", 1, m_jumpSprites * m_frameTime * i);
            }
            queueAnimating->add(tween);
            // cool aye aye sir
            oxygine::spTween tween2 = oxygine::createTween(oxygine::TweenAnim(pAnim, m_jumpSprites, m_jumpSprites + m_ayeAyeSprites - 1), oxygine::timeMS(m_ayeAyeSprites * m_frameTime), 1);
            queueAnimating->add(tween2);
            // going down on the building
            oxygine::spTween tween3 = oxygine::createTween(oxygine::Actor::TweenY(endPosition), oxygine::timeMS(m_capturingFactor * m_frameTime), 1);
            queueAnimating->add(tween3);
            // maybe going up again as well
            if (m_endPoints == 0)
            {
                oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(unitOffsetY), oxygine::timeMS(m_capturingFactor * m_frameTime), 1);
                queueAnimating->add(tween4);
                if(!m_finishQueued)
                {
                    m_finishQueued = true;
                    tween4->addDoneCallback([=](oxygine::Event *)->void
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
            pSprite->setPriority(20);
        }
        else if (mode == GameEnums::Recoloring_Table ||
                 mode == GameEnums::Recoloring_Matrix)
        {
            bool matrixMode = mode == GameEnums::Recoloring_Matrix;
            pSprite->setColorTable(pPlayer->getColorTableAnim(), matrixMode);
            pSprite->setPriority(20);
        }
        else
        {
            pSprite->setPriority(21);
        }
        pSprite->setPosition(12, startPosition);
        addChild(pSprite);
    }
    else
    {
        CONSOLE_PRINT("Unable to load animation sprite: " + spriteID, Console::eDEBUG);
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
        addChild(pSprite);
    }
    else
    {
        CONSOLE_PRINT("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}
