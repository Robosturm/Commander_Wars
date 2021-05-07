#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include <limits.h>
#include <qmath.h>

namespace oxygine
{
    Tween::Tween()
        : m_status(status_not_started),
          m_elapsed(0),
          m_duration(0),
          m_delay(0),
          m_loops(1),
          m_loopsDone(0),
          m_ease(ease_linear),
          m_globalEase(ease_linear),
          m_twoSides(false),
          m_disabledStatusDone(false),
          m_percent(0),
          m_detach(false),
          m_client(0)
    {

    }

    Tween::~Tween()
    {
    }

    void Tween::reset()
    {
        m_elapsed = timeMS(0);
        m_status = status_not_started;
        m_loopsDone = 0;
    }

    void Tween::init(timeMS duration, qint32 loops, bool twoSides, timeMS delay, EASE ease)
    {
        m_duration = duration;
        m_ease = ease;
        m_loops = loops;
        m_twoSides = twoSides;
        m_delay = delay;

        if (m_duration <= timeMS(0))
        {
            Q_ASSERT(!"Tweener duration should be more than ZERO");
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
            Q_ASSERT(!"Tweener duration should be more than ZERO");
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
        return v;
    }

    void Tween::setElapsed(const timeMS &elapsed)
    {
        m_elapsed = elapsed;
    }

    void Tween::removeFromActor()
    {
        oxygine::spActor pClient = getClient();
        if (pClient.get() != nullptr)
        {
            pClient->removeTween(this);
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

        Q_ASSERT(m_status == status_started);
        UpdateState us;
        us.dt = deltaTime;
        update(*m_client, us);
        Q_ASSERT(m_status == status_done);
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
        m_status = status_started;
        TweenEvent ev(this, &us);
        ev.target = ev.currentTarget = &actor;
        ev.m_tween = this;
        ev.type = TweenEvent::START;
        if (m_cbStart.isSet())
        {
            m_cbStart(&ev);
        }
        dispatchEvent(&ev);
        _start(*m_client);
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

                    if (m_globalEase != ease_linear)
                    {
                        float p = localElapsed.count() / float(m_duration.count() * m_loops);
                        timeMS nv = timeMS(static_cast<qint64>(calcEase(m_globalEase, std::min(p, 1.0f)) * m_duration.count() * m_loops));
                        localElapsed = nv;
                    }

                    qint32 loopsDone = localElapsed / m_duration;
                    m_percent = _calcEase(((float)(localElapsed.count() - loopsDone * m_duration.count())) / m_duration.count());

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
                        if (!m_disabledStatusDone)
                        {
                            m_status = status_done;
                        }
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
        ev.target = ev.currentTarget = &actor;
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

    static float outBounce(float t)
    {
        if (t < 0.363636363636f)
        {
            return 7.5625f * t * t;
        }
        if (t < 0.727272727273f)
        {
            t = t - 0.545454545455f;
            return 7.5625f * t * t + 0.75f;
        }
        if (t < 0.909090909091f)
        {
            t = t - 0.818181818182f;
            return 7.5625f * t * t + 0.9375f;
        }
        t = t - 0.954545454545f;
        return 7.5625f * t * t + 0.984375f;
    }

    Tween::easeHandler _customEaseHandler = nullptr;

    float Tween::calcEase(EASE ease, float t)
    {
        const float s = 1.70158f;

        switch (ease)
        {
            case ease_linear:
                return t;
                // quad
            case ease_inQuad:
                return (t * t);
            case ease_outQuad:
                return 1 - calcEase(ease_inQuad, 1 - t);
            case ease_inOutQuad:
                return t <= 0.5f ? calcEase(ease_inQuad, t * 2) / 2 : 1 - calcEase(ease_inQuad, 2 - t * 2) / 2;
            case ease_outInQuad:
                return t <= 0.5f ? calcEase(ease_inQuad, t * 2) / 2 : 1 - calcEase(ease_inQuad, 2 - t * 2) / 2;
                // cubic
            case ease_inCubic:
                return (t * t * t);
            case ease_outCubic:
                return 1 - calcEase(ease_inCubic, 1 - t);
            case ease_inOutCubic:
                return t <= 0.5f ? calcEase(ease_inCubic, t * 2) / 2 : 1 - calcEase(ease_inCubic, 2 - t * 2) / 2;
            case ease_outInCubic:
                return t <= 0.5f ? calcEase(ease_inCubic, t * 2) / 2 : 1 - calcEase(ease_inCubic, 2 - t * 2) / 2;
                // Quart
            case ease_inQuart:
                return (qPow(t, 4));
            case ease_outQuart:
                return 1 - calcEase(ease_inQuart, 1 - t);
            case ease_inOutQuart:
                return t <= 0.5f ? calcEase(ease_inQuart, t * 2) / 2 : 1 - calcEase(ease_inQuart, 2 - t * 2) / 2;
            case ease_outInQuart:
                return t <= 0.5f ? calcEase(ease_inQuart, t * 2) / 2 : 1 - calcEase(ease_inQuart, 2 - t * 2) / 2;

            case ease_inQuint:
                return (qPow(t, 5));
            case ease_outQuint:
                return 1 - calcEase(ease_inQuint, 1 - t);
            case ease_inOutQuint:
                return t <= 0.5f ? calcEase(ease_inQuint, t * 2) / 2 : 1 - calcEase(ease_inQuint, 2 - t * 2) / 2;
            case ease_outInQuint:
                return t <= 0.5f ? calcEase(ease_inQuint, t * 2) / 2 : 1 - calcEase(ease_inQuint, 2 - t * 2) / 2;
                // Sin
            case ease_inSin:
                return (1.0f - qCos(t * (M_PI / 2.0f)));
            case ease_outSin:
                return 1 - calcEase(ease_inSin, 1 - t);
            case ease_inOutSin:
                return t <= 0.5f ? calcEase(ease_inSin, t * 2) / 2 : 1 - calcEase(ease_inSin, 2 - t * 2) / 2;
            case ease_outInSin:
                return t <= 0.5f ? calcEase(ease_inSin, t * 2) / 2 : 1 - calcEase(ease_inSin, 2 - t * 2) / 2;
                // Expo
            case ease_inExpo:
                return (qPow(2, 10 * (t - 1)));
            case ease_outExpo:
                return 1 - calcEase(ease_inExpo, 1 - t);
            case ease_inOutExpo:
                return t <= 0.5f ? calcEase(ease_inExpo, t * 2) / 2 : 1 - calcEase(ease_inExpo, 2 - t * 2) / 2;
            case ease_outInExpo:
                return t <= 0.5f ? calcEase(ease_inExpo, t * 2) / 2 : 1 - calcEase(ease_inExpo, 2 - t * 2) / 2;
                // Circ
            case ease_inCirc:
                return (-1.0f * (qSqrt(1 - t * t) - 1));
            case ease_outCirc:
                return 1 - calcEase(ease_inCirc, 1 - t);
            case ease_inOutCirc:
                return t <= 0.5f ? calcEase(ease_inCirc, t * 2) / 2 : 1 - calcEase(ease_inCirc, 2 - t * 2) / 2;
            case ease_outInCirc:
                return t <= 0.5f ? calcEase(ease_inCirc, t * 2) / 2 : 1 - calcEase(ease_inCirc, 2 - t * 2) / 2;
                // Back
            case ease_inBack:
                return (t * t * ((s + 1) * t - s));
            case ease_outBack:
                return 1 - calcEase(ease_inBack, 1 - t);
            case ease_inOutBack:
                return t <= 0.5f ? calcEase(ease_inBack, t * 2) / 2 : 1 - calcEase(ease_inBack, 2 - t * 2) / 2;
            case ease_outInBack:
                return t <= 0.5f ? calcEase(ease_inBack, t * 2) / 2 : 1 - calcEase(ease_inBack, 2 - t * 2) / 2;

            case ease_inBounce:
                return (1 - outBounce(1 - t));
            case ease_outBounce:
                return 1 - calcEase(ease_inBounce, 1 - t);
            case ease_inOutBounce:
                return t <= 0.5f ? calcEase(ease_inBounce, t * 2) / 2 : 1 - calcEase(ease_inBounce, 2 - t * 2) / 2;
            case ease_outInBounce:
                return t <= 0.5f ? calcEase(ease_inBounce, t * 2) / 2 : 1 - calcEase(ease_inBounce, 2 - t * 2) / 2;
            default:
                t = _customEaseHandler(ease, t);
                break;
        }
        return t;
    }

    void  Tween::setCustomEaseHandler(easeHandler h)
    {
        _customEaseHandler = h;
    }
}
