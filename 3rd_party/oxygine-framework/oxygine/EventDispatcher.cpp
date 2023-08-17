#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#ifndef GRAPHICSUPPORT
#include "3rd_party/oxygine-framework/oxygine/TouchEvent.h"
#endif

namespace oxygine
{
    qint32 EventDispatcher::addEventListener(eventType et, const EventCallback& cb)
    {
#ifndef GRAPHICSUPPORT
        if (TouchEvent::isTouchEvent(et))
        {
            return -1;
        }
#endif
        if (!oxygine::GameWindow::getWindow()->isWorker())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "trying to add listener outside worker thread");
            return -1;
        }
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
        if (!oxygine::GameWindow::getWindow()->isWorker())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "removeEventListener trying to remove listener outside worker thread");
            return;
        }
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

    void EventDispatcher::removeEventListeners(IClosureOwner* CallbackThis)
    {
        if (!oxygine::GameWindow::getWindow()->isWorker())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "removeEventListeners trying to remove listener outside worker thread");
            return;
        }
        for (qint32 i = 0; i < m_listeners.size(); ++i)
        {
            const listener& ls = m_listeners.at(i);
            if (ls.cb.isOwner(CallbackThis))
            {
                m_listeners.erase(m_listeners.cbegin() + i);
                --i;
            }
        }
    }

    void EventDispatcher::removeEventListenersByType(eventType et)
    {
        if (!oxygine::GameWindow::getWindow()->isWorker())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "removeEventListenersByType trying to remove listener outside worker thread");
            return;
        }
        for (qint32 i = 0; i < m_listeners.size(); ++i)
        {
            const listener& ls = m_listeners.at(i);
            if (ls.type == et)
            {
                m_listeners.erase(m_listeners.cbegin() + i);
                --i;
            }
        }
    }

    void EventDispatcher::removeAllEventListeners()
    {
        if (!oxygine::GameWindow::getWindow()->isWorker())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "removeAllEventListeners trying to remove listener outside worker thread");
            return;
        }
        m_listeners.clear();
    }

    void EventDispatcher::dispatchEvent(Event* event)
    {
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
