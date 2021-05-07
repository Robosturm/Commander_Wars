#include "3rd_party/oxygine-framework/oxygine/tween/TweenQueue.h"

namespace oxygine
{
    spTweenQueue TweenQueue::create(spTween t1)
    {
        spTweenQueue t = spTweenQueue::create();
        t->add(t1);
        return t;
    }

    spTweenQueue TweenQueue::create(spTween t1, spTween t2)
    {
        spTweenQueue t = create(t1);
        t->add(t2);
        return t;
    }

    spTweenQueue TweenQueue::create(spTween t1, spTween t2, spTween t3)
    {
        spTweenQueue t = create(t1, t2);
        t->add(t3);
        return t;
    }

    spTweenQueue TweenQueue::create(spTween t1, spTween t2, spTween t3, spTween t4)
    {
        spTweenQueue t = create(t1, t2, t3);
        t->add(t4);
        return t;
    }

    spTweenQueue TweenQueue::create(spTween t1, spTween t2, spTween t3, spTween t4, spTween t5)
    {
        spTweenQueue t = create(t1, t2, t3, t4);
        t->add(t5);
        return t;
    }

    spTweenQueue TweenQueue::create(spTween t1, spTween t2, spTween t3, spTween t4, spTween t5, spTween t6)
    {
        spTweenQueue t = create(t1, t2, t3, t4, t5);
        t->add(t6);
        return t;
    }

    spTweenQueue TweenQueue::create(spTween t1, spTween t2, spTween t3, spTween t4, spTween t5, spTween t6, spTween t7)
    {
        spTweenQueue t = create(t1, t2, t3, t4, t5, t6);
        t->add(t7);
        return t;
    }

    TweenQueue::TweenQueue(): m_loopsDone(0) {}

    spTween TweenQueue::add(spTween t)
    {
        Q_ASSERT(t);
        if (!t)
        {
            return 0;
        }
        m_tweens.append(t);
        return t;
    }

    void TweenQueue::complete(timeMS deltaTime)
    {
        if (m_status == status_remove)
        {
            return;
        }
        UpdateState us;
        us.dt = deltaTime;
        while (m_status != status_done)
        {
            update(*m_client, us);
        }
    }

    void TweenQueue::_start(Actor& actor)
    {
        m_current = m_tweens._first;
        if (!m_current)
        {
            return;
        }
        Event ev(EVENT_LOOP_BEGIN);
        dispatchEvent(&ev);

        m_current->start(actor);
    }

    void TweenQueue::_update(Actor& actor, const UpdateState& us)
    {
        m_elapsed += us.dt;

        if (m_current)
        {
            spTween next = m_current->getNextSibling();
            m_current->update(actor, us);
            if (m_current->isDone())
            {
                m_current = next;
                if (m_current)
                {
                    m_current->start(actor);
                }
            }
        }

        if (!m_current)
        {
            Event ev(EVENT_LOOP_END);
            dispatchEvent(&ev);

            m_loopsDone++;

            if (m_loopsDone >= m_loops && m_loops > 0)
            {
                m_status = status_done;
            }
            else
            {
                spTween next = m_tweens._first;
                while (next)
                {
                    next->reset();
                    next = next->getNextSibling();
                }

                _start(actor);
            }
        }
    }
}
