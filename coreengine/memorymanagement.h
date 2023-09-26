#pragma once

#include <type_traits>

#include <QObject>
#include <QColor>
#include "coreengine/interpreter.h"

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

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
        ++m_objectCounter;
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        if constexpr (std::is_base_of_v<QObject, T>)
        {
            Interpreter::setCppOwnerShip(pRet.get());
        }
        return pRet;
    }

    template <class T>
    static std::shared_ptr<T> createFromPointer(T* pPtr)
    {
        ++m_objectCounter;
        std::shared_ptr<T> pRet(pPtr, &MemoryManagement::deleter<T>);
        return pRet;
    }

    template <class T, typename ...TArgs>
    static T* createAndTrackJsObject(TArgs... args)
    {
        ++m_objectCounter;
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        if constexpr (std::is_base_of_v<QObject, T>)
        {
            Interpreter::setCppOwnerShip(pRet.get());
        }
        Interpreter* pInterpreter = Interpreter::getInstance();
        Q_ASSERT(pInterpreter->getInJsCall());
        pInterpreter->trackJsObject(pRet);
        return pRet.get();
    }

    template <class T>
    static void deleter(T * pObj)
    {
        --m_objectCounter;
        if constexpr (std::is_base_of_v<QObject, T>)
        {
            printDeletionInfo(pObj);
            pObj->deleteLater();
        }
        else
        {
            printDeletionInfo("basic object");
            delete pObj;
        }
    }

    static quint32 getObjectCounter();

    static void printDeletionInfo(QObject* obj);
    static void printDeletionInfo(const QString & name);
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
    static std::atomic<quint32> m_objectCounter;
};
