#pragma once
#include <QPoint>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

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

    class PointerState final
    {
    public:
        explicit PointerState();
        ~PointerState() = default;

        void init(pointer_index pointerIndex);
        bool isPressed(MouseButton mb = MouseButton_Touch) const;
        pointer_index getIndex() const
        {
            return m_index;
        }
        const QPoint&  getPosition() const
        {
            return m_position;
        }

    private:
        friend class Input;
        QPoint m_position;
        qint32 m_pressed{0};
        pointer_index m_index{1};
    };
}
