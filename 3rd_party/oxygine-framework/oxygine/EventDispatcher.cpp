#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"

namespace oxygine
{
    EventDispatcher::EventDispatcher()
        : m_lastID(0)
    {

    }

    EventDispatcher::~EventDispatcher()
    {
    }

    qint32 EventDispatcher::addEventListener(eventType et, const EventCallback& cb)
    {
        m_lastID++;
        listener ls;
        ls.type = et;
        ls.cb = cb;
        ls.id = m_lastID;
        bool added = false;
        for (qint32 i = 0; i < m_listeners.size(); ++i)
        {
            if (et < m_listeners[i].type)
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

        for (size_t size = m_listeners.size(), i = 0; i != size; ++i)
        {
            const listener& ls = m_listeners.at(i);
            if (ls.id == id)
            {
                m_listeners.erase(m_listeners.begin() + i);
                break;
            }
        }
    }

    void EventDispatcher::removeEventListeners(IClosureOwner* CallbackThis)
    {
        for (qint32 i = 0; i < m_listeners.size(); ++i)
        {
            const listener& ls = m_listeners.at(i);
            if (ls.cb.isOwner(CallbackThis))
            {
                m_listeners.erase(m_listeners.begin() + i);
                --i;
            }
        }
    }

    void EventDispatcher::removeEventListenersByType(eventType et)
    {
        for (qint32 i = 0; i < m_listeners.size(); ++i)
        {
            const listener& ls = m_listeners.at(i);
            if (ls.type == et)
            {
                m_listeners.erase(m_listeners.begin() + i);
                --i;
            }
        }

    }

    void EventDispatcher::removeAllEventListeners()
    {
        m_listeners.clear();
    }

    void EventDispatcher::dispatchEvent(Event* event)
    {
        if (!event->target)
        {
            event->target = this;
        }

        if (!m_enabled)
        {
            return;
        }
        // make sure listeners can be added/removed while processing the current event to this dispatcher
        auto currentListener = m_listeners;
        for (auto & listener : currentListener)
        {
            if (listener.type == event->type)
            {
                event->currentTarget = this;
                event->listenerID = listener.id;
                listener.cb(event);
                if (event->stopsImmediatePropagation)
                {
                    break;
                }
            }
        }
    }

    qint32 EventDispatcher::getListenersCount() const
    {
        return m_listeners.size();
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
