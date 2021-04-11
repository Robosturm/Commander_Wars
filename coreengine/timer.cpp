#include "coreengine/timer.h"

Timer::Timer()
{
    setObjectName("Timer");
}

void Timer::pause()
{
    qint32 remaining  = remainingTime();
    stop();
    setInterval(remaining);
}

void Timer::resume()
{
    start();
}
