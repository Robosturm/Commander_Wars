#include "3rd_party/oxygine-framework/oxygine/tween/TweenQueue.h"

namespace oxygine
{
    spTween TweenQueue::add(spTween t)
    {
        if (!t)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "TweenQueue::add adding empty item");
            return spTween();
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
        if (m_tweens.size() > 0)
        {
            m_current = m_tweens.first();
            Event ev(EVENT_LOOP_BEGIN);
            dispatchEvent(&ev);
            m_current->start(actor);
        }
    }

    void TweenQueue::_update(Actor& actor, const UpdateState& us)
    {
        m_elapsed += us.dt;

        if (m_current.get() != nullptr)
        {
            qint32 next = m_tweens.indexOf(m_current);
            m_current->update(actor, us);
            if (m_current->isDone())
            {
                if (next < m_tweens.size() - 1)
                {
                    m_current = m_tweens[next + 1];
                    m_current->start(actor);
                }
                else
                {
                    m_current.reset();
                }
            }

        }

        if (m_current.get() == nullptr)
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
                for (auto & tween : m_tweens)
                {
                    tween->reset();
                }
                _start(actor);
            }
        }
    }
}
