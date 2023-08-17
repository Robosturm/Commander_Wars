#include "3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/rotatingsprite.h"

#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

#include "menue/basegamemenu.h"

#include "coreengine/interpreter.h"
#include "coreengine/audiomanager.h"

#include "game/gameanimation/gameanimationpower.h"
#include "game/co.h"
#include "game/gamemap.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

GameAnimationPower* GameAnimationPower::m_pGameAnimationPower{nullptr};

spGameAnimationPower GameAnimationPower::createGameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO, GameMap * pMap)
{
    spGameAnimationPower animation;
    if (m_pGameAnimationPower == nullptr)
    {
        animation = MemoryManagement::create<GameAnimationPower>(frameTime, pCO, pMap);

        Interpreter* pInterpreter = Interpreter::getInstance();

        QJSValueList args({pInterpreter->newQObject(animation.get()),
                           color.name(),
                           powerMode,
                           pInterpreter->newQObject(pCO),
                           pInterpreter->newQObject(pMap)});
        pInterpreter->doFunction("GAMEANIMATIONPOWER", "loadAnimation", args);
        m_pGameAnimationPower = animation.get();
    }
    else
    {
        animation = m_pGameAnimationPower->getSharedPtr<GameAnimationPower>();
    }
    return animation;
}

GameAnimationPower::GameAnimationPower(quint32 frameTime, CO* pCO, GameMap * pMap)
    : GameAnimation(frameTime, pMap),
      m_endTimer(this),
      m_pCO(pCO)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameAnimationPower");
#endif
    Interpreter::setCppOwnerShip(this);
    m_endTimer.setSingleShot(true);
    connect(&m_endTimer, &QTimer::timeout, this, [this]()
    {
        emitFinished();
    });
    addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClick();
            }
        }
    });
    connect(this, &GameAnimationPower::sigRightClick, this, &GameAnimationPower::rightClick, Qt::QueuedConnection);
    m_finishQueued = true;
}

GameAnimationPower::~GameAnimationPower()
{
    if (!m_started)
    {
        AudioManager* pAudioThread = Mainapp::getInstance()->getAudioManager();
        pAudioThread->clearPlayList();
        m_pCO->loadCOMusic();
        pAudioThread->playRandom();
    }
    m_pGameAnimationPower = nullptr;
}

void GameAnimationPower::createMovingText(const QString font, const QString text, qint32 delay, QPoint startPos, QPoint endPos, qint32 duration, QEasingCurve::Type easeType)
{
    oxygine::TextStyle headline = oxygine::TextStyle(FontManager::getInstance()->getFont(font));
    headline.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headline.multiline = false;
    oxygine::spTextField textField = MemoryManagement::create<oxygine::TextField>();
    textField->setVisible(false);
    textField->setPosition(startPos.x(), startPos.y());
    textField->setStyle(headline);
    textField->setHtmlText(text);
    m_lastCreatedTweenQueue = MemoryManagement::create<oxygine::TweenQueue>();
    oxygine::spTween tween3 = oxygine::createTween(TweenToggleVisibility(0, 1.0f), oxygine::timeMS(1), 1, false, oxygine::timeMS(static_cast<qint32>(static_cast<float>(delay) / Settings::getInstance()->getAnimationSpeed())));
    oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenPosition(endPos), oxygine::timeMS(static_cast<qint32>(static_cast<float>(duration) / Settings::getInstance()->getAnimationSpeed())), 1, false, oxygine::timeMS(0), easeType);
    m_lastCreatedTweenQueue->add(tween3);
    m_lastCreatedTweenQueue->add(tween4);
    textField->addTween(m_lastCreatedTweenQueue);
    addChild(textField);
}

void GameAnimationPower::addMovingCoSprite(const QString sprite, float scale, QPoint startPos, QPoint endPos, qint32 duration, qint32 delay, QEasingCurve::Type easeType)
{
    oxygine::ResAnim* pAnim = m_pCO->getResAnim(sprite);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pAnim);
        pSprite->setScale(scale);
        pSprite->setSize(pAnim->getWidth(), pAnim->getHeight());
        pSprite->setPosition(startPos.x(), startPos.y());
        m_lastCreatedTweenQueue = MemoryManagement::create<oxygine::TweenQueue>();
        oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenPosition(endPos), oxygine::timeMS(static_cast<qint32>(static_cast<float>(duration) / Settings::getInstance()->getAnimationSpeed())), 1, false,
                                                       oxygine::timeMS(static_cast<qint32>(static_cast<float>(delay) / Settings::getInstance()->getAnimationSpeed())), easeType);
        m_lastCreatedTweenQueue->add(tween1);
        pSprite->addTween(m_lastCreatedTweenQueue);
        addChild(pSprite);
    }
}

