#include "coreengine/tweens/tweenscreenshake.h"
#include "coreengine/globalutils.h"
#include <QtMath>

TweenScreenshake::TweenScreenshake(qint32 startIntensity, float decay, oxygine::timeMS timeMs)
    : m_intensity(startIntensity),
      m_decay(decay),
      m_timeMs(timeMs)
{
}

void TweenScreenshake::update(oxygine::Actor& actor, float, const oxygine::UpdateState& us)
{
    if (m_lastUpdate == oxygine::timeMS(0) || (us.time - m_lastUpdate) >= m_timeMs)
    {
        m_lastDegree = m_lastDegree + 180 + GlobalUtils::randIntBase(-45, 45);
        float x = qCos(m_lastDegree / 360.0f * 2.0f * M_PI) * m_intensity;
        float y = qSin(m_lastDegree / 360.0f * 2.0f * M_PI) * m_intensity;
        actor.setPosition(m_position.x() + x, m_position.y() + y);
        m_intensity *= m_decay;
        m_lastUpdate = us.time;
    }
}

void TweenScreenshake::init(oxygine::Actor& actor)
{
    m_position = QPoint(actor.getX(), actor.getY());
}

void TweenScreenshake::done(oxygine::Actor& actor)
{
    actor.setPosition(m_position.x(), m_position.y());
}
