#include "coreengine/crashreporter.h"
#include <execinfo.h>
#include <signal.h>

void crashhandler(int sig)
{
    static qint32 constexpr stacksize = 30;
    void *array[stacksize];
    size_t size = backtrace(array, stacksize);
    auto message = backtrace_symbols(array, size);
    CrashReporter::_writeLog(QString("Error: signal ") + QString::number(sig) + " received");
    CrashReporter::_writeLog(message);
}

void CrashReporter::setOsSignalHandler()
{
    signal(SIGSEGV, handler);
}
