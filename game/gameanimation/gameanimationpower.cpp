#include "3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.h"

#include "objects/rotatingsprite.h"

#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"

#include "menue/basegamemenu.h"
#include "menue/movementplanner.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/audiothread.h"

#include "game/gameanimation/gameanimationfactory.h"
#include "game/gameanimation/gameanimationpower.h"
#include "game/player.h"
#include "game/co.h"
#include "game/gamemap.h"

GameAnimationPower* GameAnimationPower::m_pGameAnimationPower = nullptr;

spGameAnimationPower GameAnimationPower::createGameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO, GameMap * pMap)
{
    spGameAnimationPower animation;
    if (m_pGameAnimationPower == nullptr)
    {
        animation = spGameAnimationPower::create(frameTime, pCO, pMap);

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
        animation = m_pGameAnimationPower;
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
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
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
        AudioThread* pAudioThread = Mainapp::getInstance()->getAudioThread();
        pAudioThread->clearPlayList();
        m_pCO->loadCOMusic();
        pAudioThread->playRandom();
    }
    m_pGameAnimationPower = nullptr;
}

void GameAnimationPower::createMovingText(const QString & font, const QString & text, qint32 delay, QPoint startPos, QPoint endPos, qint32 duration, QEasingCurve::Type easeType)
{
    oxygine::TextStyle headline = oxygine::TextStyle(FontManager::getInstance()->getResFont(font));
    headline.color = FontManager::getFontColor();
    headline.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headline.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headline.multiline = false;
    oxygine::spTextField textField = oxygine::spTextField::create();
    textField->setVisible(false);
    textField->setPosition(startPos.x(), startPos.y());
    textField->setStyle(headline);
    textField->setHtmlText(text);
    m_lastCreatedTweenQueue = oxygine::spTweenQueue::create();
    oxygine::spTween tween3 = oxygine::createTween(TweenToggleVisibility(0, 1.0f), oxygine::timeMS(1), 1, false, oxygine::timeMS(delay));
    oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(endPos.x(), endPos.y())), oxygine::timeMS(duration), 1, false, oxygine::timeMS(0), easeType);
    m_lastCreatedTweenQueue->add(tween3);
    m_lastCreatedTweenQueue->add(tween4);
    textField->addTween(m_lastCreatedTweenQueue);
    addChild(textField);
}

void GameAnimationPower::addMovingCoSprite(const QString & sprite, float scale, QPoint startPos, QPoint endPos, qint32 duration, qint32 delay, QEasingCurve::Type easeType)
{
    oxygine::ResAnim* pAnim = m_pCO->getResAnim(sprite);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        pSprite->setResAnim(pAnim);
        pSprite->setScale(scale);
        pSprite->setSize(pAnim->getWidth(), pAnim->getHeight());
        pSprite->setPosition(startPos.x(), startPos.y());
        m_lastCreatedTweenQueue = oxygine::spTweenQueue::create();
        oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(endPos.x(), endPos.y())), oxygine::timeMS(duration), 1, false, oxygine::timeMS(delay), easeType);
        m_lastCreatedTweenQueue->add(tween1);
        pSprite->addTween(m_lastCreatedTweenQueue);
        addChild(pSprite);
    }

}

QPoint GameAnimationPower::getCoSpriteSize(const QString & sprite) const
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
    m_endTimer.setInterval(timeMs);
}

void GameAnimationPower::createRotatingBackground(const QString & resAnim, const QColor & color)
{
    oxygine::ResAnim* pAnimMask = GameManager::getInstance()->getResAnim(resAnim);    
    setSize(Settings::getWidth(), Settings::getHeight());
    if (pAnimMask != nullptr)
    {
        // first sprite for rotating
        oxygine::spBox9Sprite firstSpriteMask = oxygine::spBox9Sprite::create();
        firstSpriteMask->setResAnim(pAnimMask);
        firstSpriteMask->setSize(Settings::getWidth(), Settings::getHeight());
        firstSpriteMask->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
        firstSpriteMask->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
        firstSpriteMask->setColor(color);

        // second sprite for rotating
        oxygine::spBox9Sprite secondSpriteMask = oxygine::spBox9Sprite::create();
        secondSpriteMask->setResAnim(pAnimMask);
        secondSpriteMask->setSize(Settings::getWidth(), Settings::getHeight());
        secondSpriteMask->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
        secondSpriteMask->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
        secondSpriteMask->setColor(color);

        // rotating sprite
        spRotatingSprite rotSprite = spRotatingSprite::create();
        rotSprite->setSize(Settings::getWidth(), Settings::getHeight());
        rotSprite->setSprite(firstSpriteMask, secondSpriteMask);
        rotSprite->setDirection(3);
        addChild(rotSprite);
    }
}

void GameAnimationPower::createPowerDescription(CO* pCo, GameEnums::PowerMode powerMode, bool onTop)
{
    if (!Settings::getSmallScreenDevice())
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
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
        style.multiline = true;
        oxygine::spTextField descriptionField = oxygine::spTextField::create();
        descriptionField->setStyle(style);
        descriptionField->setHtmlText(description);
        descriptionField->setSize(Settings::getWidth() - 100, descriptionField->getTextRect().getHeight());
        descriptionField->setX(50);
        if (onTop)
        {
            descriptionField->setY(20);
        }
        else
        {
            descriptionField->setY(Settings::getHeight() - descriptionField->getTextRect().getHeight() - 20);
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
        AudioThread* pAudioThread = Mainapp::getInstance()->getAudioThread();
        pAudioThread->clearPlayList();
        m_pCO->loadCOMusic();
        pAudioThread->playRandom();
        m_endTimer.start();
    }
    GameAnimation::start();
}

void GameAnimationPower::restart()
{
    BaseGamemenu* pMenu = BaseGamemenu::getInstance();
    if (pMenu != nullptr)
    {
        pMenu->addChild(spGameAnimationPower(this));
        m_endTimer.start();
    }
}
