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
    connect(this, SIGNAL(sigFinished()), this, SLOT(onFinished()), Qt::QueuedConnection);
}

void GameAnimation::queueAnimation(GameAnimation* pGameAnimation)
{
    pGameAnimation->stopSound();
    m_QueuedAnimations.append(pGameAnimation);
    GameAnimationFactory::getInstance()->queueAnimation(pGameAnimation);
}

void GameAnimation::addSprite(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish, float scale)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();

        oxygine::spTweenQueue queuedAnim = new oxygine::TweenQueue();
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * m_frameTime, 1);
        queuedAnim->add(tween);
        if (sleepAfterFinish > 0)
        {
            oxygine::spTween tween1 = oxygine::createTween(TweenWait(), sleepAfterFinish, 1);
            queuedAnim->add(tween1);
        }
        pSprite->setScale(scale);
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
    if (animationSound != nullptr)
    {
        animationSound->stop();
        delete animationSound;
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
    animationSound = new QSound(soundFile);
    animationSound->setLoops(loops);
    animationSound->play();
}

void GameAnimation::startSound()
{
    if (animationSound != nullptr)
    {
        animationSound->play();
    }
}

void GameAnimation::stopSound()
{
    if (animationSound != nullptr)
    {
        animationSound->stop();
    }
}

void GameAnimation::setEndOfAnimationCall(QString postActionObject, QString postActionFunction)
{
    jsPostActionObject = postActionObject;
    jsPostActionFunction = postActionFunction;
}
