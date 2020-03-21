#include "gameanimation.h"

#include "game/gameanimationfactory.h"

#include "resource_management/gameanimationmanager.h"

#include "resource_management/fontmanager.h"

#include "coreengine/console.h"

#include "coreengine/mainapp.h"

#include "coreengine/interpreter.h"

#include "coreengine/tweenwait.h"

#include "coreengine/settings.h"

#include "coreengine/audiothread.h"

GameAnimation::GameAnimation(quint32 frameTime)
    : QObject(),
      m_frameTime(frameTime / Settings::getAnimationSpeed())
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    if (m_frameTime <= 0)
    {
        m_frameTime = 1;
    }
    connect(this, &GameAnimation::sigFinished, this, &GameAnimation::onFinished, Qt::QueuedConnection);
    buffer.open(QIODevice::ReadWrite);
}

void GameAnimation::restart()
{
    m_stopped = false;
    m_previousAnimation = nullptr;
    GameMap::getInstance()->addChild(this);
}

void GameAnimation::stop()
{
    m_stopped = true;
}

void GameAnimation::setRotation(float angle)
{
    setRotationDegrees(angle);
}

void GameAnimation::queueAnimation(GameAnimation* pGameAnimation)
{
    pGameAnimation->setPreviousAnimation(this);
    m_QueuedAnimations.append(pGameAnimation);
    GameAnimationFactory::getInstance()->queueAnimation(pGameAnimation);
}

void GameAnimation::queueAnimationBefore(GameAnimation* pGameAnimation)
{
    if (m_previousAnimation != nullptr)
    {
        // remove ourself from previous animation and add our new ancestor
        m_previousAnimation->removeQueuedAnimation(this);
        m_previousAnimation->queueAnimation(pGameAnimation);
    }
    // queue ourself after the given animation
    pGameAnimation->queueAnimation(this);
}

void GameAnimation::removeQueuedAnimation(GameAnimation* pGameAnimation)
{
    qint32 i = 0;
    while (i < m_QueuedAnimations.size())
    {
        if (m_QueuedAnimations[i] == pGameAnimation)
        {
            m_QueuedAnimations.removeAt(i);
        }
        else
        {
            i++;
        }
    }
}

void GameAnimation::update(const oxygine::UpdateState& us)
{
    for (SpriteData& data : sprites)
    {
        if (!data.loaded)
        {
            data.pAnim = new oxygine::SingleResAnim();
            data.pAnim->setResPath(data.file);
            data.pAnim->init(data.file, data.frames, 1, 1.0f);
            loadSpriteAnim(data.pAnim.get(), data.offsetX, data.offsetY,
                           data.color, data.sleepAfterFinish, data.scaleX, data.scaleY,
                           data.delay);
            data.loaded = true;
        }
    }

    if (!m_SoundStarted)
    {
        if (!m_soundFile.isEmpty())
        {
            Mainapp::getInstance()->getAudioThread()->playSound(m_soundFile, m_loops, m_soundFolder);
        }
        m_SoundStarted = true;
    }
    oxygine::Sprite::update(us);
}

void GameAnimation::setPreviousAnimation(GameAnimation *previousAnimation)
{
    m_previousAnimation = previousAnimation;
}

void GameAnimation::addSprite(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish, float scale, qint32 delay)
{
    addSprite2(spriteID, offsetX, offsetY, sleepAfterFinish, scale, scale, delay);
}

void GameAnimation::addSprite2(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay)
{
    addSprite3(spriteID, offsetX, offsetY, QColor(255, 255, 255), sleepAfterFinish, scaleX, scaleY, delay);
}

