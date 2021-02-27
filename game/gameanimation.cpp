#include "game/gameanimation.h"
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
                           data.delay, 1);
            data.loaded = true;
        }
    }

    if (!m_SoundStarted)
    {
        if (!m_soundFile.isEmpty())
        {
            AudioThread* pAudioThread = Mainapp::getInstance()->getAudioThread();
            pAudioThread->playSound(m_soundFile, m_loops, m_soundFolder);
        }
        m_SoundStarted = true;
    }
    oxygine::Sprite::update(us);
}

bool GameAnimation::getVisible() const
{
    return oxygine::Sprite::getVisible();
}
void GameAnimation::setVisible(bool vis)
{
    oxygine::Sprite::setVisible(vis);
}

void GameAnimation::setPreviousAnimation(GameAnimation *previousAnimation)
{
    m_previousAnimation = previousAnimation;
}

void GameAnimation::addSprite(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish, float scale, qint32 delay, qint32 loops)
{
    addSprite2(spriteID, offsetX, offsetY, sleepAfterFinish, scale, scale, delay, loops);
}

void GameAnimation::addSprite2(QString spriteID, float offsetX, float offsetY, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay, qint32 loops)
{
    addSprite3(spriteID, offsetX, offsetY, QColor(255, 255, 255), sleepAfterFinish, scaleX, scaleY, delay, 0, loops);
}

void GameAnimation::addSpriteAnimTable(QString spriteID, float offsetX, float offsetY, Player* pPlayer, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay, qint32)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID, oxygine::error_policy::ep_ignore_error);
    if (pAnim != nullptr)
    {
        loadSpriteAnimTable(pAnim, offsetX, offsetY, pPlayer, sleepAfterFinish, scaleX, scaleY, delay);
    }
    else
    {
        Console::print("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}

void GameAnimation::addSprite3(QString spriteID, float offsetX, float offsetY, QColor color, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay, qint32 frames, qint32 loops)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID, oxygine::error_policy::ep_ignore_error);
    if (pAnim != nullptr)
    {
        loadSpriteAnim(pAnim, offsetX, offsetY, color, sleepAfterFinish, scaleX, scaleY, delay, loops);
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

void GameAnimation::addBox(QString spriteID, float offsetX, float offsetY, qint32 width, qint32 heigth,  qint32 sleepAfterFinish, QColor color)
{
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    oxygine::spTweenQueue queuedAnim = new oxygine::TweenQueue();
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID, oxygine::error_policy::ep_ignore_error);
    pBox->setResAnim(pAnim);
    pBox->setSize(width, heigth);
    if (sleepAfterFinish > 0)
    {
        oxygine::spTween tween1 = oxygine::createTween(TweenWait(), oxygine::timeMS(static_cast<qint64>(sleepAfterFinish / Settings::getAnimationSpeed())), 1);
        queuedAnim->add(tween1);
    }
    pBox->addTween(queuedAnim);
    if (color != Qt::white)
    {
        pBox->setColor(color);
    }
    this->addChild(pBox);
    pBox->setPosition(offsetX, offsetY);
    if(!finishQueued)
    {
        finishQueued = true;
        queuedAnim->setDoneCallback([=](oxygine::Event *)->void
        {
            emitFinished();
        });

    }
}

void GameAnimation::loadSpriteAnimTable(oxygine::ResAnim* pAnim, float offsetX, float offsetY, Player* pPlayer, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay)
{
    if (pAnim != nullptr)
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
        if (pPlayer != nullptr)
        {
            pSprite->setColorTable(pPlayer->getColorTableAnim());
        }
        else
        {
            pSprite->setColorTable(Player::getNeutralTableAnim());
        }
        this->addChild(pSprite);
        pSprite->setPosition(offsetX, offsetY);
        if(!finishQueued)
        {
            finishQueued = true;
            queuedAnim->setDoneCallback([=](oxygine::Event *)->void
            {
                emitFinished();
            });
        }
    }
}

void GameAnimation::loadSpriteAnim(oxygine::ResAnim* pAnim, float offsetX, float offsetY, QColor color, qint32 sleepAfterFinish, float scaleX, float scaleY, qint32 delay, qint32 loops)
{
    oxygine::spSprite pSprite = new oxygine::Sprite();
    oxygine::spTweenQueue queuedAnim = new oxygine::TweenQueue();
    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * m_frameTime), loops, false, oxygine::timeMS(static_cast<qint64>(delay / Settings::getAnimationSpeed())));
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
        pSprite->setColor(color);
    }
    this->addChild(pSprite);
    pSprite->setPosition(offsetX, offsetY);
    if(!finishQueued)
    {
        finishQueued = true;
        queuedAnim->setDoneCallback([=](oxygine::Event *)->void
        {
            emitFinished();
        });

    }
}

qint32 GameAnimation::addText(QString text, float offsetX, float offsetY, float scale, QColor color, qint32 priority)
{
    oxygine::TextStyle style = FontManager::getMainFont72();
    style.color = color;
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(text);
    pTextfield->setPosition(offsetX, offsetY);
    pTextfield->setScale(scale * 16.0f / 72.0f);
    pTextfield->setPriority(priority);
    addChild(pTextfield);
    return pTextfield->getTextRect().getWidth() * scale * 16.0f / 72.0f;
}

bool GameAnimation::onFinished(bool skipping)
{    
    m_skipping |= skipping;
    if (m_skipping == skipping)
    {
        Mainapp::getInstance()->getAudioThread()->stopSound(m_soundFile, m_soundFolder);
        for (qint32 i = 0; i < m_QueuedAnimations.size(); i++)
        {
            GameAnimationFactory::getInstance()->startQueuedAnimation(m_QueuedAnimations[i]);
        }
        if ((!jsPostActionObject.isEmpty()) && (!jsPostActionObject.isEmpty()))
        {
            Console::print("Calling post Animation function " + jsPostActionObject + "." + jsPostActionFunction, Console::eDEBUG);
            Interpreter* pInterpreter = Interpreter::getInstance();
            QJSValueList args1;
            QJSValue obj1 = pInterpreter->newQObject(this);
            args1 << obj1;
            pInterpreter->doFunction(jsPostActionObject, jsPostActionFunction, args1);
        }
    }
    GameAnimationFactory::removeAnimation(this, m_skipping);
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
            emitFinished();
        });
    }
}

void GameAnimation::setEndOfAnimationCall(QString postActionObject, QString postActionFunction)
{
    jsPostActionObject = postActionObject;
    jsPostActionFunction = postActionFunction;
}

void GameAnimation::emitFinished()
{
    if (!m_skipping)
    {
        emit sigFinished(m_skipping);
    }
}
