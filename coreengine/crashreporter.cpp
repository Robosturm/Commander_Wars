#include "coreengine/crashreporter.h"
#include "coreengine/console.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <QTextStream>
#include <QStandardPaths>

QString CrashReporter::sCrashReportDirPath;
QString CrashReporter::sProgramName;
CrashReporter::logWrittenCallback CrashReporter::sLogWrittenCallback = nullptr;
QProcess* CrashReporter::sProcess = nullptr;

void CrashReporter::_writeLog( const QString &inSignal)
{
    const QStringList cReportHeader
    {
        QStringLiteral("%1 v%2 Builddate: %3 %4").arg(QCoreApplication::applicationName(), QCoreApplication::applicationVersion(), Console::compileDate,  Console::compileTime ),
                QDateTime::currentDateTime().toString("dd MMM yyyy @ HH:mm:ss"),
                QString(),
                inSignal,
                QString(),
    };

    QDir().mkpath( sCrashReportDirPath );

    const QString  cFileName = QStringLiteral("%1/%2 %3 Crash.log").arg(sCrashReportDirPath,
                                                                        QDateTime::currentDateTime().toString( "yyyyMMdd-HHmmss" ),
                                                                        QCoreApplication::applicationName() );
    QFile file(cFileName);
    QString ret;

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream( &file );
        QStringList dataList = cReportHeader;
        for (const QString & data : qAsConst(dataList))
        {
            stream << data << Qt::endl;
            ret += data + "\n";
            stream.flush();
        }
        stream.flush();
        stream << "\nConsole log\n";
        stream.flush();
        std::vector<QString> consoleOutput = Console::getInstance()->getConsoleLog();
        for (auto & item : consoleOutput)
        {
            stream << item << Qt::endl;
            ret += item + "\n";
            stream.flush();
        }

        file.close();
    }

    if ( sLogWrittenCallback != nullptr )
    {
        (*sLogWrittenCallback)(ret);
    }
}

void CrashReporter::setSignalHandler(logWrittenCallback inLogWrittenCallback )
{
    sProgramName = QCoreApplication::arguments().at(0);
    sCrashReportDirPath = QCoreApplication::applicationDirPath() + "/logs";
    sLogWrittenCallback = inLogWrittenCallback;
    if ( sProcess == nullptr )
    {
        sProcess = new QProcess();
#ifdef GRAPHICSUPPORT
        sProcess->setObjectName("CrashreporterProcess");
#endif
        sProcess->setProcessChannelMode( QProcess::MergedChannels );
    }
    setOsSignalHandler();
}
