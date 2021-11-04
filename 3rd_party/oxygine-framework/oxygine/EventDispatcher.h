#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/closure.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
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
    using spEventDispatcher = intrusive_ptr<EventDispatcher>;
    class EventDispatcher: public Object
    {
    public:
        explicit EventDispatcher(const EventDispatcher& ed)
            : Object(ed)
        {
        }
        explicit EventDispatcher() = default;
        virtual ~EventDispatcher() = default;

        qint32 addEventListener(eventType, const EventCallback&);

        /**remove by ID, where is ID returned from addEventListener*/
        void removeEventListener(qint32 id);

        /**removes all added event listeners by THIS used in Closure(this, ...)*/
        void removeEventListeners(IClosureOwner* CallbackThis);

        /**removes all added event listeners by THIS used in Closure(this, ...)*/
        void removeEventListenersByType(eventType);

        /**removes all added event listeners*/
        void removeAllEventListeners();

        virtual void dispatchEvent(Event* event);
        void dispatchEvent(Event& event)
        {
            dispatchEvent(&event);
        }
        qint32 getListenersCount() const;
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
