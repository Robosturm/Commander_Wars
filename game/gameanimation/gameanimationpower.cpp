#include "objects/rotatingsprite.h"

#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/tweens/tweentogglevisibility.h"
#include "coreengine/tweens/tweenwait.h"
#include "coreengine/audiothread.h"

#include "game/gameanimation/gameanimationfactory.h"
#include "game/gameanimation/gameanimationpower.h"
#include "game/player.h"
#include "game/co.h"

GameAnimationPower* GameAnimationPower::m_pGameAnimationPower = nullptr;

spGameAnimationPower GameAnimationPower::createGameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO)
{
    spGameAnimationPower animation;
    if (m_pGameAnimationPower == nullptr)
    {
        animation = spGameAnimationPower::create(frameTime, color, powerMode, pCO);
        m_pGameAnimationPower = animation.get();
    }
    else
    {
        animation = m_pGameAnimationPower;
    }
    return animation;
}

GameAnimationPower::GameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO)
    : GameAnimation (frameTime),
      m_pCO(pCO)
{
    setObjectName("GameAnimationPower");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    Interpreter* pInterpreter = Interpreter::getInstance();
    oxygine::ResAnim* pAnimMask = GameManager::getInstance()->getResAnim("power_background");
    if (powerMode == GameEnums::PowerMode_Superpower ||
        powerMode == GameEnums::PowerMode_Tagpower)
    {
        pAnimMask = GameManager::getInstance()->getResAnim("superpower_background");
    }
    setSize(Settings::getWidth(), Settings::getHeight());
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

    QString coid = m_pCO->getCoID();
    QString resAnim = coid.toLower() + "+nrm";
    oxygine::ResAnim* pAnim = m_pCO->getResAnim(resAnim);
    oxygine::spSprite m_CO = oxygine::spSprite::create();
    if (pAnim != nullptr)
    {
        m_CO->setResAnim(pAnim);
        m_CO->setScale((Settings::getHeight() - 400) / pAnim->getHeight());
        m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
        m_CO->setPosition(Settings::getWidth() - m_CO->getScaledWidth() - 20, - m_CO->getScaledHeight());
        oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenY(Settings::getHeight() / 2 -  m_CO->getScaledHeight() / 2), oxygine::timeMS(m_frameTime * 30));
        m_CO->addTween(tween1);
        addChild(m_CO);
    }
    // cool text incoming
    QString text;
    if (powerMode == GameEnums::PowerMode_Tagpower)
    {
        spGameMap pMap = GameMap::getInstance();
        QJSValueList args;
        QJSValue obj1 = pInterpreter->newQObject(pMap->getCurrentPlayer()->getCO(0));
        args << obj1;
        CO* pCO1 = pMap->getCurrentPlayer()->getCO(1);
        QJSValue obj2 = pInterpreter->newQObject(pCO1);
        args << obj2;
        QJSValue ret = pInterpreter->doFunction("TAGPOWER", "getTagname", args);
        if (ret.isString())
        {
            text = ret.toString();
        }
        ret = pInterpreter->doFunction("TAGPOWER", "getTagpower", args);
        if (ret.isNumber())
        {
            qint32 value = 100 + ret.toInt();
            text += " Synergy: " + QString::number(value) + " %";
        }
        if (pCO1 != nullptr)
        {
            resAnim = pCO1->getCoID().toLower() + "+nrm";
            pAnim = pCO1->getResAnim(resAnim);
            m_CO = oxygine::spSprite::create();
            if (pAnim != nullptr)
            {
                m_CO->setResAnim(pAnim);
                m_CO->setScale((Settings::getHeight() - 400) / pAnim->getHeight());
                m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
                m_CO->setPosition(Settings::getWidth() - m_CO->getScaledWidth() * 2 - 40, Settings::getHeight());
                oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenY(Settings::getHeight() / 2 -  m_CO->getScaledHeight() / 2), oxygine::timeMS(m_frameTime * 30));
                m_CO->addTween(tween1);
                addChild(m_CO);
            }
        }
    }
    else if (powerMode == GameEnums::PowerMode_Superpower)
    {
        QJSValue ret = pInterpreter->doFunction(coid, "getSuperPowerName");
        if (ret.isString())
        {
            text = ret.toString();
        }
    }
    else
    {
        QJSValue ret = pInterpreter->doFunction(coid, "getPowerName");
        if (ret.isString())
        {
            text = ret.toString();
        }
    }
    // text incoming
    oxygine::TextStyle headline = FontManager::getMainFont72();
    headline.color = FontManager::getFontColor();
    headline.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headline.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headline.multiline = false;
    oxygine::spTextField testField = oxygine::spTextField::create();
    testField->setHtmlText(text);
    qint32 textSize = testField->getTextRect().getWidth();
    if (textSize > Settings::getWidth() - 20)
    {
        // select smaller font size in case of small screen or large name
        headline = FontManager::getMainFont48();
        headline.color = FontManager::getFontColor();
        headline.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        headline.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        headline.multiline = false;
    }

    qint32 xPos = 10;
    for (qint32 i = 0; i < text.size(); i++)
    {
        oxygine::spTextField textField = oxygine::spTextField::create();
        textField->setHtmlText(QString(text[i]));
        textField->setPosition(xPos, 0);
        textField->setStyle(headline);
        qint32 size = textField->getTextRect().getWidth();
        float heigth = textField->getTextRect().getHeight();
        if (size <= 0)
        {
            size = (xPos - 10) / (i + 1);
        }
        xPos += size;
        textField->setVisible(false);
        if (GlobalUtils::isEven(i))
        {
            textField->setY(Settings::getHeight());
        }
        oxygine::spTweenQueue queue = oxygine::spTweenQueue::create();
        oxygine::spTween tween2 = oxygine::createTween(TweenWait(), oxygine::timeMS(m_frameTime * 2 * i + 1));
        oxygine::spTween tween3 = oxygine::createTween(TweenToggleVisibility(0, 1.0f), oxygine::timeMS(1));
        oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(Settings::getHeight() / 2 - heigth / 2), oxygine::timeMS(m_frameTime * 4));
        queue->add(tween2);
        queue->add(tween3);
        queue->add(tween4);
        textField->addTween(queue);
        addChild(textField);
    }
    m_endTimer.setSingleShot(true);
    qint32 time = text.size() * static_cast<qint32>(m_frameTime) * 4;
    if (time < static_cast<qint32>(m_frameTime) * 30)
    {
        time = static_cast<qint32>(m_frameTime) * 30;
    }
    m_endTimer.setInterval(time);
    connect(&m_endTimer, &QTimer::timeout, [=]()
    {
        emitFinished();
    });
    addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClick();
            }
        }
    });
    connect(this, &GameAnimationPower::sigRightClick, this, &GameAnimationPower::rightClick, Qt::QueuedConnection);
    addSound("power_activation.wav");
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
    }
    GameAnimation::start();
}

void GameAnimationPower::restart()
{
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->addChild(this);
        m_endTimer.start();
    }
}
