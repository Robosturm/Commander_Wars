#pragma once
#include "oxygine-include.h"
#include "closure/closure.h"
#include "core/Object.h"
#include <list>

namespace oxygine
{
    static constexpr int makefourcc(unsigned int a, unsigned int b,
                                    unsigned int c, unsigned int d)
    {
        return static_cast<int>(a | (b << 8) | (c << 16) | (d << 24));
    };

    static constexpr int eventID(unsigned int a, unsigned int b,
                                 unsigned int c, unsigned int d)
    {
        return makefourcc(a, b, c, d);
    };
    static constexpr int sysEventID(unsigned int b, unsigned int c,
                                    unsigned int d)
    {
        return makefourcc(0xA, b, c, d);
    };


    DECLARE_SMART(EventDispatcher, spEventDispatcher);
    class EventDispatcher: public Object
    {
        INHERITED(Object);
    public:
        EventDispatcher(const EventDispatcher& ed)
            :
              inherited(ed),
              _lastID(0),
              _listeners(nullptr)
        {
        }
        EventDispatcher();
        ~EventDispatcher();

        int addEventListener(eventType, const EventCallback&);

        /**remove event listener by event type with callback, not working with lambda functions, works only for CLOSURE(...)*/
        void removeEventListener(eventType, const EventCallback&);

        /**remove by ID, where is ID returned from addEventListener*/
        void removeEventListener(int id);

        /**check if event listener exists by THIS used in CLOSURE(this, ...), not working with labmda*/
        bool hasEventListeners(void* CallbackThis);

        /**check if event listener exists, not working with lambda, works only with CLOSURE(...)*/
        bool hasEventListeners(eventType, const EventCallback&);

        /**removes all added event listeners by THIS used in CLOSURE(this, ...)*/
        void removeEventListeners(void* CallbackThis);

        /**removes all added event listeners by THIS used in CLOSURE(this, ...)*/
        void removeEventListenersByType(eventType);

        /**removes all added event listeners*/
        void removeAllEventListeners();

        virtual void dispatchEvent(Event* event);
        void dispatchEvent(Event& event) { dispatchEvent(&event); }

        int getListenersCount() const;
        int getLastListenerID() const { return _lastID; }

        const EventCallback& getListenerByIndex(int index) const { return (*_listeners)[index].cb; }


        bool getEnabled() const;
        virtual void setEnabled(bool enabled);

    protected:

        struct listenerbase
        {
            EventCallback cb;
            int id;
        };

        struct listener : public listenerbase
        {
            eventType type;
        };

        int _lastID;

        typedef std::vector<listener> listeners;
        listeners* _listeners;

        bool m_enabled{true};
    };
}
