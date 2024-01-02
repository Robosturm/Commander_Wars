#include "3rd_party/oxygine-framework/oxygine/Input.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    spInput Input::m_instance;

    Input::Input()
    {
        m_pointerMouse.init(MAX_TOUCHES + 1);
        for (qint32 i = 0; i < MAX_TOUCHES; ++i)
        {
            m_pointers[i].init(i + 1);
        }
        memset(m_ids, 0, sizeof(m_ids));
        m_multiTouch = true;
    }

    void Input::handlePausePointerEvents()
    {
        Q_ASSERT(oxygine::GameWindow::getWindow()->isMainThread());
        Q_ASSERT(!GameWindow::getWindow()->renderingPaused());
        spStage stage = oxygine::Stage::getStage();
        if (m_movePauseEvent.type == TouchEvent::MOVE)
        {
            stage->handleEvent(&m_movePauseEvent);
            m_movePauseEvent.type = TouchEvent::__FIRST;
        }
        if (m_downPauseEvent.type == TouchEvent::TOUCH_DOWN)
        {
            stage->handleEvent(&m_downPauseEvent);
            m_downPauseEvent.type = TouchEvent::__FIRST;
        }
        if (m_upPauseEvent.type == TouchEvent::TOUCH_UP)
        {
            stage->handleEvent(&m_upPauseEvent);
            m_upPauseEvent.type = TouchEvent::__FIRST;
        }
        if (m_wheelPauseEvent.type == TouchEvent::WHEEL_DIR)
        {
            stage->handleEvent(&m_wheelPauseEvent);
            m_wheelPauseEvent.type = TouchEvent::__FIRST;
        }
    }

    const PointerState* TouchEvent::getPointer() const
    {
        return Input::getInstance().getTouchByIndex(index);
    }

    PointerState* Input::getPointerMouse()
    {
        return &m_pointerMouse;
    }

    void Input::sendPointerButtonEvent(spStage & stage, MouseButton button, qint32 x, qint32 y, float pressure, qint32 type, PointerState* ps)
    {
        Q_ASSERT(oxygine::GameWindow::getWindow()->isMainThread());
        if (!m_multiTouch && ps->getIndex() != 1 && ps != &m_pointerMouse)
        {
            if (type == TouchEvent::TOUCH_UP)
            {
                m_ids[ps->getIndex() - 1] = 0;
            }
            return;
        }

        QPoint p(x, y);
        TouchEvent me(type, true, p);
        me.index = ps->getIndex();
        me.mouseButton = button;
        me.pressure = pressure;

        if (type == TouchEvent::TOUCH_DOWN)
        {
            ps->m_pressed |= 1 << button;
        }
        else if (type == TouchEvent::TOUCH_UP)
        {
            ps->m_pressed &= ~(1 << button);
        }
        ps->m_position = p;
        if (GameWindow::getWindow()->renderingPaused())
        {
            if (type == TouchEvent::TOUCH_DOWN)
            {
                m_downPauseEvent = me;
            }
            else if (type == TouchEvent::TOUCH_UP)
            {
                m_upPauseEvent = me;
            }
            return;
        }
        else if (stage != nullptr)
        {
            stage->handleEvent(&me);
        }

        if (type == TouchEvent::TOUCH_UP)
        {
            m_ids[ps->getIndex() - 1] = 0;
        }
    }

    void Input::sendPointerMotionEvent(spStage & stage, qint32 x, qint32 y, float pressure, PointerState* ps)
    {
        Q_ASSERT(oxygine::GameWindow::getWindow()->isMainThread());
        if (!m_multiTouch && ps->getIndex() != 1 && ps != &m_pointerMouse)
        {
            return;
        }
        TouchEvent me(TouchEvent::MOVE, true, QPoint(x, y));
        me.index = ps->getIndex();
        me.pressure = pressure;
        ps->m_position = QPoint(x, y);
        if (GameWindow::getWindow()->renderingPaused())
        {
            m_movePauseEvent = me;
            return;
        }
        else if (stage != nullptr)
        {
            stage->handleEvent(&me);
        }
    }

    void Input::sendPointerWheelEvent(spStage & stage, const QPoint& dir, PointerState* ps)
    {
        Q_ASSERT(oxygine::GameWindow::getWindow()->isMainThread());
        TouchEvent te(TouchEvent::WHEEL_DIR, true, ps->getPosition());
        te.index = ps->getIndex();
        te.wheelDirection = dir;
        if (GameWindow::getWindow()->renderingPaused())
        {
            m_wheelPauseEvent = te;
            return;
        }
        else if (stage != nullptr)
        {
            stage->handleEvent(&te);
        }
    }

    void Input::cleanup()
    {
    }

    void Input::multiTouchEnabled(bool en)
    {
        m_multiTouch = en;
    }

    PointerState* Input::getTouchByIndex(pointer_index index_)
    {
        if (index_ == 0)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Input::getTouchByIndex index is 0");
        }

        qint32 index = index_;
        if (index == MAX_TOUCHES + 1)
        {
            return &m_pointerMouse;
        }
        index -= 1;
        if (index < 0 || index >= MAX_TOUCHES)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Input::getTouchByIndex index is out of valid range");
        }
        index = std::min(std::max(index, 0), MAX_TOUCHES);
        return &m_pointers[index];
    }

    qint32 Input::touchID2index(qint64 id)
    {
        id += 1;
        qint32 firstEmptySlotIndex = -1;
        for (qint32 i = 0; i < MAX_TOUCHES; ++i)
        {
            qint64& d = m_ids[i];

            if (d == id)
            {
                return i + 1;
            }
            if (d == 0 && firstEmptySlotIndex == -1)
            {
                firstEmptySlotIndex = i;
            }
        }

        if (firstEmptySlotIndex != -1)
        {
            m_ids[firstEmptySlotIndex] = id;
            return firstEmptySlotIndex + 1;
        }
        return -1;
    }

    PointerState* Input::getTouchByID(qint64 id)
    {
        qint32 i = touchID2index(id);
        if (i == -1)
        {
            return 0;
        }
        return getTouchByIndex(i);
    }
}
