#include "3rd_party/oxygine-framework/oxygine/PointerState.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    PointerState::PointerState(): m_index(1)
    {
        init(m_index);
    }

    void PointerState::init(pointer_index index)
    {
        m_index = index;
        m_pressed = 0;
        m_position.setZero();
    }

    bool PointerState::isPressed(MouseButton mb) const
    {
        return (m_pressed & (1 << mb)) != 0;
    }
}
