#include "3rd_party/oxygine-framework/oxygine/Clock.h"

namespace oxygine
{

    Clock::Clock():
        m_counter(0), m_destTime(0), m_srcTime(0),
        m_multiplier(1.0f), m_fixedStep(0),
        m_lastDT(0),
        m_lastUpdateTime(-1)
    {
    }

    Clock::~Clock()
    {

    }

    float Clock::getMultiplier() const
    {
        return m_multiplier;
    }

    qint32 Clock::getFixedStep() const
    {
        return (int)m_fixedStep;
    }

    qint32 Clock::getLastDT() const
    {
        return m_lastDT;
    }

    timeMS  Clock::getLastUpdateTime() const
    {
        return m_lastUpdateTime;
    }

    void Clock::setMultiplier(float m)
    {
        m_multiplier = m;
    }

    void Clock::setFixedStep(float step)
    {
        m_fixedStep = step;
    }

    void Clock::pause()
    {
        m_counter += 1;
    }

    void Clock::resume()
    {
        m_counter -= 1;
    }
    void Clock::resetPause()
    {
        m_counter = 0;
    }

    void Clock::update(timeMS globalTime)
    {
        timeMS time = globalTime;
        const timeMS neg =  std::chrono::milliseconds(-1);
        if (time <= neg)
        {
            time = Clock::getTimeMS();
        }
        if (m_lastUpdateTime <= neg)
        {
            m_lastUpdateTime = time;
        }
        double dt = (time - m_lastUpdateTime).count() * m_multiplier;
        if (dt < 1 && dt > 0)
        {
            dt = 1;
        }
        if (dt > 100)
        {
            dt = 100;
        }
        if (dt < 0)
        {
            dt = 1;
        }
        if (m_counter > 0)
        {
            dt = 0;
        }
        m_destTime += dt;

        m_lastUpdateTime = time;
        m_lastDT = static_cast<int>(dt);
    }

    timeMS Clock::doTick()
    {
        if (m_counter > 0)
        {
            return timeMS(0);
        }
        if (m_srcTime + m_fixedStep > m_destTime)
        {
            return timeMS(0);
        }
        if (m_fixedStep == 0)
        {
            timeMS dt = timeMS(static_cast<qint64>(m_destTime - m_srcTime));
            m_srcTime = m_destTime;
            return dt;
        }

        m_srcTime += m_fixedStep;
        return timeMS(static_cast<qint64>(m_fixedStep));
    }

    timeMS Clock::getTime() const
    {
        return timeMS(static_cast<qint64>(m_srcTime));
    }

    qint32 Clock::getPauseCounter() const
    {
        return m_counter;
    }
}
