#ifndef CRASHREPORTER_H
#define CRASHREPORTER_H

#include <QString>

class QProcess;
class CrashReporter;
using spCrashReporter = std::shared_ptr<CrashReporter>;
using spQProcess = std::shared_ptr<QProcess>;

class CrashReporter final
{
public:
    static constexpr const char* const getTypeName()
    {
        return "CrashReporter";
    }
    ~CrashReporter() = default;
    /// Function signature for a callback after the log is written.
    /// @param log
    using logWrittenCallback = void (*)(const QString & log);
    ///! Set a signal handler to capture stack trace to a log file.
    ///
    /// @param inCrashReportDirPath Path to directory to write our crash report to.
    ///   If not set, it will use Desktop/<App Name> Crash Logs/
    /// @param inLogWrittenCallback A function to call after we've written the log file.
    ///   You might use this to display a message to the user about where to find the log for example.
    static void setSignalHandler(logWrittenCallback inLogWrittenCallback = nullptr );
    static void _writeLog( const QString &inSignal);
private:
    friend class MemoryManagement;
    static void setOsSignalHandler();
    CrashReporter() = default;
private:
    static spCrashReporter m_instance;
    QString sCrashReportDirPath;             // log file path
    QString sProgramName;                    // the full path to the executable (which we need to resolve symbols)
    logWrittenCallback sLogWrittenCallback{nullptr};  // function to call after we've written the log file
    spQProcess sProcess;               // process used to capture output of address mapping tool
};

#endif // CRASHREPORTER_H
