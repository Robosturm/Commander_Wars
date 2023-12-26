#include "coreengine/crashreporter.h"
#include <execinfo.h>
#include <signal.h>
#include <QStringList>

void crashhandler(int sig)
{
    void *array[CrashReporter::STACKSIZE];
    size_t size = backtrace(array, CrashReporter::STACKSIZE);
    auto messages = backtrace_symbols(array, size);
    QStringList frameList;
    for (qint32 i = 0; i < size; ++i)
    {
        frameList.append(messages[i]);
    }
    CrashReporter::_writeLog(QString("Error: signal ") + QString::number(sig) + " received", frameList);
}

void CrashReporter::setOsSignalHandler()
{
    signal(SIGSEGV, crashhandler);
}
