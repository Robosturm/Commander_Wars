#include "EventDispatcher.h"
#include "Event.h"

namespace oxygine
{
    EventDispatcher::EventDispatcher()
        : _lastID(0),
          _listeners(nullptr)
    {

    }

    EventDispatcher::~EventDispatcher()
    {
        delete _listeners;
    }

    int EventDispatcher::addEventListener(eventType et, const EventCallback& cb)
    {
        if (!_listeners)
            _listeners = new listeners;

        _lastID++;


        listener ls;
        ls.type = et;
        ls.cb = cb;
        ls.id = _lastID;
        _listeners->push_back(ls);

        return ls.id;
    }

    void EventDispatcher::removeEventListener(int id)
    {
        if (!_listeners)
            return;

        for (size_t size = _listeners->size(), i = 0; i != size; ++i)
        {
            const listener& ls = _listeners->at(i);
            if (ls.id == id)
            {
                _listeners->erase(_listeners->begin() + i);
                break;
            }
        }
    }

    void EventDispatcher::removeEventListener(eventType et, const EventCallback& cb)
    {
         //Q_ASSERT(_listeners);
        if (!_listeners)
            return;

        for (size_t size = _listeners->size(), i = 0; i != size; ++i)
        {
            const listener& ls = _listeners->at(i);
            if (ls.type == et && cb == ls.cb)
            {
                _listeners->erase(_listeners->begin() + i);
                break;
                //Q_ASSERT(hasEventListeners(et, cb) == false);
                //--i;
            }
        }
    }

    bool EventDispatcher::hasEventListeners(void* CallbackThis)
    {
        if (!_listeners)
            return false;

        for (size_t size = _listeners->size(), i = 0; i != size; ++i)
        {
            const listener& ls = _listeners->at(i);
            if (ls.cb.p_this == CallbackThis)
                return true;
        }
        return false;
    }

    bool EventDispatcher::hasEventListeners(eventType et, const EventCallback& cb)
    {
        if (!_listeners)
            return false;

        for (size_t size = _listeners->size(), i = 0; i != size; ++i)
        {
            const listener& ls = _listeners->at(i);
            if (ls.type == et && cb == ls.cb)
                return true;
        }
        return false;
    }

    void EventDispatcher::removeEventListeners(void* CallbackThis)
    {
        if (!_listeners)
        {
            return;
        }
        for (int i = 0; i < _listeners->size(); ++i)
        {
            const listener& ls = _listeners->at(i);
            if (ls.cb.p_this == CallbackThis)
            {
                _listeners->erase(_listeners->begin() + i);
                //Q_ASSERT(hasEventListeners(CallbackThis) == false);
                --i;
            }
        }
    }

    void EventDispatcher::removeEventListenersByType(eventType et)
    {
        if (!_listeners)
        {
            return;
        }
        for (int i = 0; i < _listeners->size(); ++i)
        {
            const listener& ls = _listeners->at(i);
            if (ls.type == et)
            {
                _listeners->erase(_listeners->begin() + i);
                //Q_ASSERT(hasEventListeners(CallbackThis) == false);
                --i;
            }
        }

    }

    void EventDispatcher::removeAllEventListeners()
    {
        delete _listeners;
        _listeners = nullptr;
    }


    void EventDispatcher::dispatchEvent(Event* event)
    {
        if (!event->target)
        {
            event->target = this;
        }

        if (!_listeners || !m_enabled)
        {
            return;
        }

        size_t size = _listeners->size();
        size_t num = 0;

        listenerbase* copy = new listenerbase[size];

        for (size_t i = 0; i != size; ++i)
        {
            const listener& ls = _listeners->at(i);
            if (ls.type != event->type)
                continue;
            copy[num] = ls;
            ++num;
        }

        for (size_t i = 0; i != num; ++i)
        {
            listenerbase& ls = copy[i];
            event->currentTarget = this;
            event->listenerID = ls.id;
            ls.cb(event);
            if (event->stopsImmediatePropagation)
                break;
        }
        delete[] copy;
    }

    int EventDispatcher::getListenersCount() const
    {
        if (!_listeners)
        {
            return 0;
        }
        return (int)_listeners->size();
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
