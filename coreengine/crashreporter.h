#ifndef CRASHREPORTER_H
#define CRASHREPORTER_H

#include <QString>

namespace crashReporter
{
    /// Function signature for a callback after the log is written.
    /// @param log
    using logWrittenCallback = void (*)(QString log);

    ///! Set a signal handler to capture stack trace to a log file.
    ///
    /// @param inCrashReportDirPath Path to directory to write our crash report to.
    ///   If not set, it will use Desktop/<App Name> Crash Logs/
    /// @param inLogWrittenCallback A function to call after we've written the log file.
    ///   You might use this to display a message to the user about where to find the log for example.
    void  setSignalHandler(logWrittenCallback inLogWrittenCallback = nullptr );

}

#endif // CRASHREPORTER_H
