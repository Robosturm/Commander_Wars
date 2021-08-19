#ifndef CRASHREPORTER_H
#define CRASHREPORTER_H

#include <QString>

class QProcess;

class CrashReporter
{
public:
    /// Function signature for a callback after the log is written.
    /// @param log
    using logWrittenCallback = void (*)(QString log);
    ///! Set a signal handler to capture stack trace to a log file.
    ///
    /// @param inCrashReportDirPath Path to directory to write our crash report to.
    ///   If not set, it will use Desktop/<App Name> Crash Logs/
    /// @param inLogWrittenCallback A function to call after we've written the log file.
    ///   You might use this to display a message to the user about where to find the log for example.
    static void setSignalHandler(logWrittenCallback inLogWrittenCallback = nullptr );
    static void _writeLog( const QString &inSignal);
private:
    static void setOsSignalHandler();
private:
    static QString sCrashReportDirPath;             // log file path
    static QString sProgramName;                    // the full path to the executable (which we need to resolve symbols)
    static logWrittenCallback sLogWrittenCallback;  // function to call after we've written the log file
    static QProcess* sProcess ;                     // process used to capture output of address mapping tool
};

#endif // CRASHREPORTER_H
