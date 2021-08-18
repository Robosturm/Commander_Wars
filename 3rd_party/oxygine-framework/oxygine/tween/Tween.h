#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Property.h"
#include "3rd_party/oxygine-framework/oxygine/UpdateState.h"
#include "3rd_party/oxygine-framework/oxygine/core/closure.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include <limits>

namespace oxygine
{
    class Tween;
    using spTween = intrusive_ptr<Tween>;

    class TweenEvent : public Event
    {
    public:
        enum
        {
            START = sysEventID('S', 'T', 'A'),
            DONE = Event::COMPLETE,
        };

        TweenEvent(Tween* tween_, const UpdateState* us_)
            : Event(DONE, false),
              m_tween(tween_),
              m_us(us_)
        {
        }
        Actor* getActor() const;
        Tween* m_tween{nullptr};
        const UpdateState* m_us{nullptr};
    };

    class Tween : public EventDispatcher
    {
    public:
        static constexpr qint32 TWEEN_COMPLETE_DT = std::numeric_limits<int>::max() / 2;
        enum EASE
        {
            ease_unknown,
            ease_linear,

            ease_inQuad,
            ease_outQuad,
            ease_inOutQuad,
            ease_outInQuad,

            ease_inCubic,
            ease_outCubic,
            ease_inOutCubic,
            ease_outInCubic,

            ease_inQuart,
            ease_outQuart,
            ease_inOutQuart,
            ease_outInQuart,

            ease_inQuint,
            ease_outQuint,
            ease_inOutQuint,
            ease_outInQuint,

            ease_inSin,
            ease_outSin,
            ease_inOutSin,
            ease_outInSin,

            ease_inExpo,
            ease_outExpo,
            ease_inOutExpo,
            ease_outInExpo,

            ease_inCirc,
            ease_outCirc,
            ease_inOutCirc,
            ease_outInCirc,

            ease_inBack,
            ease_outBack,
            ease_inOutBack,
            ease_outInBack,

            ease_inBounce,
            ease_outBounce,
            ease_inOutBounce,
            ease_outInBounce,

            ease_count
        };

        explicit Tween() = default;
        virtual ~Tween() = default;
        void init(timeMS duration, qint32 loops = 1, bool twoSides = false, timeMS delay = timeMS(0), EASE ease = Tween::ease_linear);
        void init2(const TweenOptions& opt);
        void reset();
        qint32 getLoops() const
        {
            return m_loops;
        }
        timeMS getDuration() const
        {
            return m_duration;
        }
        void setElapsed(const timeMS &elapsed);
        timeMS getElapsed() const
        {
            return m_elapsed;
        }
        EASE getEase() const
        {
            return m_ease;
        }
        EASE getGlobalEase() const
        {
            return m_globalEase;
        }
        timeMS getDelay() const
        {
            return m_delay;
        }
        Actor* getClient() const
        {
            return m_client;
        }
        float getPercent() const
        {
            return m_percent;
        }
        const EventCallback& getDoneCallback() const
        {
            return m_cbDone;
        }

        bool isStarted() const
        {
            return m_status != status_not_started;
        }
        bool isDone() const
        {
            return m_status == status_remove;
        }
        /**add callback would be called when tween done.  Could be added more than one.
        setDoneCallback is faster because it doesn't allocate memory for list internally*/
        void addDoneCallback(const EventCallback& cb);
        /**set Easing function*/
        void setEase(EASE ease)
        {
            m_ease = ease;
        }
        /**set Global Easing function */
        void setGlobalEase(EASE ease)
        {
            m_globalEase = ease;
        }
        /**set Delay before starting tween*/
        void setDelay(timeMS delay)
        {
            m_delay = delay;
        }
        /** loops = -1 means infinity repeat cycles*/
        void setLoops(qint32 loops)
        {
            m_loops = loops;
        }
        /*set Duration of tween**/
        void setDuration(timeMS duration)
        {
            m_duration = duration;
        }
        void setClient(Actor* client)
        {
            m_client = client;
        }
        void setTwoSides(bool ts)
        {
            m_twoSides = ts;
        }
        /** remove actor from parent node when tween done*/
        void detachWhenDone(bool detach = true)
        {
            m_detach = detach;
        }
        /**immediately completes tween, calls doneCallback and mark tween as completed and removes self from Actor. If tween has infinity loops (=-1) then do nothing*/
        virtual void complete(timeMS deltaTime = timeMS(TWEEN_COMPLETE_DT));

