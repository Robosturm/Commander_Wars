#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Input.h"
#include "3rd_party/oxygine-framework/oxygine/closure/closure.h"

namespace oxygine
{
    class Event
    {
    public:
        enum { COMPLETE = sysEventID('C', 'M', 'P') };

        enum Phase
        {
            phase_capturing = 1,
            phase_target,
            phase_bubbling
        };

        Event(eventType Type, bool Bubbles = false) : listenerID(0), type(Type), phase(phase_target), bubbles(Bubbles), stopsImmediatePropagation(false), stopsPropagation(false) { userData64 = 0; }
        virtual ~Event() {}

        int listenerID;
        union
        {
            quint64 userData64;
            int    userData32;
            bool   userDataBool;
            void*  userData;
        };
        
        eventType type;
        Phase phase;
        bool bubbles;
        bool stopsImmediatePropagation;
        bool stopsPropagation;

        spObject userDataObject;

        /**The event target*/
        spEventDispatcher target;
        /**The object that is actively processing the Event object with an event listener*/
        spEventDispatcher currentTarget;

        void stopPropagation() { stopsPropagation = true; }
        void stopImmediatePropagation() { stopsPropagation = stopsImmediatePropagation = true; }

        /**
        removes self from EventDispatcher, usage example
        actor->addEventListener(...buttonClicked...);
        void buttonClicked(Event* ev)
        {
            ev->removeListener();
        }
        */
        void removeListener() { currentTarget->removeEventListener(listenerID); }
    };
}
