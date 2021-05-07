#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"

namespace oxygine
{

    Button::Button(): _state(stateNormal), _resAnim(nullptr), _row(0), m_btnPressed(0), m_btnOvered(0)
    {
        EventCallback ncb = EventCallback(this, &Button::_mouseEvent);
        addEventListener(TouchEvent::TOUCH_DOWN, ncb);
        addEventListener(TouchEvent::OVER, ncb);
        addEventListener(TouchEvent::OUTX, ncb);
        addEventListener(TouchEvent::CLICK, ncb);
    }

    Button::~Button()
    {
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
            return;

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
            }
            break;
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
            }
            break;
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
            }
            break;
        }
    }

    void Button::setResAnim(const ResAnim* r, int, int)
    {
        _resAnim = r;
        updateButtonState(_state);
    }

    void Button::setRow(qint32 row)
    {
        _row = row;
        updateButtonState(_state);
    }

    void Button::setState(state s)
    {
        if (s == _state)
        {
            return;
        }
        _state = s;
        updateButtonState(s);
    }

    void Button::updateButtonState(state s)
    {
        if (!_resAnim)
        {
            return;
        }
        if (_resAnim->getColumns() > s)
        {
            Sprite::setAnimFrame(_resAnim->getFrame(s, _row));
        }
        else
        {
            Sprite::setAnimFrame(_resAnim->getFrame(0, _row));
        }
    }
}
