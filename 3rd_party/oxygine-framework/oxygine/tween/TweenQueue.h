#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"

namespace oxygine
{
    class TweenQueue;
    using spTweenQueue = intrusive_ptr<TweenQueue>;
    class TweenQueue : public Tween
    {
    public:

        enum
        {
            EVENT_LOOP_BEGIN = sysEventID('T', 'L', 'B'),
            EVENT_LOOP_END = sysEventID('T', 'L', 'E'),
        };
        explicit TweenQueue() = default;
        virtual ~TweenQueue() = default;
        /*Add tween to Queue, returns added tween*/
        spTween add(spTween t);
        virtual void complete(timeMS deltaTime) override;
    private:
        virtual void _start(Actor& actor) override;
        virtual void _update(Actor& actor, const UpdateState& us) override;

    private:
        using tweens = QVector<spTween>;
        tweens m_tweens;
        spTween m_current;
        qint32 m_loopsDone{0};
    };
}
