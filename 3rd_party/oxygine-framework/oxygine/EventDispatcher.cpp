#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include <algorithm>

#ifndef GRAPHICSUPPORT
#include "3rd_party/oxygine-framework/oxygine/TouchEvent.h"
#endif

namespace oxygine
{
    bool EventDispatcher::m_syncEvents{true};
    std::vector<EventDispatcher::EventUpdateInfo> EventDispatcher::m_eventUpdateActions;
    QMutex EventDispatcher::m_eventUpdateActionMutex;

    void EventDispatcher::setSyncEvents(bool sync)
    {
        m_syncEvents = sync;
    }

    void EventDispatcher::doUpdateInfos()
    {
        QMutexLocker locker(&m_eventUpdateActionMutex);
        for (auto & item : m_eventUpdateActions)
        {
            switch (item.action)
            {
                case EventUpdateAction::AddEventListener:
                {
                    item.dispatcher->addEventListenerWithId(item.et, item.cb);
                    break;
                }
                case EventUpdateAction::RemoveEventListenerId:
                {
                    item.dispatcher->__removeEventListener(item.id);
                    break;
                }
                case EventUpdateAction::RemoveEventListenerThis:
                {
                    item.dispatcher->__removeEventListeners(item.callbackThis);
                    break;
                }
                default:
                    Q_ASSERT(false);
            }
        }
        m_eventUpdateActions.clear();
    }

    void EventDispatcher::addEventListener(eventType et, const EventCallback cb)
    {
        if (isNotThreadsafe())
        {
            if (m_syncEvents)
            {
                emit MemoryManagement::getInstance().sigAddEventListener(getSharedPtr<EventDispatcher>(), et, cb);
            }
            else
            {
                QMutexLocker lock(&m_eventUpdateActionMutex);
                EventUpdateInfo info;
                info.action = EventUpdateAction::AddEventListener;
                info.dispatcher = getSharedPtr<EventDispatcher>();
                info.et = et;
                info.cb = cb;
                m_eventUpdateActions.push_back(info);
            }
        }
        else
        {
            addEventListenerWithId(et, cb);
        }
    }

    qint32 EventDispatcher::addEventListenerWithId(eventType et, const EventCallback & cb)
    {
        Q_ASSERT(!isNotThreadsafe());
        m_lastID++;
        const listener ls{cb, m_lastID, et};
        const auto insertPosition = std::upper_bound(m_listeners.cbegin(), m_listeners.cend(), et,
                                                     [](eventType type, const listener& item)
        {
            return type < item.type;
        });
        m_listeners.insert(insertPosition, ls);
        return ls.id;
    }

    void EventDispatcher::removeEventListener(qint32 id)
    {
        if (isNotThreadsafe())
        {
            if (m_syncEvents)
            {
                emit MemoryManagement::getInstance().sigRemoveEventListener(getSharedPtr<EventDispatcher>(), id);
            }
            else
            {
                EventUpdateInfo info;
                info.action = EventUpdateAction::RemoveEventListenerId;
                info.dispatcher = getSharedPtr<EventDispatcher>();
                info.id = id;
                QMutexLocker lock(&m_eventUpdateActionMutex);
                m_eventUpdateActions.push_back(info);
            }
        }
        else
        {
            __removeEventListener(id);
        }
    }

    void EventDispatcher::__removeEventListener(qint32 id)
    {
        const auto listenerToRemove = std::find_if(m_listeners.cbegin(), m_listeners.cend(), [id](const listener& item)
        {
            return item.id == id;
        });
        if (listenerToRemove != m_listeners.cend())
        {
            m_listeners.erase(listenerToRemove);
        }
    }

    void EventDispatcher::removeEventListeners(IClosureOwner* callbackThis)
    {
        if (isNotThreadsafe())
        {
            if (m_syncEvents)
            {
                emit MemoryManagement::getInstance().sigRemoveEventListeners(getSharedPtr<EventDispatcher>(), callbackThis);
            }
            else
            {
                EventUpdateInfo info;
                info.action = EventUpdateAction::RemoveEventListenerThis;
                info.dispatcher = getSharedPtr<EventDispatcher>();
                info.callbackThis = callbackThis;
                QMutexLocker lock(&m_eventUpdateActionMutex);
                m_eventUpdateActions.push_back(info);
            }
        }
        else
        {
            __removeEventListeners(callbackThis);
        }
    }

    void EventDispatcher::__removeEventListeners(IClosureOwner* callbackThis)
    {
        const auto newEnd = std::remove_if(m_listeners.begin(), m_listeners.end(), [callbackThis](const listener& item)
        {
            return item.cb.isOwner(callbackThis);
        });
        m_listeners.erase(newEnd, m_listeners.end());
    }

    void EventDispatcher::dispatchEvent(Event* event)
    {
        if (!m_enabled)
        {
            return;
        }
        Q_ASSERT(oxygine::GameWindow::getWindow()->isMainThread() || oxygine::GameWindow::getWindow()->renderingPaused());
        spEventDispatcher dispatcher;
        if (!event->target)
        {
            dispatcher = getSharedPtr<EventDispatcher>();
            event->target = dispatcher;
        }
        const auto firstListener = std::lower_bound(m_listeners.cbegin(), m_listeners.cend(), event->type,
                                                    [](const listener& item, eventType type)
        {
            return item.type < type;
        });
        if (firstListener == m_listeners.cend() || firstListener->type != event->type)
        {
            return;
        }
        if (!dispatcher)
        {
            dispatcher = getSharedPtr<EventDispatcher>();
        }
        qint32 i = static_cast<qint32>(firstListener - m_listeners.cbegin());
        while (i < m_listeners.size() && m_listeners[i].type == event->type)
        {
            event->currentTarget = dispatcher;
            event->listenerID = m_listeners[i].id;
            m_listeners[i].cb(event);
            if (event->stopsImmediatePropagation)
            {
                break;
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

    bool EventDispatcher::isNotThreadsafe() const
    {
#ifdef GRAPHICSUPPORT
        // Headless mode has no render pass to drain deferred updates.
        auto* window = GameWindow::getWindow();
        return !window->isMainThread() &&
               !window->renderingPaused() &&
               !window->getNoUi() &&
               !notInSharedUse();
#else
        return false;
#endif
    }
}
