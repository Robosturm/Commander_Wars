#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#ifndef GRAPHICSUPPORT
#include "3rd_party/oxygine-framework/oxygine/TouchEvent.h"
#endif

namespace oxygine
{

    bool EventDispatcher::detached() const
    {
        const Actor* pActor = dynamic_cast<const Actor*>(this);
        if (pActor != nullptr)
        {
            return pActor->__getStage() == nullptr;
        }
        else
        {
            return false;
        }
    }

    void EventDispatcher::addEventListener(eventType et, const EventCallback cb)
    {
        if (requiresThreadChange())
        {
            emit MemoryManagement::getInstance().sigAddEventListener(getSharedPtr<EventDispatcher>(), et, cb);
        }
        else
        {
            addEventListenerWithId(et, cb);
        }
    }

    qint32 EventDispatcher::addEventListenerWithId(eventType et, const EventCallback & cb)
    {
        OXY_ASSERT(!requiresThreadChange());
        m_lastID++;
        listener ls;
        ls.type = et;
        ls.cb = cb;
        ls.id = m_lastID;
        bool added = false;
        for (auto i = m_listeners.begin(); i != m_listeners.end(); ++i)
        {
            if (et < i->type)
            {
                m_listeners.insert(i, ls);
                added = true;
                break;
            }
        }
        if (!added)
        {
            m_listeners.push_back(ls);
        }
        return ls.id;
    }

    void EventDispatcher::removeEventListener(qint32 id)
    {
        if (requiresThreadChange())
        {
            emit MemoryManagement::getInstance().sigRemoveEventListener(getSharedPtr<EventDispatcher>(), id);
        }
        else
        {
            for (size_t size = m_listeners.size(), i = 0; i != size; ++i)
            {
                const listener& ls = m_listeners.at(i);
                if (ls.id == id)
                {

                    m_listeners.erase(m_listeners.cbegin() + i);
                    break;
                }
            }
        }
    }

    void EventDispatcher::removeEventListeners(IClosureOwner* callbackThis)
    {
        if (requiresThreadChange())
        {
            emit MemoryManagement::getInstance().sigRemoveEventListeners(getSharedPtr<EventDispatcher>(), callbackThis);
        }
        else
        {
            for (qint32 i = 0; i < m_listeners.size(); ++i)
            {
                const listener& ls = m_listeners.at(i);
                if (ls.cb.isOwner(callbackThis))
                {
                    m_listeners.erase(m_listeners.cbegin() + i);
                    --i;
                }
            }
        }
    }

    void EventDispatcher::dispatchEvent(Event* event)
    {
        if (!m_enabled || GameWindow::getWindow()->renderingPaused())
        {
            return;
        }
        OXY_ASSERT(oxygine::GameWindow::getWindow()->isMainThread());
        if (!event->target)
        {
            event->target = getSharedPtr<EventDispatcher>();
        }
        qint32 i = 0;
        while (i < m_listeners.size())
        {
            auto & listener = m_listeners[i];
            if (listener.type == event->type)
            {
                event->currentTarget = getSharedPtr<EventDispatcher>();
                event->listenerID = listener.id;
                listener.cb(event);
                if (event->stopsImmediatePropagation)
                {
                    break;
                }
            }
            ++i;
        }
    }

    bool EventDispatcher::getEnabled() const
    {
        return m_enabled;
    }

    void EventDispatcher::setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    bool EventDispatcher::requiresThreadChange() const
    {
        return !notInSharedUse() &&
               !GameWindow::getWindow()->isMainThread() &&
               !detached() &&
               !GameWindow::getWindow()->renderingPaused();
    }
}
