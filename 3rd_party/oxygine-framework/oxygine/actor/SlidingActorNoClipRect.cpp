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
        m_content.free();
    }

    SlidingActorNoClipRect::~SlidingActorNoClipRect()
    {
        destroy();
    }

    void SlidingActorNoClipRect::setTouchThreshold(float rad)
    {
        m_rad = rad;
    }

    void SlidingActorNoClipRect::sizeChanged(const QSize& size)
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
        m_speed = QPoint(0, 0);
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
        m_holded.free();
        m_finger = 0;
        m_speed = QPoint(0, 0);

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
        qint32 w = std::max(0.0, m_content->getWidth() * m_content->getScaleX() - getWidth());
        qint32 h = std::max(0.0, m_content->getHeight() * m_content->getScaleY() - getHeight());
        QRect bounds(-w, -h, w + 1, h + 1);

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
            auto pos = m_content->getPosition();
            m_prev[m_current].pos = pos;
            m_prev[m_current].tm = ct;
            m_current = (m_current + 1) % NUM;
            return;
        }

        if (m_sliding)
        {
            const auto& bounds = m_drag.getDragBounds();
            while (m_lastIterTime + fdt <= ct)
            {
                auto pos = m_content->getPosition();
                auto newpos = pos + (m_speed * (static_cast<float>(fdt.count()) / 1000.0f));
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
            Settings::getInstance()->getSmallScreenDevice())
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
                        auto pos = m_content->getPosition();

                        m_holded.free();

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
                        auto midpos = mid->pos;
                        auto dir = pos - midpos;
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

                            QPoint ns = (dr * 1000.0f) / v.count();

                            if (m_speed.x() *ns.x() + m_speed.y() * ns.y() < 0)
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
                                act = spActor(act->getParent());
                            }

                            m_holded.free();
                        }
                    }
                    break;
                }
            }
        }
    }
}
