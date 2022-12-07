#include "3rd_party/oxygine-framework/oxygine/actor/SlidingActorNoClipRect.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"
#include "3rd_party/oxygine-framework/oxygine/UpdateState.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"

#include "coreengine/settings.h"

namespace oxygine
{

    SlidingActorNoClipRect::SlidingActorNoClipRect()
    {
        m_drag.setNoLockForMiddleButton(true);
    }

    void SlidingActorNoClipRect::destroy()
    {
        removeChildren();
        m_drag.destroy();
        m_content = nullptr;
    }

    SlidingActorNoClipRect::~SlidingActorNoClipRect()
    {
        destroy();
    }

    void SlidingActorNoClipRect::setTouchThreshold(float rad)
    {
        m_rad = rad;
    }

    void SlidingActorNoClipRect::sizeChanged(const Vector2& size)
    {
        updateDragBounds();
    }

    void SlidingActorNoClipRect::snap()
    {
        if (!m_content)
        {
            return;
        }
        updateDragBounds();
        m_drag.snapClient2Bounds();
        m_sliding = false;
    }

    void SlidingActorNoClipRect::stop()
    {
        m_speed = Vector2(0, 0);
    }

    void SlidingActorNoClipRect::setContent(spActor content)
    {
        if (m_content.get() != nullptr)
        {
            m_content->removeEventListener(m_touchDownId);
            m_content->removeEventListener(m_touchUpId);
            m_content->removeEventListener(m_touchMoveId);
            m_drag.destroy();
            m_content->detach();
        }
        m_touchDownId = content->addEventListener(TouchEvent::TOUCH_DOWN, EventCallback(this, &SlidingActorNoClipRect::_newEvent));
        m_touchUpId = content->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &SlidingActorNoClipRect::_newEvent));
        m_touchMoveId = content->addEventListener(TouchEvent::MOVE, EventCallback(this, &SlidingActorNoClipRect::_newEvent));

        m_current = 0;
        m_lastIterTime = timeMS(0);
        m_sliding = false;

        for (qint32 i = 0; i < NUM; ++i)
        {
            m_prev[i].tm = timeMS(0);
        }
        m_holded = nullptr; //event->target;
        m_finger = 0;
        m_speed = Vector2(0, 0);

        m_content = content;
        m_drag.init(m_content.get());
        addChild(m_content);
        updateDragBounds();
    }

    void SlidingActorNoClipRect::setLocked(bool locked)
    {
        m_drag.setDragEnabled(!locked);
    }

    void SlidingActorNoClipRect::updateDragBounds()
    {
        if (!m_content)
        {
            return;
        }
        float w = std::max(0.0f, m_content->getWidth() * m_content->getScaleX() - getWidth());
        float h = std::max(0.0f, m_content->getHeight() * m_content->getScaleY() - getHeight());
        RectF bounds(-w, -h, w, h);

        m_drag.setDragBounds(bounds);
    }

    const timeMS fdt = timeMS(1000 / 60);

    void SlidingActorNoClipRect::doUpdate(const UpdateState&)
    {
        if (!m_content)
        {
            return;
        }

        timeMS ct = Clock::getTimeMS();
        if (m_lastIterTime + NUM * fdt < ct)
        {
            m_lastIterTime = ct;
        }

        if (m_drag.isDragging())
        {
            Vector2 pos = m_content->getPosition();
            m_prev[m_current].pos = pos;
            m_prev[m_current].tm = ct;
            m_current = (m_current + 1) % NUM;
            return;
        }

        if (m_sliding)
        {
            const RectF& bounds = m_drag.getDragBounds();
            while (m_lastIterTime + fdt <= ct)
            {
                Vector2 pos = m_content->getPosition();
                Vector2 newpos = pos + m_speed * (fdt.count() / 1000.0f);
                if (newpos.x < bounds.getLeft())
                {
                    newpos.x = bounds.getLeft();
                    m_speed.x = 0;
                }
                else if (newpos.x > bounds.getRight())
                {
                    newpos.x = bounds.getRight();
                    m_speed.x = 0;
                }

                if (newpos.y < bounds.getTop())
                {
                    newpos.y = bounds.getTop();
                    m_speed.y = 0;
                }
                else if (newpos.y > bounds.getBottom())
                {
                    newpos.y = bounds.getBottom();
                    m_speed.y = 0;
                }
                m_speed *= 0.97f;
                m_content->setPosition(newpos);

                m_lastIterTime += fdt;
            }


            SlidingEvent sl(SlidingEvent::SLIDING);
            sl.speed = m_speed;
            dispatchEvent(&sl);
            m_speed = sl.speed;

            if (m_speed.sqlength() < 8)
            {
                m_sliding = false;
                SlidingEvent ev(SlidingEvent::END);
                dispatchEvent(&ev);
            }
        }

    }

    void SlidingActorNoClipRect::handleEvent(Event* event)
    {
        Actor::handleEvent(event);
    }

    void SlidingActorNoClipRect::_newEvent(Event* event)
    {
        if (!m_content || !m_enabled)
        {
            return;
        }
        TouchEvent* te = safeCast<TouchEvent*>(event);

        if (te->mouseButton == MouseButton_Middle ||
            Settings::getSmallScreenDevice())
        {
            timeMS tm = Clock::getTimeMS();
            switch (te->type)
            {
                case TouchEvent::TOUCH_DOWN:
                {
                    m_finger = te->index;
                    m_current = 0;
                    m_lastIterTime = tm;

                    m_prev[0].pos = m_content->getPosition();
                    m_prev[0].tm = tm;

                    for (qint32 i = 1; i < NUM; ++i)
                    {
                        m_prev[i].tm = timeMS(0);
                    }

                    m_holded = event->target;
                    m_downPos = te->localPosition;
                    break;
                }
                case TouchEvent::TOUCH_UP:
                {
                    if (m_ignoreTouchUp)
                    {
                        te->stopImmediatePropagation();
                    }

                    if (m_drag.getDragEnabled() && te->index == m_finger && m_ignoreTouchUp == false)
                    {
                        m_finger = 0;
                        Vector2 pos = m_content->getPosition();

                        m_holded = nullptr;

                        const iter* old = 0;
                        const iter* mid = 0;
                        const iter* last = m_prev + m_current;

                        for (qint32 i = 1; i < NUM; ++i)
                        {
                            qint32 n = (m_current + NUM - i) % NUM;
                            if (m_prev[n].tm > timeMS(0))
                            {
                                last = m_prev + n;
                            }
                            else
                            {
                                break;
                            }
                            if (!mid && (last->tm + timeMS(50) <= tm))
                            {
                                mid = last;
                            }
                            if (last->tm + timeMS(150) <= tm)
                            {
                                old = last;
                                break;
                            }
                        }
                        if (!old)
                        {
                            old = last;
                        }
                        if (!mid)
                        {
                            mid = last;
                        }
                        Vector2 midpos = mid->pos;
                        Vector2 dir = pos - midpos;
                        if (dir.sqlength() < 10 * 10)
                        {
                            m_speed = Vector2(0, 0);
                        }
                        else
                        {
                            timeMS v = tm - old->tm;
                            if (v == timeMS(0))
                            {
                                return;
                            }

                            Vector2 dr = pos - old->pos;

                            Vector2 ns = (dr * 1000.0f) / v.count();

                            if (m_speed.x *ns.x + m_speed.y * ns.y < 0)
                            {
                                m_speed = ns;
                            }
                            else
                            {
                                m_speed += ns;
                            }
                        }


                        if (!m_sliding)
                        {
                            m_sliding = true;
                        }

                        SlidingEvent sd(SlidingEvent::BEGIN);
                        sd.speed = m_speed;
                        dispatchEvent(&sd);
                        m_speed = sd.speed;

                        m_lastIterTime = tm;
                    }
                    break;
                }
                case TouchEvent::MOVE:
                {
                    if (te->index == m_finger)
                    {
                        Vector2 offset = m_downPos - te->localPosition;
                        float d = offset.x * offset.x + offset.y * offset.y;
                        if (m_holded && (d >= m_rad * m_rad))
                        {
                            spActor act = safeSpCast<Actor>(m_holded);

                            while (act && act.get() != m_content.get())
                            {
                                for (qint32 i = 0; i < MouseButton_Num; ++i)
                                {
                                    act->setNotPressed((MouseButton)i);

                                    TouchEvent ev(TouchEvent::TOUCH_UP, true, Vector2(-100000, -100000));
                                    ev.mouseButton = (MouseButton)i;
                                    ev.index = te->index;
                                    ev.bubbles = false;
                                    act->dispatchEvent(&ev);

                                }
                                act = act->getParent();
                            }

                            m_holded = nullptr;
                        }
                    }
                    break;
                }
            }
        }
    }
}
