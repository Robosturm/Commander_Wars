#include "gameanimationpower.h"

#include "objects/rotatingsprite.h"

#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/tweentogglevisibility.h"
#include "coreengine/tweenwait.h"

#include "game/gameanimationfactory.h"

#include "game/player.h"
#include "game/co.h"

GameAnimationPower::GameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, QString coid)
    : GameAnimation (frameTime)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    oxygine::ResAnim* pAnimMask = GameManager::getInstance()->getResAnim("power_background");
    if (powerMode == GameEnums::PowerMode_Superpower ||
        powerMode == GameEnums::PowerMode_Tagpower)
    {
        pAnimMask = GameManager::getInstance()->getResAnim("superpower_background");
    }
    // first sprite for rotating
    oxygine::spBox9Sprite firstSpriteMask = new oxygine::Box9Sprite();
    firstSpriteMask->setResAnim(pAnimMask);
    firstSpriteMask->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    firstSpriteMask->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    firstSpriteMask->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);

    oxygine::Sprite::TweenColor tweenColor(color);
    oxygine::spTween tween = oxygine::createTween(tweenColor, oxygine::timeMS(1));
    firstSpriteMask->addTween(tween);

    // second sprite for rotating
    oxygine::spBox9Sprite secondSpriteMask = new oxygine::Box9Sprite();
    secondSpriteMask->setResAnim(pAnimMask);
    secondSpriteMask->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    secondSpriteMask->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    secondSpriteMask->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    oxygine::spTween tween2 = oxygine::createTween(tweenColor, oxygine::timeMS(1));
    secondSpriteMask->addTween(tween2);

    // rotating sprite
    spRotatingSprite rotSprite = new RotatingSprite();
    rotSprite->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    rotSprite->setSprite(firstSpriteMask, secondSpriteMask);
    rotSprite->setDirection(3);
    addChild(rotSprite);

    QString resAnim = coid.toLower() + "+nrm";
    oxygine::ResAnim* pAnim = COSpriteManager::getInstance()->getResAnim(resAnim);
    oxygine::spSprite m_CO = new oxygine::Sprite();
    m_CO->setResAnim(pAnim);
    m_CO->setScale((pApp->getSettings()->getHeight() - 400) / pAnim->getHeight());
    m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
    m_CO->setPosition(pApp->getSettings()->getWidth() - m_CO->getScaledWidth() - 20, - m_CO->getScaledHeight());
    oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenY(pApp->getSettings()->getHeight() / 2 -  m_CO->getScaledHeight() / 2), oxygine::timeMS(m_frameTime * 30));
    m_CO->addTween(tween1);
    addChild(m_CO);



    // cool text incoming
    QString text;
    if (powerMode == GameEnums::PowerMode_Tagpower)
    {
        GameMap* pMap = GameMap::getInstance();
        QJSValueList args;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(pMap->getCurrentPlayer()->getCO(0));
        args << obj1;
        CO* pCO1 = pMap->getCurrentPlayer()->getCO(1);
        QJSValue obj2 = pApp->getInterpreter()->newQObject(pCO1);
        args << obj2;
        QJSValue ret = pApp->getInterpreter()->doFunction("TAGPOWER", "getTagname", args);
        if (ret.isString())
        {
            text = ret.toString();
        }
        ret = pApp->getInterpreter()->doFunction("TAGPOWER", "getTagpower", args);
        if (ret.isNumber())
        {
            qint32 value = 100 + ret.toInt();
            text += "   Synergy: " + QString::number(value) + " %";
        }
        if (pCO1 != nullptr)
        {
            resAnim = pCO1->getCoID().toLower() + "+nrm";
            pAnim = COSpriteManager::getInstance()->getResAnim(resAnim);
            m_CO = new oxygine::Sprite();
            m_CO->setResAnim(pAnim);
            m_CO->setScale((pApp->getSettings()->getHeight() - 400) / pAnim->getHeight());
            m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
            m_CO->setPosition(pApp->getSettings()->getWidth() - m_CO->getScaledWidth() * 2 - 40, pApp->getSettings()->getHeight());
            tween1 = oxygine::createTween(oxygine::Actor::TweenY(pApp->getSettings()->getHeight() / 2 -  m_CO->getScaledHeight() / 2), oxygine::timeMS(m_frameTime * 30));
            m_CO->addTween(tween1);
            addChild(m_CO);
        }
    }
    else if (powerMode == GameEnums::PowerMode_Superpower)
    {
        QJSValue ret = pApp->getInterpreter()->doFunction(coid, "getSuperPowerName");
        if (ret.isString())
        {
            text = ret.toString();
        }
    }
    else
    {
        QJSValue ret = pApp->getInterpreter()->doFunction(coid, "getPowerName");
        if (ret.isString())
        {
            text = ret.toString();
        }
    }
    // text incoming
    oxygine::ResFont* font = FontManager::getMainFont();
    oxygine::TextStyle style = font;
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    float heigth = 20.0f;
    float scale = 3;
    qint32 xPos = 10;
    for (qint32 i = 0; i < text.size(); i++)
    {
        oxygine::spTextField textField = new oxygine::TextField();
        textField->setHtmlText(QString(text[i]));
        textField->setPosition(xPos, 0);
        textField->setScale(scale);
        textField->setStyle(style);
        qint32 size = textField->getTextRect().getWidth() * scale;
        if (size <= 0)
        {
            size = (xPos - 10) / (i + 1);
        }
        xPos += size;
        textField->setVisible(false);
        if (Mainapp::isEven(i))
        {
            textField->setY(pApp->getSettings()->getHeight());
        }
        oxygine::spTweenQueue queue = new oxygine::TweenQueue();
        oxygine::spTween tween2 = oxygine::createTween(TweenWait(), oxygine::timeMS(m_frameTime * 2 * i + 1));
        oxygine::spTween tween3 = oxygine::createTween(TweenToggleVisibility(0, 1.0f), oxygine::timeMS(1));
        oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(pApp->getSettings()->getHeight() / 2 - heigth / 2 * scale), oxygine::timeMS(m_frameTime * 4));
        queue->add(tween2);
        queue->add(tween3);
        queue->add(tween4);
        textField->addTween(queue);
        addChild(textField);
    }
    endTimer.setSingleShot(true);
    qint32 time = text.size() * static_cast<qint32>(m_frameTime) * 4;
    if (time < static_cast<qint32>(m_frameTime) * 30)
    {
        time = static_cast<qint32>(m_frameTime) * 30;
    }
    endTimer.setInterval(time);
    connect(&endTimer, &QTimer::timeout, this, &GameAnimationPower::onFinished, Qt::QueuedConnection);
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
}

void GameAnimationPower::rightClick()
{
    GameAnimationFactory::finishAllAnimations();
}

void GameAnimationPower::stop()
{
    endTimer.stop();
}

void GameAnimationPower::restart()
{
    GameMenue::getInstance()->addChild(this);
    endTimer.start();
}
