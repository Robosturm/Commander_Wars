#pragma once
#include <QPoint>
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"

namespace oxygine
{
    class Event
    {
    public:
        enum
        {
            COMPLETE = sysEventID('C', 'M', 'P')
        };

        enum Phase
        {
            phase_capturing = 1,
            phase_target,
            phase_bubbling
        };

        Event(eventType Type, bool Bubbles = false)
            : listenerID(0),
              type(Type),
              phase(phase_target),
              bubbles(Bubbles),
              stopsImmediatePropagation(false),
              stopsPropagation(false)
        {
        }
        virtual ~Event() = default;

        qint32 listenerID;
        eventType type;
        Phase phase;
        bool bubbles;
        bool stopsImmediatePropagation;
        bool stopsPropagation;

        /**The event target*/
        spEventDispatcher target;
        /**The object that is actively processing the Event object with an event listener*/
        spEventDispatcher currentTarget;

        void stopPropagation()
        {
            stopsPropagation = true;
        }
        void stopImmediatePropagation()
        {
            stopsPropagation = stopsImmediatePropagation = true;
        }
        /**
        removes self from EventDispatcher, usage example
        actor->addEventListener(...buttonClicked...);
        void buttonClicked(Event* ev)
        {
            ev->removeListener();
        }
        */
        void removeListener()
        {
            currentTarget->removeEventListener(listenerID);
        }
    };

    class SlidingEvent: public Event
    {
        public:
            enum SEvent
            {
                BEGIN = sysEventID('S', 'B', 'G'),
                SLIDING = sysEventID('S', 'S', 'L'),
                END = sysEventID('S', 'E', 'N')
            };

            SlidingEvent(SEvent ev) : Event(ev) {}
            QPoint speed;
    };
}
