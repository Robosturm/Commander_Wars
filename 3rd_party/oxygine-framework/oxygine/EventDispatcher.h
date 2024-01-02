#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/closure.h"
#include "coreengine/refobject.h"
#include <vector>

namespace oxygine
{
    static constexpr qint32 makefourcc(quint32 a, quint32 b,
                                       quint32 c, quint32 d)
    {
        return static_cast<int>(a | (b << 8) | (c << 16) | (d << 24));
    };

    static constexpr qint32 eventID(quint32 a, quint32 b,
                                    quint32 c, quint32 d)
    {
        return makefourcc(a, b, c, d);
    };
    static constexpr qint32 sysEventID(quint32 b, quint32 c,
                                       quint32 d)
    {
        return makefourcc(0xA, b, c, d);
    };

    class EventDispatcher;
    using spEventDispatcher = std::shared_ptr<EventDispatcher>;
    class EventDispatcher : public IClosureOwner, public RefObject<EventDispatcher>
    {
    public:
        explicit EventDispatcher() = default;
        virtual ~EventDispatcher() = default;

        void addEventListener(eventType, const EventCallback);
        qint32 addEventListenerWithId(eventType, const EventCallback&);

        /**remove by ID, where is ID returned from addEventListener*/
        void removeEventListener(qint32 id);

        /**removes all added event listeners by THIS used in Closure(this, ...)*/
        void removeEventListeners(IClosureOwner* callbackThis);

        virtual void dispatchEvent(Event* event);
        void dispatchEvent(Event& event)
        {
            dispatchEvent(&event);
        }
        qint32 getLastListenerID() const
        {
            return m_lastID;
        }
        const EventCallback& getListenerByIndex(qint32 index) const
        {
            return m_listeners[index].cb;
        }
        bool getEnabled() const;
        virtual void setEnabled(bool enabled);
        bool requiresThreadChange() const;

    protected:
        bool detached() const;

    protected:
        struct listener
        {
            EventCallback cb;
            qint32 id;
            eventType type;
        };

        qint32 m_lastID{0};

        using listeners = std::vector<listener>;
        listeners m_listeners;

        bool m_enabled{true};
    };
}
