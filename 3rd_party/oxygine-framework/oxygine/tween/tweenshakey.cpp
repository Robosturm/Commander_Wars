#include "3rd_party/oxygine-framework/oxygine/tween/tweenshakey.h"
#include <QtMath>

TweenShakeY::TweenShakeY(qint32 startIntensity, float start, float end)
    : m_intensity(startIntensity),
      m_startFraction(start),
      m_endFracrtion(end)
{
}

void TweenShakeY::update(oxygine::Actor& actor, float percentDone, const oxygine::UpdateState& us)
{
    float y = 0;
    auto div = m_startFraction - m_endFracrtion;
    if (percentDone < m_endFracrtion)
    {
        y = 0;
    }
    else if (percentDone > m_startFraction)
    {
        y = m_intensity;
    }
    else if (m_startFraction > m_endFracrtion && div > 0)
    {
        y = m_intensity * (percentDone - m_endFracrtion) / (m_startFraction - m_endFracrtion);
    }

    actor.setY(m_startY + y);
}

void TweenShakeY::init(oxygine::Actor& actor)
{
    m_startY =  actor.getY();
}

void TweenShakeY::done(oxygine::Actor& actor)
{
    actor.setY(m_startY);
}
