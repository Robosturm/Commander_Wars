#include "coreengine/crashreporter.h"
#include "coreengine/gameconsole.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <QTextStream>
#include <QStandardPaths>

spCrashReporter CrashReporter::m_instance;

void CrashReporter::_writeLog( const QString &inSignal)
{
    const QStringList cReportHeader
    {
        QStringLiteral("%1 v%2 Builddate: %3 %4").arg(QCoreApplication::applicationName(), QCoreApplication::applicationVersion(), GameConsole::compileDate,  GameConsole::compileTime ),
                QDateTime::currentDateTime().toString("dd MMM yyyy @ HH:mm:ss"),
                QString(),
                inSignal,
                QString(),
    };

    QDir().mkpath(m_instance->sCrashReportDirPath);

    const QString  cFileName = QStringLiteral("%1/%2 %3 Crash.log").arg(m_instance->sCrashReportDirPath,
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
        std::vector<QString> consoleOutput = GameConsole::getInstance()->getConsoleLog();
        for (auto & item : consoleOutput)
        {
            stream << item << Qt::endl;
            ret += item + "\n";
            stream.flush();
        }

        file.close();
    }

    if (m_instance->sLogWrittenCallback != nullptr )
    {
        (*m_instance->sLogWrittenCallback)(ret);
    }
}

void CrashReporter::setSignalHandler(logWrittenCallback inLogWrittenCallback )
{
    m_instance = MemoryManagement::create<CrashReporter>();
    m_instance->sProgramName = QCoreApplication::arguments().at(0);
    m_instance->sCrashReportDirPath = QCoreApplication::applicationDirPath() + "/logs";
    m_instance->sLogWrittenCallback = inLogWrittenCallback;
    if (m_instance->sProcess == nullptr)
    {
        m_instance->sProcess = MemoryManagement::createNamedQObject<QProcess>("QProcess");
#ifdef GRAPHICSUPPORT
        m_instance->sProcess->setObjectName("CrashreporterProcess");
#endif
        m_instance->sProcess->setProcessChannelMode( QProcess::MergedChannels );
    }
    setOsSignalHandler();
}
