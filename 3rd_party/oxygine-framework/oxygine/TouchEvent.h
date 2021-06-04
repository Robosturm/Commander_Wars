#pragma once
#include "oxygine-include.h"
#include "Event.h"
namespace oxygine
{
    class TouchEvent : public Event
    {
    public:
        enum
        {
            __FIRST = sysEventID('T', 'O', 0),
            CLICK,
            OVER,
            MOVE,
            TOUCH_DOWN,
            TOUCH_UP,
            WHEEL_DIR,
            OUTX,       //OUT, X - avoid conflicts with WinAPI
            __LAST      //system
        };


        TouchEvent(eventType type, bool Bubbles = true, const Vector2& locPosition = Vector2(0, 0)) : Event(type, Bubbles), localPosition(locPosition), position(locPosition), mouseButton(MouseButton_Touch), pressure(1.0f), index(1), __clickDispatched(false), wheelDirection(0, 0), __localScale(1.0f) {}

        /**position in local space for Event::currentTarget Actor*/
        Vector2 localPosition;
        /**position in local space for Event::target actor*/
        Vector2 position;

        MouseButton mouseButton;
        float pressure;

        pointer_index index;

        const PointerState* getPointer() const;

        static bool isTouchEvent(qint32 eventID) { return eventID > __FIRST && eventID < __LAST; }

        bool __clickDispatched;
        Vector2 wheelDirection;//actual only for WHEEL_DIR event

        float __localScale;
    };
}
