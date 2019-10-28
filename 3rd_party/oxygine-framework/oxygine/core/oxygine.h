#pragma once
#include "../oxygine-include.h"
#include "../EventDispatcher.h"
#include "../Event.h"
#include "../math/Vector2.h"
#include <string>

/**main oxygine namespace*/
namespace oxygine
{
    namespace core
    {

        /**destroy and reset any GPU allocated memory and handles. Call it to free memory if app was minimized (lost focus)*/
        void reset();

        enum
        {
            EVENT_SYSTEM = sysEventID('c', 'S', 'y'), //events from SDL
            EVENT_PRECREATEWINDOW = sysEventID('c', 'P', 'W'),//dispatched before creating window/context
            EVENT_EXIT = sysEventID('c', 'E', 'x'), //dispatched from core::release
        };

        class PreCreateWindowEvent : public Event
        {
        public:
            PreCreateWindowEvent(): Event(EVENT_PRECREATEWINDOW) {}
            int flags;
        };

        spEventDispatcher getDispatcher();

        void init0();
    }
}
