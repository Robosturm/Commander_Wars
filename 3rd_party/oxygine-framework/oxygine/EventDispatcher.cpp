#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#ifndef GRAPHICSUPPORT
#include "3rd_party/oxygine-framework/oxygine/TouchEvent.h"
#endif

namespace oxygine
{
    void EventDispatcher::addEventListener(eventType et, const EventCallback cb)
    {
        if (!oxygine::GameWindow::getWindow()->isMainThread())
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
        OXY_ASSERT(oxygine::GameWindow::getWindow()->isMainThread());
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
        if (!oxygine::GameWindow::getWindow()->isMainThread())
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
        if (!oxygine::GameWindow::getWindow()->isMainThread())
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
        OXY_ASSERT(oxygine::GameWindow::getWindow()->isMainThread());
        if (!event->target)
        {
            event->target = getSharedPtr<EventDispatcher>();
        }
        if (!m_enabled)
        {
            return;
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
}
