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
        int     getPauseCounter() const;
        int     getFixedStep() const;
        int     getLastDT() const;
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
        int     _counter;
        double  _destTime;
        double  _srcTime;

        float   _multiplier;
        float   _fixedStep;

        int    _lastDT;
        timeMS _lastUpdateTime;


    };
}
