#include "coreengine/memorymanagement.h"

#include "3rd_party/oxygine-framework/oxygine/actor/VisualStyleActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#include "coreengine/mainapp.h"

MemoryManagement MemoryManagement::m_memoryManagement;
std::atomic<quint32> MemoryManagement::m_objectCounter{0};

MemoryManagement::MemoryManagement()
{
    Interpreter::setCppOwnerShip(this);
    connect(this, &MemoryManagement::sigSetAddColor, this, &MemoryManagement::setAddColor, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigRebuildText, this, &MemoryManagement::rebuildText, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigChangeAnimFrame, this, &MemoryManagement::changeAnimFrame, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigSetColorTable, this, &MemoryManagement::setColorTable, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigRestartAllTweens, this, &MemoryManagement::restartAllTweens, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigSyncAllTweens, this, &MemoryManagement::syncAllTweens, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigAddChild, this, &MemoryManagement::addChild, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigRemoveChild, this, &MemoryManagement::removeChild, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigSetPriority, this, &MemoryManagement::setPriority, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigAddTween, this, &MemoryManagement::addTween, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigRemoveTween, this, &MemoryManagement::removeTween, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigRemoveTweens, this, &MemoryManagement::removeTweens, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigRemoveChildren, this, &MemoryManagement::removeChildren, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigAddEventListener, this, &MemoryManagement::addEventListener, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigRemoveEventListener, this, &MemoryManagement::removeEventListener, Qt::BlockingQueuedConnection);
    connect(this, &MemoryManagement::sigRemoveEventListeners, this, &MemoryManagement::removeEventListeners, Qt::BlockingQueuedConnection);
}

void MemoryManagement::printDeletionInfo(QObject* obj)
{
    Q_ASSERT(QJSEngine::objectOwnership(obj) == QJSEngine::ObjectOwnership::CppOwnership);
    if (obj->objectName().isEmpty())
    {
        CONSOLE_PRINT("Deleting object with no name from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
    }
    else
    {
        CONSOLE_PRINT("Deleting object " + obj->objectName() + " from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
    }
}

void MemoryManagement::printDeletionInfo(const QString & name)
{
    CONSOLE_PRINT("Deleting object " + name + " from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
}

void MemoryManagement::printCreationInfo(QObject* obj)
{
    Q_ASSERT(QJSEngine::objectOwnership(obj) == QJSEngine::ObjectOwnership::CppOwnership);
    if (obj->objectName().isEmpty())
    {
        CONSOLE_PRINT("Creating object with no name from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
    }
    else
    {
        CONSOLE_PRINT("Creating object " + obj->objectName() + " from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
    }
}

void MemoryManagement::printCreationInfo(const QString & name)
{
    CONSOLE_PRINT("Creating object " + name + " from thread " + QThread::currentThread()->objectName() + " rendering paused " + QString::number(Mainapp::getInstance() != nullptr ? Mainapp::getInstance()->renderingPaused() : false), GameConsole::eDEBUG);    
}

quint32 MemoryManagement::getObjectCounter()
{
    return m_objectCounter;
}

void MemoryManagement::addEventListener(oxygine::spEventDispatcher dispatcher, oxygine::eventType et, const oxygine::EventCallback cb)
{
    dispatcher->addEventListener(et, cb);
}

void MemoryManagement::removeEventListener(oxygine::spEventDispatcher dispatcher, qint32 id)
{
    dispatcher->removeEventListener(id);
}

void MemoryManagement::removeEventListeners(oxygine::spEventDispatcher dispatcher, oxygine::IClosureOwner* callbackThis)
{
    dispatcher->removeEventListeners(callbackThis);
}

void MemoryManagement::setAddColor(oxygine::spVStyleActor actor, QColor color)
{
    actor->setAddColor(color);
}

void MemoryManagement::rebuildText(oxygine::spTextField actor)
{
    actor->rebuildText();
}

void MemoryManagement::changeAnimFrame(oxygine::spSprite actor, const oxygine::AnimationFrame& frame)
{
    actor->changeAnimFrame(frame);
}

void MemoryManagement::setColorTable(oxygine::spSprite actor, const oxygine::spResAnim pAnim, bool matrix)
{
    actor->setColorTable(pAnim, matrix);
}

void MemoryManagement::restartAllTweens(oxygine::spActor actor)
{
    actor->restartAllTweens();
}

void MemoryManagement::syncAllTweens(oxygine::spActor actor, oxygine::timeMS syncTime)
{
    actor->syncAllTweens(syncTime);
}

void MemoryManagement::addChild(oxygine::spActor parent, oxygine::spActor actor)
{
    parent->addChild(actor);
}

void MemoryManagement::removeChild(oxygine::spActor parent, oxygine::spActor actor)
{
    parent->removeChild(actor);
}

void MemoryManagement::setPriority(oxygine::spActor parent, qint32 zorder)
{
    parent->setPriority(zorder);
}

void MemoryManagement::addTween(oxygine::spActor parent, oxygine::spTween tween)
{
    parent->addTween(tween);
}

void MemoryManagement::removeTween(oxygine::spActor parent, oxygine::spTween tween)
{
    parent->removeTween(tween);
}

void MemoryManagement::removeChildren(oxygine::spActor actor)
{
    actor->removeChildren();
}

void MemoryManagement::removeTweens(oxygine::spActor actor)
{
    actor->removeTweens();
}