void GameAnimation::addSprite3(QString spriteID, float offsetX, float offsetY, QColor color, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay, qint32 frames)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID, oxygine::error_policy::ep_ignore_error);
    if (pAnim != nullptr)
    {
        loadSpriteAnim(pAnim, offsetX, offsetY, color, sleepAfterFinish, scaleX, scaleY, delay);
    }
    else if (QFile::exists(spriteID))
    {
        sprites.append(SpriteData());
        qint32 index = sprites.size() - 1;
         sprites[index].frames = frames;
         sprites[index].file = spriteID;
         sprites[index].offsetX = offsetX;
         sprites[index].offsetY = offsetY;
         sprites[index].color = color;
         sprites[index].sleepAfterFinish = sleepAfterFinish;
         sprites[index].scaleX = scaleX;
         sprites[index].scaleY = scaleY;
         sprites[index].delay = delay;
    }
    else
    {
        Console::print("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}

void GameAnimation::loadSpriteAnim(oxygine::ResAnim* pAnim, float offsetX, float offsetY, QColor color, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay)
{
    oxygine::spSprite pSprite = new oxygine::Sprite();

    oxygine::spTweenQueue queuedAnim = new oxygine::TweenQueue();
    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * m_frameTime), 1, false, oxygine::timeMS(static_cast<qint64>(delay / Settings::getAnimationSpeed())));
    queuedAnim->add(tween);
    if (sleepAfterFinish > 0)
    {
        oxygine::spTween tween1 = oxygine::createTween(TweenWait(), oxygine::timeMS(static_cast<qint64>(sleepAfterFinish / Settings::getAnimationSpeed())), 1);
        queuedAnim->add(tween1);
    }
    pSprite->setScaleX(scaleX);
    pSprite->setScaleY(scaleY);
    pSprite->addTween(queuedAnim);
    if (color != Qt::white)
    {
        oxygine::Sprite::TweenColor tweenColor(color);
        oxygine::spTween tween = oxygine::createTween(tweenColor, oxygine::timeMS(1));
        pSprite->addTween(tween);
    }
    this->addChild(pSprite);
    pSprite->setPosition(offsetX, offsetY);
    if(!finishQueued)
    {
        finishQueued = true;
        queuedAnim->setDoneCallback([=](oxygine::Event *)->void
        {
            emit sigFinished();
        });

    }
}

void GameAnimation::addText(QString text, float offsetX, float offsetY, float scale, QColor color)
{
    oxygine::TextStyle style = FontManager::getMainFont16();
    style.color = color;
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(text);
    pTextfield->setPosition(offsetX, offsetY);
    pTextfield->setScale(scale);
    addChild(pTextfield);
}

bool GameAnimation::onFinished()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    pApp->getAudioThread()->stopSound(m_soundFile, m_soundFolder);
    for (qint32 i = 0; i < m_QueuedAnimations.size(); i++)
    {
        GameAnimationFactory::getInstance()->startQueuedAnimation(m_QueuedAnimations[i]);
    }
    if ((!jsPostActionObject.isEmpty()) && (!jsPostActionObject.isEmpty()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        pInterpreter->doFunction(jsPostActionObject, jsPostActionFunction, args1);
    }
    GameAnimationFactory::removeAnimation(this);
    pApp->continueThread();
    return true;
}

void GameAnimation::setSound(QString soundFile, qint32 loops, QString folder)
{
    m_soundFile = soundFile;
    m_loops = loops;
    m_soundFolder = folder;
}

void GameAnimation::addTweenScale(float endScale, qint32 duration)
{
    oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenScale(endScale), oxygine::timeMS(static_cast<qint64>(duration / Settings::getAnimationSpeed())));
    this->addTween(tween1);
}

void GameAnimation::addTweenPosition(QPoint point, qint32 duration)
{
    oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenPosition(oxygine::Vector2(point.x(), point.y())), oxygine::timeMS(static_cast<qint64>(duration/ Settings::getAnimationSpeed())));
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
        sprite->setColor(startColor);
        oxygine::Sprite::TweenColor tweenColor(endColor);
        oxygine::spTween tween = oxygine::createTween(tweenColor, oxygine::timeMS(static_cast<qint64>(duration / Settings::getAnimationSpeed())), 1, twoSided, oxygine::timeMS(static_cast<qint64>(delay / Settings::getAnimationSpeed())));
        sprite->addTween(tween);
    }
}

void GameAnimation::addTweenWait(qint32 duration)
{
    oxygine::spTween tween1 = oxygine::createTween(TweenWait(), oxygine::timeMS(static_cast<qint64>(duration / Settings::getAnimationSpeed())), 1);
    addTween(tween1);
    if(!finishQueued)
    {
        finishQueued = true;
        tween1->setDoneCallback([=](oxygine::Event *)->void
        {
            emit sigFinished();
        });
    }
}

void GameAnimation::setEndOfAnimationCall(QString postActionObject, QString postActionFunction)
{
    jsPostActionObject = postActionObject;
    jsPostActionFunction = postActionFunction;
}
