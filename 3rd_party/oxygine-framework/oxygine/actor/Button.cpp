#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{

    Button::Button()
        : m_state(stateNormal),
          m_resAnim(nullptr),
          m_row(0),
          m_btnPressed(0),
          m_btnOvered(0)
    {
        EventCallback ncb = EventCallback(this, &Button::_mouseEvent);
        addEventListener(TouchEvent::TOUCH_DOWN, ncb);
        addEventListener(TouchEvent::OVER, ncb);
        addEventListener(TouchEvent::OUTX, ncb);
        addEventListener(TouchEvent::CLICK, ncb);
    }

    void Button::_mouseEvent(Event* event)
    {
        TouchEvent* me = safeCast<TouchEvent*>(event);
        if (event->type == TouchEvent::CLICK)
        {
            if (me->mouseButton == MouseButton_Left)
            {
                event->phase = Event::phase_target;
                event->target = this;
            }
            else
            {
                event->stopImmediatePropagation();
            }

            return;
        }

        if (me->mouseButton != MouseButton_Left)
        {
            return;
        }
        switch (event->type)
        {
            case TouchEvent::OVER:
            {
                if (!m_btnOvered)
                {
                    m_btnOvered = me->index;
                    if (!m_btnPressed)
                    {
                        setState(stateOvered);
                    }
                }
                break;
            }
            case TouchEvent::OUTX:
            {
                if (m_btnOvered == me->index)
                {
                    if (!m_btnPressed)
                    {
                        setState(stateNormal);
                    }
                    m_btnOvered = 0;
                }
                break;
            }
            case TouchEvent::TOUCH_DOWN:
            {
                if (!m_btnPressed)
                {
                    m_btnPressed = me->index;
                    setState(statePressed);
                    m_touchUpCbId = addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Button::_mouseEvent));
                }
            }
            break;
            case TouchEvent::TOUCH_UP:
            {
                if (m_btnPressed == me->index)
                {
                    setState(stateNormal);
                    removeEventListener(m_touchUpCbId);
                    m_btnPressed = 0;
                }
                break;
            }
        }
    }

    void Button::setResAnim(const ResAnim* r, int, int)
    {
        m_resAnim = r;
        updateButtonState(m_state);
    }

    void Button::setRow(qint32 row)
    {
        m_row = row;
        updateButtonState(m_state);
    }

    void Button::setState(state s)
    {
        if (s == m_state)
        {
            return;
        }
        m_state = s;
        updateButtonState(s);
    }

    void Button::updateButtonState(state s)
    {
        if (!m_resAnim)
        {
            return;
        }
        if (m_resAnim->getColumns() > s)
        {
            Box9Sprite::setAnimFrame(m_resAnim->getFrame(s, m_row));
        }
        else
        {
            Box9Sprite::setAnimFrame(m_resAnim->getFrame(0, m_row));
        }
    }
}