QPoint GameAnimationPower::getCoSpriteSize(const QString sprite) const
{
    oxygine::ResAnim* pAnim = m_pCO->getResAnim(sprite);
    if (pAnim != nullptr)
    {
        return QPoint(pAnim->getWidth(), pAnim->getHeight());
    }
    else
    {
        return QPoint(1, 1);
    }
}

void GameAnimationPower::setDuration(qint32 timeMs)
{
    m_endTimer.setInterval(static_cast<qint32>(static_cast<float>(timeMs) / Settings::getInstance()->getAnimationSpeed()));
}

void GameAnimationPower::createRotatingBackground(const QString resAnim, const QColor color, qint32 speedX)
{
    oxygine::ResAnim* pAnimMask = GameManager::getInstance()->getResAnim(resAnim);    
    setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    if (pAnimMask != nullptr)
    {
        // first sprite for rotating
        oxygine::spBox9Sprite firstSpriteMask = MemoryManagement::create<oxygine::Box9Sprite>();
        firstSpriteMask->setResAnim(pAnimMask);
        firstSpriteMask->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
        firstSpriteMask->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
        firstSpriteMask->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
        firstSpriteMask->setColor(color);

        // second sprite for rotating
        oxygine::spBox9Sprite secondSpriteMask = MemoryManagement::create<oxygine::Box9Sprite>();
        secondSpriteMask->setResAnim(pAnimMask);
        secondSpriteMask->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
        secondSpriteMask->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
        secondSpriteMask->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
        secondSpriteMask->setColor(color);

        // rotating sprite
        spRotatingSprite rotSprite = MemoryManagement::create<RotatingSprite>();
        rotSprite->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
        rotSprite->setSprite(firstSpriteMask, secondSpriteMask);
        rotSprite->setDirection(speedX);
        addChild(rotSprite);
    }
}

void GameAnimationPower::createPowerDescription(CO* pCo, GameEnums::PowerMode powerMode, bool onTop)
{
    if (!Settings::getInstance()->getSmallScreenDevice())
    {
        QString description;
        switch (powerMode)
        {
            case GameEnums::PowerMode_Tagpower:
            case GameEnums::PowerMode_Superpower:
            {
                description = pCo->getSuperPowerDescription();
                break;
            }
            case GameEnums::PowerMode_Power:
            {
                description = pCo->getPowerDescription();
                break;
            }
            default:
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "illegal power mode GameAnimationPower::createPowerDescription");
                break;
            }
        }
        oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont32());
        style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
        style.multiline = true;
        oxygine::spTextField descriptionField = MemoryManagement::create<oxygine::TextField>();
        descriptionField->setStyle(style);
        descriptionField->setHtmlText(description);
        descriptionField->setSize(oxygine::Stage::getStage()->getWidth() - 100, descriptionField->getTextRect().height());
        descriptionField->setX(50);
        if (onTop)
        {
            descriptionField->setY(20);
        }
        else
        {
            descriptionField->setY(oxygine::Stage::getStage()->getHeight() - descriptionField->getTextRect().height() - 20);
        }
        addChild(descriptionField);
    }
}

void GameAnimationPower::rightClick()
{
    stop();
    onFinished(false);
}

void GameAnimationPower::stop()
{
    m_endTimer.stop();
}

void GameAnimationPower::start()
{
    if (!m_started)
    {
        AudioManager* pAudioThread = Mainapp::getInstance()->getAudioManager();
        pAudioThread->clearPlayList();
        m_pCO->loadCOMusic();
        pAudioThread->playRandom();
        m_endTimer.start();
    }
    GameAnimation::start();
}

void GameAnimationPower::restart()
{
    if (m_pMap != nullptr)
    {
        auto* pMenu = m_pMap->getMenu();
        if (pMenu != nullptr)
        {
            pMenu->addChild(getSharedPtr<oxygine::Actor>());
            m_endTimer.start();
        }
    }
}
