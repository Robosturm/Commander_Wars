#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/math/Vector2.h"

namespace oxygine
{
    enum MouseButton
    {
        MouseButton_Touch = 0,
        MouseButton_Left = 0,
        MouseButton_Middle = 1,
        MouseButton_Right = 2,
        MouseButton_Num = 3

    };

    class PointerState
    {
    public:
        PointerState();

        void init(pointer_index pointerIndex);

        bool            isPressed(MouseButton mb = MouseButton_Touch) const;
        pointer_index   getIndex() const {return _index;}
        const Vector2&  getPosition() const {return _position;}

    private:
        friend class Input;

        Vector2 _position;
        int _pressed;
        pointer_index _index;
    };
}
