#include "3rd_party/oxygine-framework/oxygine/actor/SlidingActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"
#include "3rd_party/oxygine-framework/oxygine/UpdateState.h"
#include "3rd_party/oxygine-framework/oxygine/initActor.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"

namespace oxygine
{
    float _defaultTouchThreshold = 15;
    void SlidingActor::setDefaultTouchThreshold(float val)
    {
        _defaultTouchThreshold = val;
    }

    SlidingActor::SlidingActor():
        m_sliding(false),
        m_rad(_defaultTouchThreshold),
        m_ignoreTouchUp(false),
        m_current(0), m_lastIterTime(0),
        m_finger(0)
    {
        m_clip = spClipRectActor::create();
        m_clip->addEventListener(TouchEvent::TOUCH_DOWN, EventCallback(this, &SlidingActor::_newEvent));
        m_clip->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &SlidingActor::_newEvent));
        m_clip->addEventListener(TouchEvent::MOVE, EventCallback(this, &SlidingActor::_newEvent));

        addChild(m_clip);
    }

    void SlidingActor::destroy()
    {
        m_clip->removeChildren();

        m_drag.destroy();
        m_content = nullptr;
    }

    SlidingActor::~SlidingActor()
    {
        destroy();
    }

    void SlidingActor::setTouchThreshold(float rad)
    {
        m_rad = rad;
    }

    void SlidingActor::sizeChanged(const Vector2& size)
    {
        if (m_clip)
        {
            m_clip->setSize(size);
        }
        updateDragBounds();
    }

    void SlidingActor::snap()
    {
        if (!m_content)
        {
            return;
        }
        updateDragBounds();
        m_drag.snapClient2Bounds();
        m_sliding = false;
    }

    void SlidingActor::stop()
    {
        m_speed = Vector2(0, 0);
    }

    void SlidingActor::setContent(spActor content)
    {
        if (m_content)
        {
            m_drag.destroy();
            m_content->detach();
        }

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
        m_drag.init(content.get());

        m_clip->addChild(m_content);

        updateDragBounds();
    }

    void SlidingActor::setLocked(bool locked)
    {
        m_drag.setDragEnabled(!locked);
    }

    void SlidingActor::updateDragBounds()
    {
        if (!m_content)
        {
            return;
        }
        float w = std::max(0.0f, m_content->getWidth() * m_content->getScaleX() - m_clip->getWidth());
        float h = std::max(0.0f, m_content->getHeight() * m_content->getScaleY() - m_clip->getHeight());
        RectF bounds(-w, -h, w, h);

        m_drag.setDragBounds(bounds);
    }

    const timeMS fdt = timeMS(1000 / 60);

    void SlidingActor::doUpdate(const UpdateState&)
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

    void SlidingActor::handleEvent(Event* event)
    {
        Actor::handleEvent(event);
    }

    void SlidingActor::_newEvent(Event* event)
    {
        if (!m_content)
        {
            return;
        }
        TouchEvent* te = safeCast<TouchEvent*>(event);
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
            }
            break;

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

                        if (m_speed.dot(ns) < 0)
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
            }
            break;

            case TouchEvent::MOVE:
            {
                if (te->index == m_finger)
                {
                    Vector2 offset = m_downPos - te->localPosition;
                    float d = offset.dot(offset);
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
            }
            break;
        }
    }
}
