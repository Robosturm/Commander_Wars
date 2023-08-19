#include "3rd_party/oxygine-framework/oxygine/actor/SlidingActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"
#include "3rd_party/oxygine-framework/oxygine/UpdateState.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"

namespace oxygine
{
    SlidingActor::SlidingActor()
    {
        m_clip = MemoryManagement::create<ClipRectActor>();
        m_clip->addEventListener(TouchEvent::TOUCH_DOWN, EventCallback(this, &SlidingActor::_newEvent));
        m_clip->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &SlidingActor::_newEvent));
        m_clip->addEventListener(TouchEvent::MOVE, EventCallback(this, &SlidingActor::_newEvent));

        addChild(m_clip);
    }

    SlidingActor::~SlidingActor()
    {
        destroy();
    }

    void SlidingActor::destroy()
    {
        m_clip->removeChildren();
        m_drag.destroy();
        m_content.reset();
    }

    void SlidingActor::setTouchThreshold(float rad)
    {
        m_rad = rad;
    }

    void SlidingActor::sizeChanged(const QSize& size)
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
        m_speed = QPoint(0, 0);
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
        m_holded.reset();
        m_finger = 0;
        m_speed = QPoint(0, 0);

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
        qint32 w = std::max(0.0, m_content->getWidth() * m_content->getScaleX() - m_clip->getWidth());
        qint32 h = std::max(0.0, m_content->getHeight() * m_content->getScaleY() - m_clip->getHeight());
        QRect bounds(-w, -h, w + 1, h + 1);

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
            QPoint pos = m_content->getPosition();
            m_prev[m_current].pos = pos;
            m_prev[m_current].tm = ct;
            m_current = (m_current + 1) % NUM;
            return;
        }

        if (m_sliding)
        {
            const QRect& bounds = m_drag.getDragBounds();
            while (m_lastIterTime + fdt <= ct)
            {
                QPoint pos = m_content->getPosition();
                QPoint newpos = pos + (m_speed * (static_cast<float>(fdt.count()) / 1000.0f));
                if (newpos.x() < bounds.left())
                {
                    newpos.setX(bounds.left());
                    m_speed.setX(0);
                }
                else if (newpos.x() > bounds.right())
                {
                    newpos.setX(bounds.right());
                    m_speed.setX(0);
                }

                if (newpos.y() < bounds.top())
                {
                    newpos.setY(bounds.top());
                    m_speed.setY(0);
                }
                else if (newpos.y() > bounds.bottom())
                {
                    newpos.setY(bounds.bottom());
                    m_speed.setY(0);
                }
                m_speed *= 0.97f;
                m_content->setPosition(newpos);

                m_lastIterTime += fdt;
            }


            SlidingEvent sl(SlidingEvent::SLIDING);
            sl.speed = m_speed;
            dispatchEvent(&sl);
            m_speed = sl.speed;

            if (m_speed.x() * m_speed.x() +
                m_speed.y() * m_speed.y() < 8)
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
                    QPoint pos = m_content->getPosition();

                    m_holded.reset();

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
                    QPoint midpos = mid->pos;
                    QPoint dir = pos - midpos;
                    if (dir.x() * dir.x() +
                        dir.y() * dir.y() < 10 * 10)
                    {
                        m_speed = QPoint(0, 0);
                    }
                    else
                    {
                        timeMS v = tm - old->tm;
                        if (v == timeMS(0))
                        {
                            return;
                        }

                        QPoint dr = pos - old->pos;

                        QPoint ns = dr;
                        if (v.count() > 0)
                        {
                            ns = (dr * 1000.0f) / v.count();
                        }

                        if (m_speed.x() * ns.x() + m_speed.y() * ns.y() < 0)
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
                    QPoint offset = m_downPos - te->localPosition;
                    float d = offset.x() * offset.x() + offset.y() * offset.y();
                    if (m_holded && (d >= m_rad * m_rad))
                    {
                        spActor act = safeSpCast<Actor>(m_holded);

                        while (act && act.get() != m_content.get())
                        {
                            for (qint32 i = 0; i < MouseButton_Num; ++i)
                            {
                                act->setNotPressed((MouseButton)i);

                                TouchEvent ev(TouchEvent::TOUCH_UP, true, QPoint(-100000, -100000));
                                ev.mouseButton = (MouseButton)i;
                                ev.index = te->index;
                                ev.bubbles = false;
                                act->dispatchEvent(&ev);

                            }
                            auto* parent = act->getParent();
                            if (parent != nullptr)
                            {
                                act = parent->getSharedPtr<Actor>();
                            }
                            else
                            {
                                act.reset();
                            }
                        }

                        m_holded.reset();
                    }
                }
            }
            break;
        }
    }
}
