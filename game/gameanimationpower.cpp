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

GameAnimationPower::GameAnimationPower(quint32 frameTime, QColor color, bool superpower, QString coid)
    : GameAnimation (frameTime)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    oxygine::ResAnim* pAnimMask = GameManager::getInstance()->getResAnim("power_background");
    if (superpower)
    {
        pAnimMask = GameManager::getInstance()->getResAnim("superpower_background");
    }
    // first sprite for rotating
    oxygine::spBox9Sprite firstSpriteMask = new oxygine::Box9Sprite();
    firstSpriteMask->setResAnim(pAnimMask);
    firstSpriteMask->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    firstSpriteMask->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    firstSpriteMask->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);

    firstSpriteMask->setColor(static_cast<quint8>(color.red()), static_cast<quint8>(color.green()), static_cast<quint8>(color.blue()), 255);
    // second sprite for rotating
    oxygine::spBox9Sprite secondSpriteMask = new oxygine::Box9Sprite();
    secondSpriteMask->setResAnim(pAnimMask);
    secondSpriteMask->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    secondSpriteMask->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    secondSpriteMask->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    secondSpriteMask->setColor(static_cast<quint8>(color.red()), static_cast<quint8>(color.green()), static_cast<quint8>(color.blue()), 255);
    // rotating sprite
    spRotatingSprite rotSprite = new RotatingSprite();
    rotSprite->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    rotSprite->setSprite(firstSpriteMask, secondSpriteMask);
    rotSprite->setDirection(3);
    addChild(rotSprite);

    QString resAnim = coid.toLower() + "+nrm";
    oxygine::ResAnim* pAnim = COSpriteManager::getInstance()->getResAnim(resAnim.toStdString().c_str());
    oxygine::spSprite m_CO = new oxygine::Sprite();
    m_CO->setResAnim(pAnim);
    m_CO->setScale((pApp->getSettings()->getHeight() - 300) / pAnim->getHeight());
    m_CO->setSize(pAnim->getWidth(), pAnim->getHeight());
    m_CO->setPosition(pApp->getSettings()->getWidth() - m_CO->getScaledWidth() - 20, -m_CO->getScaledHeight());
    oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenY(pApp->getSettings()->getHeight() / 2 -  m_CO->getScaledHeight() / 2), frameTime * 30);
    m_CO->addTween(tween1);
    addChild(m_CO);

    // cool text incoming
    QString text;
    if (superpower)
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
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    float heigth = 20.0f;
    float scale = 3;
    qint32 xPos = 10;
    for (qint32 i = 0; i < text.size(); i++)
    {
        oxygine::spTextField textField = new oxygine::TextField();
        textField->setText(QString(text[i]).toStdString().c_str());
        textField->setPosition(xPos, 0);
        textField->setScale(scale);
        textField->setStyle(style);
        xPos += textField->getTextRect().getWidth() * scale;
        textField->setVisible(false);
        if (Mainapp::isEven(i))
        {
            textField->setY(pApp->getSettings()->getHeight());
        }
        oxygine::spTweenQueue queue = new oxygine::TweenQueue();
        oxygine::spTween tween2 = oxygine::createTween(TweenWait(), frameTime * 2 * i + 1);
        oxygine::spTween tween3 = oxygine::createTween(TweenToggleVisibility(0, 1.0f), 1);
        oxygine::spTween tween4 = oxygine::createTween(oxygine::Actor::TweenY(pApp->getSettings()->getHeight() / 2 - heigth / 2 * scale), frameTime * 4);
        queue->add(tween2);
        queue->add(tween3);
        queue->add(tween4);
        textField->addTween(queue);
        addChild(textField);
    }
    endTimer.setSingleShot(true);
    qint32 time = text.size() * frameTime * 6 + frameTime * 2;
    if (time < frameTime * 32)
    {
        time = frameTime * 32;
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
