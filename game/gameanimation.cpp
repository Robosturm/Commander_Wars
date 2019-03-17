#include "gameanimation.h"

#include "game/gameanimationfactory.h"

#include "resource_management/gameanimationmanager.h"

#include "coreengine/console.h"

#include "coreengine/mainapp.h"

#include "coreengine/interpreter.h"

#include "coreengine/tweenwait.h"

GameAnimation::GameAnimation(quint32 frameTime)
    : QObject(),
      m_frameTime(frameTime)
{
    Interpreter::setCppOwnerShip(this);
    connect(this, SIGNAL(sigFinished()), this, SLOT(onFinished()), Qt::QueuedConnection);
}

void GameAnimation::queueAnimation(GameAnimation* pGameAnimation)
{
    m_QueuedAnimations.append(pGameAnimation);
    GameAnimationFactory::getInstance()->queueAnimation(pGameAnimation);
}

void GameAnimation::update(const oxygine::UpdateState& us)
{
    if (!m_SoundStarted)
    {
        if (!m_soundFile.isEmpty())
        {
            Mainapp::getInstance()->getAudioThread()->playSound(m_soundFile, m_loops);
        }
        m_SoundStarted = true;
    }
    oxygine::Sprite::update(us);
}

void GameAnimation::addSprite(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish, float scale, qint32 delay)
{
    addSprite2(spriteID, offsetX, offsetY, sleepAfterFinish, scale, scale, delay);
}

void GameAnimation::addSprite2(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();

        oxygine::spTweenQueue queuedAnim = new oxygine::TweenQueue();
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * m_frameTime, 1, false, delay);
        queuedAnim->add(tween);
        if (sleepAfterFinish > 0)
        {
            oxygine::spTween tween1 = oxygine::createTween(TweenWait(), sleepAfterFinish, 1);
            queuedAnim->add(tween1);
        }
        pSprite->setScaleX(scaleX);
        pSprite->setScaleY(scaleY);
        pSprite->addTween(queuedAnim);
        this->addChild(pSprite);
        pSprite->setPosition(offsetX, offsetY);

        queuedAnim->setDoneCallback([=](oxygine::Event *)->void
        {
            emit sigFinished();
        });
    }
    else
    {
        Console::print("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}

void GameAnimation::onFinished()
{
    if (m_loops < 0)
    {
       Mainapp::getInstance()->getAudioThread()->stopSound(m_soundFile);
    }
    for (qint32 i = 0; i < m_QueuedAnimations.size(); i++)
    {
        GameAnimationFactory::getInstance()->startQueuedAnimation(m_QueuedAnimations[i]);
    }
    if ((!jsPostActionObject.isEmpty()) && (!jsPostActionObject.isEmpty()))
    {
        Mainapp::getInstance()->getInterpreter()->doFunction(jsPostActionObject, jsPostActionFunction);
    }
    GameAnimationFactory::removeAnimation(this);
}

void GameAnimation::setSound(QString soundFile, qint32 loops)
{
    m_soundFile = soundFile;
    m_loops = loops;
}

void GameAnimation::addTweenScale(float endScale, qint32 duration)
{
    oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenScale(endScale), duration);
    this->addTween(tween1);
}

void GameAnimation::addTweenPosition(QPoint point, qint32 duration)
{
    oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(point.x(), point.y())), duration);
    this->addTween(tween1);
}

void GameAnimation::addTweenColor(qint32 spriteIdx, QColor startColor, QColor endColor, qint32 duration, bool twoSided, qint32 delay)
{
    oxygine::spActor actor = getFirstChild().get();
    for (qint32 i = 0; i <spriteIdx; i++)
    {
        actor = getNextSibling();
    }
    oxygine::Sprite* sprite = dynamic_cast<oxygine::Sprite*>(actor.get());
    if (sprite != nullptr)
    {
        sprite->setColor(oxygine::Color(startColor.red(), startColor.green(), startColor.blue(), startColor.alpha()));
        oxygine::Sprite::TweenColor tweenColor(oxygine::Color(endColor.red(), endColor.green(), endColor.blue(), endColor.alpha()));
        oxygine::spTween tween = oxygine::createTween(tweenColor, duration, 1, twoSided, delay);
        sprite->addTween(tween);
    }
}

void GameAnimation::setEndOfAnimationCall(QString postActionObject, QString postActionFunction)
{
    jsPostActionObject = postActionObject;
    jsPostActionFunction = postActionFunction;
}
