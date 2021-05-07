#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include <chrono>

namespace oxygine
{
    DECLARE_SMART(Clock, spClock);
    class Clock: public Object
    {
    public:
        Clock();
        ~Clock();

        timeMS  getTime() const;
        qint32     getPauseCounter() const;
        qint32     getFixedStep() const;
        qint32     getLastDT() const;
        timeMS  getLastUpdateTime() const;
        float   getMultiplier() const;

        void    setFixedStep(float stepMS);
        void    setMultiplier(float m);

        void    pause();
        void    resume();
        void    resetPause();

        void    update(timeMS globalTime = timeMS(-1));
        timeMS  doTick();

        static inline timeMS getTimeMS()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::system_clock::now().time_since_epoch()));
        }
    private:
        qint32  m_counter;
        double  m_destTime;
        double  m_srcTime;
        float   m_multiplier;
        float   m_fixedStep;
        qint32  m_lastDT;
        timeMS  m_lastUpdateTime;
    };
}
