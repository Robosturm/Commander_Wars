#ifndef TWEENSHAKEY_H
#define TWEENSHAKEY_H

#include <QPoint>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

class TweenShakeY final
{
public:
    using TActor = oxygine::Actor;
    explicit TweenShakeY(qint32 startIntensity, float start, float end);
   virtual ~TweenShakeY() = default;
    void update(oxygine::Actor& actor, float percentDone, const oxygine::UpdateState& us);

    void init(oxygine::Actor& actor);
    void done(oxygine::Actor& actor);
private:
    float m_startY;
    qint32 m_intensity;
    oxygine::timeMS m_timeMs;
    oxygine::timeMS m_lastUpdate{0};
    float m_startFraction{0};
    float m_endFracrtion{0};
};

#endif // TWEENSHAKEY_H
