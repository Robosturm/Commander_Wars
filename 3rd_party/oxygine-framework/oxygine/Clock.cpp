#include "3rd_party/oxygine-framework/oxygine/Clock.h"

namespace oxygine
{

    Clock::Clock():
        _counter(0), _destTime(0), _srcTime(0),
        _multiplier(1.0f), _fixedStep(0),
        _lastDT(0),
        _lastUpdateTime(-1)
    {
    }

    Clock::~Clock()
    {

    }

    float Clock::getMultiplier() const
    {
        return _multiplier;
    }

    int Clock::getFixedStep() const
    {
        return (int)_fixedStep;
    }

    int Clock::getLastDT() const
    {
        return _lastDT;
    }

    timeMS  Clock::getLastUpdateTime() const
    {
        return _lastUpdateTime;
    }

    void Clock::setMultiplier(float m)
    {
        _multiplier = m;
    }

    void Clock::setFixedStep(float step)
    {
        _fixedStep = step;
    }

    void Clock::pause()
    {
        _counter += 1;
    }

    void Clock::resume()
    {
        _counter -= 1;
    }
    void Clock::resetPause()
    {
        _counter = 0;
    }

    void Clock::update(timeMS globalTime)
    {
        timeMS time = globalTime;
        const timeMS neg =  std::chrono::milliseconds(-1);
        if (time <= neg)
        {
            time = Clock::getTimeMS();
        }
        if (_lastUpdateTime <= neg)
        {
            _lastUpdateTime = time;
        }
        double dt = (time - _lastUpdateTime).count() * _multiplier;
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
        if (_counter > 0)
        {
            dt = 0;
        }
        _destTime += dt;

        _lastUpdateTime = time;
        _lastDT = static_cast<int>(dt);
    }

    timeMS Clock::doTick()
    {
        if (_counter > 0)
        {
            return timeMS(0);
        }
        if (_srcTime + _fixedStep > _destTime)
        {
            return timeMS(0);
        }
        if (_fixedStep == 0)
        {
            timeMS dt = timeMS(static_cast<qint64>(_destTime - _srcTime));
            _srcTime = _destTime;
            return dt;
        }

        _srcTime += _fixedStep;
        return timeMS(static_cast<qint64>(_fixedStep));
    }

    timeMS Clock::getTime() const
    {
        return timeMS(static_cast<qint64>(_srcTime));
    }

    int Clock::getPauseCounter() const
    {
        return _counter;
    }
}
