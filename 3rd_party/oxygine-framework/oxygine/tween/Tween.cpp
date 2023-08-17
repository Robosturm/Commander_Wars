#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "coreengine/gameconsole.h"

#include <limits.h>
#include <QtMath>

namespace oxygine
{
    void Tween::reset()
    {
        m_elapsed = timeMS(0);
        m_status = status_not_started;
        m_loopsDone = 0;
    }

    void Tween::init(timeMS duration, qint32 loops, bool twoSides, timeMS delay, QEasingCurve::Type ease)
    {
        m_duration = duration;
        m_ease = ease;
        m_loops = loops;
        m_twoSides = twoSides;
        m_delay = delay;

        if (m_duration <= timeMS(0))
        {
            CONSOLE_PRINT("Tween::init Tweener duration should be more than ZERO", GameConsole::eDEBUG);
            m_duration = timeMS(1);
        }
    }

    void Tween::init2(const TweenOptions& opt)
    {
        m_duration = opt.m_duration;
        m_ease = opt.m_ease;
        m_loops = opt.m_loops;
        m_twoSides = opt.m_twoSides;
        m_delay = opt.m_delay;
        m_detach = opt.m_detach;
        m_globalEase = opt.m_globalEase;
        m_cbDone = opt.m_callback;

        if (m_duration == timeMS(0))
        {
            m_duration = timeMS(1);
        }
        else if (m_duration < timeMS(0))
        {
            CONSOLE_PRINT("Tween::init2 Tweener duration should be more than ZERO", GameConsole::eDEBUG);
            m_duration = timeMS(1);
        }
    }

    void Tween::setDoneCallback(const EventCallback& cb)
    {
        m_cbDone = cb;
    }

    void Tween::setStartCallback(const EventCallback& cb)
    {
        m_cbStart = cb;
    }

    void Tween::addDoneCallback(const EventCallback& cb)
    {
        addEventListener(TweenEvent::DONE, cb);
    }

    float Tween::_calcEase(float v)
    {
        if (m_twoSides)
        {
            if (v > 0.5f)
            {
                v = 1.0f - v;
            }
            v *= 2.0f;
        }

        v = calcEase(m_ease, v);
        if (v > 1.0f)
        {
            v = 1.0f;
        }
        return v;
    }

    void Tween::setElapsed(const timeMS &elapsed)
    {
        m_elapsed = elapsed;
    }

    void Tween::removeFromActor()
    {
        oxygine::spActor pClient = getClient()->getSharedPtr<Actor>();
        if (pClient.get() != nullptr)
        {
            spTween pTween = getSharedPtr<Tween>();
            pClient->removeTween(pTween);
        }
    }

    void Tween::complete(timeMS deltaTime)
    {
        if (m_loops == -1)
        {
            return;
        }
        //if already done
        if (m_status >= status_done)
        {
            return;
        }
        if (!m_client)
        {
            return;
        }
        //not started yet because has delay
        if (m_status == status_delayed)
        {
            _start(*m_client);
            m_status = status_started;
        }

        if (m_status != status_started)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Tween::complete finishing none started tween");
        }
        UpdateState us;
        us.dt = deltaTime;
        update(*m_client, us);
        if (m_status != status_done)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Tween::complete tween not done after completion");
        }
    }

    void Tween::start(Actor& actor)
    {
        m_client = &actor;
        m_status = status_delayed;
        if (m_delay == timeMS(0))
        {
            const UpdateState us;
            __start(actor, us);
        }
    }

    void Tween::__start(Actor& actor, const UpdateState& us)
    {
        TweenEvent ev(this, &us);
        ev.currentTarget = actor.getSharedPtr<Actor>();
        ev.target = ev.currentTarget;
        ev.m_tween = this;
        ev.type = TweenEvent::START;
        if (m_cbStart.isSet())
        {
            m_cbStart(&ev);
        }
        dispatchEvent(&ev);
        _start(*m_client);
        m_status = status_started;
    }

    void Tween::update(Actor& actor, const UpdateState& us)
    {
        m_elapsed += us.dt;
        switch (m_status)
        {
            case status_delayed:
            {
                if (m_elapsed >= m_delay)
                {
                    __start(actor, us);
                }
                break;
            }
            case status_started:
            {
                if (m_duration > timeMS(0))
                {
                    timeMS localElapsed = m_elapsed - m_delay;

                    if (m_globalEase != QEasingCurve::Linear)
                    {
                        float p = localElapsed.count() / static_cast<float>(m_duration.count() * m_loops);
                        timeMS nv = timeMS(static_cast<qint64>(calcEase(m_globalEase, std::min(p, 1.0f)) * m_duration.count() * m_loops));
                        localElapsed = nv;
                    }
                    qint32 loopsDone = localElapsed / m_duration;
                    m_percent = _calcEase(static_cast<float>(localElapsed.count() - loopsDone * m_duration.count()) / static_cast<float>(m_duration.count()));

                    while(m_loopsDone < loopsDone)
                    {
                        _loopDone(actor, us);
                        m_loopsDone++;
                    }

                    if (m_loops > 0 && static_cast<qint32>(loopsDone) >= m_loops)
                    {
                        if (m_twoSides)
                        {
                            m_percent = 0;
                        }
                        else
                        {
                            m_percent = 1;
                        }                        
                        m_status = status_done;
                    }
                }
                _update(*m_client, us);
                break;
            }
            case status_done:
            {
                done(*m_client, us);
                break;
            }
            default:
                break;
        }
    }

    void Tween::done(Actor& actor, const UpdateState& us)
    {
        _done(actor, us);

        if (m_detach)
        {
            actor.detach();
        }

        TweenEvent ev(this, &us);
        ev.currentTarget = actor.getSharedPtr<Actor>();
        ev.target = ev.currentTarget;
        ev.m_tween = this;

        if (m_cbDone.isSet())
        {
            m_cbDone(&ev);
        }

        dispatchEvent(&ev);

        m_status = status_remove;
    }

    Actor* TweenEvent::getActor() const
    {
        return safeCast<Actor*>(currentTarget.get());
    }

    float Tween::calcEase(QEasingCurve::Type ease, float t)
    {
        static QEasingCurve curve;
        curve.setType(ease);
        return curve.valueForProgress(t);
    }
}
