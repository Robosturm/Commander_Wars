#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"

namespace oxygine
{
    EventDispatcher::EventDispatcher()
        : _lastID(0)
    {

    }

    EventDispatcher::~EventDispatcher()
    {
    }

    int EventDispatcher::addEventListener(eventType et, const EventCallback& cb)
    {
        _lastID++;


        listener ls;
        ls.type = et;
        ls.cb = cb;
        ls.id = _lastID;
        _listeners.push_back(ls);

        return ls.id;
    }

    void EventDispatcher::removeEventListener(int id)
    {

        for (size_t size = _listeners.size(), i = 0; i != size; ++i)
        {
            const listener& ls = _listeners.at(i);
            if (ls.id == id)
            {
                _listeners.erase(_listeners.begin() + i);
                break;
            }
        }
    }

    void EventDispatcher::removeEventListener(eventType et, const EventCallback& cb)
    {
        for (size_t size = _listeners.size(), i = 0; i != size; ++i)
        {
            const listener& ls = _listeners.at(i);
            if (ls.type == et && cb == ls.cb)
            {
                _listeners.erase(_listeners.begin() + i);
                break;
            }
        }
    }

    void EventDispatcher::removeEventListeners(void* CallbackThis)
    {
        for (int i = 0; i < _listeners.size(); ++i)
        {
            const listener& ls = _listeners.at(i);
            if (ls.cb.isOwner(CallbackThis))
            {
                _listeners.erase(_listeners.begin() + i);
                --i;
            }
        }
    }

    void EventDispatcher::removeEventListenersByType(eventType et)
    {
        for (int i = 0; i < _listeners.size(); ++i)
        {
            const listener& ls = _listeners.at(i);
            if (ls.type == et)
            {
                _listeners.erase(_listeners.begin() + i);
                --i;
            }
        }

    }

    void EventDispatcher::removeAllEventListeners()
    {
        _listeners.clear();
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

        for (auto & listener : _listeners)
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

    int EventDispatcher::getListenersCount() const
    {
        return _listeners.size();
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