        /**removes self from Actor, complete callback not called*/
        void removeFromActor();
        void start(Actor& actor);
        void update(Actor& actor, const UpdateState& us);
        static float calcEase(EASE ease, float v);
        /**set callback when tween done. Doesn't allocate memory. faster than addDoneCallback*/
        void setDoneCallback(const EventCallback& cb);
        /**set callback when tween done. Doesn't allocate memory. faster than addDoneCallback*/
        void setStartCallback(const EventCallback& cb);
    private:
        void __start(Actor& actor, const UpdateState& us);

    protected:
        void done(Actor&, const UpdateState& us);

        virtual void _start(Actor&) {}
        virtual void _update(Actor&, const UpdateState&) {}
        virtual void _done(Actor&, const UpdateState&) {}
        virtual void _loopDone(Actor&, const UpdateState&) {}
        virtual float _calcEase(float v);

        enum status
        {
            status_not_started,
            status_delayed,
            status_started,
            status_done,
            status_remove,
        };

    protected:
        status m_status{status_not_started};
        timeMS m_elapsed{0};
        timeMS m_duration{0};
        timeMS m_delay{0};
        qint32 m_loops{1};
        qint32 m_loopsDone{0};
        EASE m_ease{ease_linear};
        EASE m_globalEase{ease_linear};
        bool m_twoSides{false};
        float m_percent{0};
        bool m_detach{false};
        EventCallback m_cbStart;
        EventCallback m_cbDone;
        Actor* m_client{nullptr};
    };

    template<class TProperty>
    class TweenT;
    template<class TProperty>
    using spTweenT = oxygine::intrusive_ptr<TweenT<TProperty>>;

    template<class TProperty>
    class TweenT : public Tween
    {
    public:
        using TActor = typename TProperty::TActor;
        explicit TweenT(const TProperty& property)
            : m_property(property)
        {
        }
        virtual ~TweenT() = default;

        virtual void _update(Actor& actor, const UpdateState& us) override
        {
            TActor& rActor = *oxygine::safeCast<TActor*>(&actor);
            m_property.update(rActor, m_percent, us);
        }

        virtual void _start(Actor& actor) override
        {
            TActor& rActor = *oxygine::safeCast<TActor*>(&actor);
            m_property.init(rActor);
            UpdateState us;
            us.iteration = -1;
            m_property.update(rActor, _calcEase(0.0f), us);
        }

        virtual void _done(Actor& actor, const UpdateState&) override
        {
            TActor& rActor = *oxygine::safeCast<TActor*>(&actor);
            m_property.done(rActor);
        }
    private:
        TProperty m_property;
    };

    template <typename TProperty>
    spTweenT<TProperty> createTween(const TProperty& property, timeMS duration, qint32 loops = 1,
                                    bool twoSides = false, timeMS delay = oxygine::timeMS(0), Tween::EASE ease = Tween::ease_linear)
    {
        spTweenT<TProperty> pTween = spTweenT<TProperty>::create(property);
        pTween->init(duration, loops, twoSides, delay, ease);
        return pTween;
    }

    class TweenBase
    {
    public:
        using TActor = oxygine::Actor;
        virtual void init(Actor&) {}
        virtual void done(Actor&) {}
        virtual void update(Actor&, float, const UpdateState&) {}
    };

    template<class TTween>
    class TweenProxy
    {
    public:
        TweenProxy(intrusive_ptr<TTween> object)
        {
            m_obj = object;
        }
        void init(Actor& actor)
        {
            m_obj->init(actor);
        }
        void done(Actor& actor)
        {
            m_obj->done(actor);
        }
        void update(Actor& actor, float p, const UpdateState& us)
        {
            m_obj->update(actor, p, us);
        }
        intrusive_ptr<TTween> m_obj;
    };
}
