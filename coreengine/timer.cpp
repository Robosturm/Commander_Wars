#include "coreengine/timer.h"
#include "coreengine/interpreter.h"

Timer::Timer(QObject* pParent)
    : QTimer(pParent)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Timer");
#endif
    Interpreter::setCppOwnerShip(this);
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
