#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include <limits.h>
#include <qmath.h>

namespace oxygine
{
    Tween::Tween()
        : _status(status_not_started),
          _elapsed(0),
          _duration(0),
          _delay(0),
          _loops(1),
          _loopsDone(0),
          _ease(ease_linear),
          _globalEase(ease_linear),
          _twoSides(false),
          _disabledStatusDone(false),
          _percent(0),
          _detach(false),
          _client(0)
    {

    }

    Tween::~Tween()
    {
    }

    void Tween::reset()
    {
        _elapsed = timeMS(0);
        _status = status_not_started;
        _loopsDone = 0;
    }

    void Tween::init(timeMS duration, int loops, bool twoSides, timeMS delay, EASE ease)
    {
        _duration = duration;
        _ease = ease;
        _loops = loops;
        _twoSides = twoSides;
        _delay = delay;

        if (_duration <= timeMS(0))
        {
            Q_ASSERT(!"Tweener duration should be more than ZERO");
            _duration = timeMS(1);
        }
    }

    void Tween::init2(const TweenOptions& opt)
    {
        _duration = opt._duration;
        _ease = opt._ease;
        _loops = opt._loops;
        _twoSides = opt._twoSides;
        _delay = opt._delay;
        _detach = opt._detach;
        _globalEase = opt._globalEase;
        _cbDone = opt._callback;

        if (_duration == timeMS(0))
        {
            _duration = timeMS(1);
        }
        else if (_duration < timeMS(0))
        {
            Q_ASSERT(!"Tweener duration should be more than ZERO");
            _duration = timeMS(1);
        }
    }

    void Tween::setDoneCallback(const EventCallback& cb)
    {
        _cbDone = cb;
    }

    void Tween::addDoneCallback(const EventCallback& cb)
    {
        addEventListener(TweenEvent::DONE, cb);
    }



    float Tween::_calcEase(float v)
    {
        if (_twoSides)
        {
            if (v > 0.5f)
                v = 1.0f - v;
            v *= 2.0f;
        }

        v = calcEase(_ease, v);
        return v;
    }

    void Tween::setElapsed(const timeMS &elapsed)
    {
        _elapsed = elapsed;
    }

    void Tween::remove()
    {
        getClient()->removeTween(this);
    }

    void Tween::complete(timeMS deltaTime)
    {
        if (_loops == -1)
            return;

        //if already done
        if (_status >= status_done)
            return;

        //Q_ASSERT(_client);

        if (!_client)
            return;

        //Q_ASSERT(!"not implemented");

        //not started yet because has delay
        if (_status == status_delayed)
        {
            _start(*_client);
            _status = status_started;
        }


        Q_ASSERT(_status == status_started);
        //while (_status != status_remove)
        {
            UpdateState us;
            us.dt = deltaTime;

            update(*_client, us);
        }

        Q_ASSERT(_status == status_done);

        //_client->removeTween(this);
    }

    void Tween::start(Actor& actor)
    {
        _client = &actor;
        _status = status_delayed;
        if (_delay == timeMS(0))
        {
            _status = status_started;
            _start(actor);
        }
    }

    void Tween::update(Actor& actor, const UpdateState& us)
    {
        _elapsed += us.dt;
        switch (_status)
        {
            case status_delayed:
            {
                if (_elapsed >= _delay)
                {
                    _status = status_started;
                    _start(*_client);
                }
            }
                break;
            case status_started:
            {
                if (_duration > timeMS(0))
                {
                    timeMS localElapsed = _elapsed - _delay;

                    if (_globalEase != ease_linear)
                    {
                        float p = localElapsed.count() / float(_duration.count() * _loops);
                        timeMS nv = timeMS(static_cast<qint64>(calcEase(_globalEase, std::min(p, 1.0f)) * _duration.count() * _loops));
                        localElapsed = nv;
                    }

                    int loopsDone = localElapsed / _duration;
                    _percent = _calcEase(((float)(localElapsed.count() - loopsDone * _duration.count())) / _duration.count());

                    while(_loopsDone < loopsDone)
                    {
                        _loopDone(actor, us);
                        _loopsDone++;
                    }

                    if (_loops > 0 && int(loopsDone) >= _loops)
                    {
                        if (_twoSides)
                            _percent = 0;
                        else
                            _percent = 1;

                        if (!_disabledStatusDone)
                            _status = status_done;
                    }
                }
                _update(*_client, us);
            }
                break;
            case status_done:
            {
                done(*_client, us);
            }
                break;
            default:
                break;
        }
    }

    void Tween::done(Actor& actor, const UpdateState& us)
    {
        _done(actor, us);

        if (_detach)
        {
            actor.detach();
        }

        TweenEvent ev(this, &us);
        ev.target = ev.currentTarget = &actor;
        ev.tween = this;

        if (_cbDone.isSet())
        {
            _cbDone(&ev);
        }

        dispatchEvent(&ev);

        _status = status_remove;
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
                //Q_ASSERT(!"unsupported ease");
                break;
        }
        return t;
    }

    void  Tween::setCustomEaseHandler(easeHandler h)
    {
        _customEaseHandler = h;
    }
}
