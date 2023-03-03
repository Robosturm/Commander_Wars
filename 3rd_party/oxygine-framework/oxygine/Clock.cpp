#include "3rd_party/oxygine-framework/oxygine/Clock.h"

namespace oxygine
{
    void Clock::pause()
    {
        m_pauseCounter += 1;
    }

    void Clock::resume()
    {
        m_pauseCounter -= 1;
    }
    void Clock::resetPause()
    {
        m_pauseCounter = 0;
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
        double dt = (time - m_lastUpdateTime).count();
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
        if (m_pauseCounter > 0)
        {
            dt = 0;
        }
        m_destTime += dt;

        m_lastUpdateTime = time;
        m_lastDT = static_cast<int>(dt);
    }

    timeMS Clock::doTick()
    {
        if (m_pauseCounter > 0)
        {
            return timeMS(0);
        }
        if (m_srcTime > m_destTime)
        {
            return timeMS(0);
        }
        timeMS dt = timeMS(static_cast<qint64>(m_destTime - m_srcTime));
        m_srcTime = m_destTime;
        return dt;
    }

    timeMS Clock::getTime() const
    {
        return timeMS(static_cast<qint64>(m_srcTime));
    }
}
