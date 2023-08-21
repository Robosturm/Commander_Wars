#ifndef MemoryManagement_H
#define MemoryManagement_H

#include <concepts>
#include <memory>

#include <QObject>
#include <QColor>
#include "coreengine/interpreter.h"

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

template <class Base>
class RefObject : public std::enable_shared_from_this<Base>
{
public:
    std::weak_ptr<Base> getWeakPtr()
    {
        return std::enable_shared_from_this<Base>::weak_from_this();
    }
    template <class Derived>
    std::shared_ptr<Derived> getSharedPtrFromWeak()
    {
        return std::static_pointer_cast<Derived>(std::enable_shared_from_this<Base>::weak_from_this().lock());
    }

    bool notInSharedUse() const
    {
        auto count = std::enable_shared_from_this<Base>::weak_from_this().use_count();
        return  count == 0;
    }
protected:
    template <class Derived>
    std::shared_ptr<Derived> getSharedPtr()
    {
        return std::static_pointer_cast<Derived>(std::enable_shared_from_this<Base>::shared_from_this());
    }
};

class MemoryManagement final : public QObject
{
    Q_OBJECT
public:

    static MemoryManagement & getInstance()
    {
        return m_memoryManagement;
    }

    template <class T, typename ...TArgs>
    static std::shared_ptr<T> create(TArgs... args)
    {
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        return pRet;
    }

    template <class T>
    static std::shared_ptr<T> createFromPointer(T* pPtr)
    {
        std::shared_ptr<T> pRet(pPtr, &MemoryManagement::deleter<T>);
        return pRet;
    }

    template <class T, typename ...TArgs>
    static T* createAndTrackJsObject(TArgs... args)
    {
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        Interpreter* pInterpreter = Interpreter::getInstance();
        Q_ASSERT(pInterpreter->getInJsCall());
        pInterpreter->trackJsObject(pRet);
        return pRet.get();
    }

    template <class T>
    static void deleter(T * pObj)
    {
        if constexpr (std::derived_from<T, QObject>)
        {
            pObj->deleteLater();
        }
        else
        {
            delete pObj;
        }
    }

signals:
    void sigSetAddColor(oxygine::spVStyleActor actor, QColor color);
    void sigRebuildText(oxygine::spTextField actor);
    void sigChangeAnimFrame(oxygine::spSprite actor, const oxygine::AnimationFrame& frame);
    void sigSetColorTable(oxygine::spSprite actor, const oxygine::spResAnim pAnim, bool matrix);
    void sigRestartAllTweens(oxygine::spActor actor);
    void sigSyncAllTweens(oxygine::spActor actor, oxygine::timeMS syncTime);
    void sigAddChild(oxygine::spActor parent, oxygine::spActor actor);
    void sigRemoveChild(oxygine::spActor parent, oxygine::spActor actor);
    void sigSetPriority(oxygine::spActor parent, qint32 zorder);
    void sigAddTween(oxygine::spActor parent, oxygine::spTween tween);
    void sigRemoveTween(oxygine::spActor parent, oxygine::spTween tween);
    void sigRemoveChildren(oxygine::spActor actor);
    void sigRemoveTweens(oxygine::spActor actor);
    void sigAddEventListener(oxygine::spEventDispatcher dispatcher, oxygine::eventType et, const oxygine::EventCallback cb);
    void sigRemoveEventListener(oxygine::spEventDispatcher dispatcher, qint32 id);
    void sigRemoveEventListeners(oxygine::spEventDispatcher dispatcher, oxygine::IClosureOwner* callbackThis);

private slots:
    void setAddColor(oxygine::spVStyleActor actor, QColor color);
    void rebuildText(oxygine::spTextField actor);
    void changeAnimFrame(oxygine::spSprite actor, const oxygine::AnimationFrame& frame);
    void setColorTable(oxygine::spSprite actor, const oxygine::spResAnim pAnim, bool matrix);
    void restartAllTweens(oxygine::spActor actor);
    void syncAllTweens(oxygine::spActor actor, oxygine::timeMS syncTime);
    void addChild(oxygine::spActor parent, oxygine::spActor actor);
    void removeChild(oxygine::spActor parent, oxygine::spActor actor);
    void setPriority(oxygine::spActor parent, qint32 zorder);
    void addTween(oxygine::spActor parent, oxygine::spTween tween);
    void removeTween(oxygine::spActor parent, oxygine::spTween tween);
    void removeChildren(oxygine::spActor actor);
    void removeTweens(oxygine::spActor actor);
    void addEventListener(oxygine::spEventDispatcher dispatcher, oxygine::eventType et, const oxygine::EventCallback cb);
    void removeEventListener(oxygine::spEventDispatcher dispatcher, qint32 id);
    void removeEventListeners(oxygine::spEventDispatcher dispatcher, oxygine::IClosureOwner* callbackThis);
private:
    explicit MemoryManagement();

private:
    static MemoryManagement m_memoryManagement;
};

#endif // TIMER_H
