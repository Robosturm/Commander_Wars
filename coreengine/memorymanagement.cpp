#include "coreengine/memorymanagement.h"

#include "3rd_party/oxygine-framework/oxygine/actor/VisualStyleActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

MemoryManagement MemoryManagement::m_memoryManagement;

MemoryManagement::MemoryManagement()
{
    connect(this, &MemoryManagement::sigSetAddColor, this, &MemoryManagement::setAddColor);
    connect(this, &MemoryManagement::sigRebuildText, this, &MemoryManagement::rebuildText);
    connect(this, &MemoryManagement::sigChangeAnimFrame, this, &MemoryManagement::changeAnimFrame);
    connect(this, &MemoryManagement::sigSetColorTable, this, &MemoryManagement::setColorTable);
    connect(this, &MemoryManagement::sigRestartAllTweens, this, &MemoryManagement::restartAllTweens);
    connect(this, &MemoryManagement::sigSyncAllTweens, this, &MemoryManagement::syncAllTweens);
    connect(this, &MemoryManagement::sigAddChild, this, &MemoryManagement::addChild);
    connect(this, &MemoryManagement::sigRemoveChild, this, &MemoryManagement::removeChild);
    connect(this, &MemoryManagement::sigSetPriority, this, &MemoryManagement::setPriority);
    connect(this, &MemoryManagement::sigRemoveTweens, this, &MemoryManagement::removeTweens);
    connect(this, &MemoryManagement::sigRemoveChildren, this, &MemoryManagement::removeChildren);

    connect(this, &MemoryManagement::sigAddEventListener, this, &MemoryManagement::addEventListener);
    connect(this, &MemoryManagement::sigRemoveEventListener, this, &MemoryManagement::removeEventListener);
    connect(this, &MemoryManagement::sigRemoveEventListeners, this, &MemoryManagement::removeEventListeners);
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
    parent->removeTween(tween);
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
