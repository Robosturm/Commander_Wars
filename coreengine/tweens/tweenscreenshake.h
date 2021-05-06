#ifndef TWEENSCREENSHAKE_H
#define TWEENSCREENSHAKE_H

#include <QPoint>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class TweenScreenshake
{
public:
    using type = oxygine::Actor;
    TweenScreenshake(qint32 startIntensity, float decay, oxygine::timeMS timeMs);

     void update(oxygine::Actor& actor, float p, const oxygine::UpdateState& us);

    void init(oxygine::Actor& actor);
    void done(oxygine::Actor& actor);
private:
    QPoint m_position;
    qint32 m_intensity;
    float m_decay;
    oxygine::timeMS m_timeMs;
    oxygine::timeMS m_lastUpdate{ 0};
    float m_lastDegree{-1};
};

#endif // TWEENSCREENSHAKE_H
