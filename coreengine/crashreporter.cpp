#include "coreengine/crashreporter.h"
#include "coreengine/console.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <QTextStream>
#include <QStandardPaths>


#ifdef Q_OS_WIN
#include <windows.h>
#include <cxxabi.h>
#include <imagehlp.h>
#elif defined(Q_OS_ANDROID)
// no crash reporter for android
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
#include <csignal>
#include <err.h>
#include <execinfo.h>
#else
// no crash reporting for other os right now
#endif

namespace crashReporter
{
    QString sCrashReportDirPath; // log file path
    QString sProgramName;        // the full path to the executable (which we need to resolve symbols)
    // Note that we are looking for GCC-style name mangles
    // See: https://en.wikipedia.org/wiki/Name_mangling #How_different_compilers_mangle_the_same_functions
    QRegularExpression  sSymbolMatching("^.*(_Z[^ ]+).*$");

    static logWrittenCallback  sLogWrittenCallback; // function to call after we've written the log file
    static QProcess            *sProcess = nullptr; // process used to capture output of address mapping tool

    // Resolve symbol name & source location
    QString _addressToLine( const QString &inProgramName, void const * const inAddr )
    {
        if (sProcess->state() == QProcess::NotRunning)
        {
            const QString  cAddrStr = QStringLiteral("0x%1").arg(quintptr(inAddr), 16, 16, QChar('0'));
#ifdef Q_OS_MAC
            // Uses atos
            const QString  cProgram = QStringLiteral( "atos" );

            const QStringList  cArguments = {
                "-o", inProgramName,
                "-arch", "x86_64",
                cAddrStr
            };
#else
            // Uses addr2line
            const QString  cProgram = QStringLiteral("%1/addr2line").arg(QCoreApplication::applicationDirPath());
            const QStringList  cArguments =
            {
                "-f",
                "-p",
                "-s",
                "-e", inProgramName,
                cAddrStr
            };
#endif
            sProcess->setProgram(cProgram);
            sProcess->setArguments(cArguments);
            sProcess->start(QIODevice::ReadOnly);
            if (!sProcess->waitForFinished())
            {
                return QStringLiteral( "* Error running command\n   %1 %2\n   %3" ).arg(
                            sProcess->program(),
                            sProcess->arguments().join( ' ' ),
                            sProcess->errorString() );
            }
            const QString  cLocationStr = QString( sProcess->readAll() ).trimmed();
            if (cLocationStr == cAddrStr)
            {
                return cAddrStr;
            }
            else
            {
                return cLocationStr;
            }
        }
        else
        {
            return "Crash reporting already running.\n Critical error caused another thread to crash. Only one crash can be gathered and reported";
        }
    }

    void  _writeLog( const QString &inSignal, const QStringList &inFrameInfoList )
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
            QStringList dataList = cReportHeader + inFrameInfoList;
            for (const QString & data : qAsConst(dataList))
            {
                stream << data << Qt::endl;
                ret += data + "\n";
                stream.flush();
            }
            stream.flush();
            stream << "\nConsole log\n";
            stream.flush();
            QList<QString> consoleOutput = Console::getInstance()->getConsoleLog();
            for (qint32 i = 0; i < consoleOutput.size(); i++)
            {
                stream << consoleOutput[i] << Qt::endl;
                ret += consoleOutput[i] + "\n";
                stream.flush();
            }

            file.close();
        }

        if ( sLogWrittenCallback != nullptr )
        {
            (*sLogWrittenCallback)(ret);
        }
    }
#ifdef Q_OS_WIN
    QStringList _stackTrace( CONTEXT* context )
    {
        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();

        SymInitialize(process, nullptr, true);

        STACKFRAME64 stackFrame;
        memset(&stackFrame, 0, sizeof(STACKFRAME64));

        DWORD   image;

#ifdef _M_IX86
        image = IMAGE_FILE_MACHINE_I386;

        stackFrame.AddrPC.Offset = context->Eip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context->Esp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context->Ebp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
#elif _M_X64
        image = IMAGE_FILE_MACHINE_AMD64;

        stackFrame.AddrPC.Offset = context->Rip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context->Rsp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context->Rsp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#else
        // see http://theorangeduck.com/page/printing-stack-trace-mingw
#error You need to define the stack frame layout for this architecture
#endif
        QStringList frameList;
        try
        {
            qint32         frameNumber = 0;
            while (StackWalk64(image, process, thread,
                               &stackFrame, context, nullptr,
                               SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
            {
                QString  locationStr = _addressToLine(sProgramName, reinterpret_cast<void*>(stackFrame.AddrPC.Offset));
                // match the mangled name and demangle if we can
                QRegularExpressionMatch match = sSymbolMatching.match(locationStr);
                const QString cSymbol(match.captured(1));

                if (!cSymbol.isNull())
                {
                    qint32 demangleStatus = 0;
                    const char *cFunctionName = abi::__cxa_demangle( cSymbol.toLatin1().constData(), nullptr, nullptr, &demangleStatus);
                    if ( demangleStatus == 0 )
                    {
                        locationStr.replace(cSymbol, cFunctionName);
                    }
                }
                QString line = QStringLiteral("[%1] 0x%2 %3")
                               .arg(QString::number(frameNumber))
                               .arg(quintptr(reinterpret_cast<void*>(stackFrame.AddrPC.Offset)), 16, 16, QChar('0'))
                               .arg(locationStr);
                frameList += line;

                ++frameNumber;
                // anything above isn't really helpful
                if (frameNumber > 10)
                {
                    break;
                }
            }
        } catch (...)
        {

        }
        SymCleanup(GetCurrentProcess());
        return frameList;
    }

    LONG WINAPI _winExceptionHandler( EXCEPTION_POINTERS *inExceptionInfo )
    {
        const QString  cExceptionType = [] ( DWORD code )
        {
            switch( code )
            {
                case EXCEPTION_ACCESS_VIOLATION:
                    return QStringLiteral( "EXCEPTION_ACCESS_VIOLATION" );
                case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
                    return QStringLiteral( "EXCEPTION_ARRAY_BOUNDS_EXCEEDED" );
                case EXCEPTION_BREAKPOINT:
                    return QStringLiteral( "EXCEPTION_BREAKPOINT" );
                case EXCEPTION_DATATYPE_MISALIGNMENT:
                    return QStringLiteral( "EXCEPTION_DATATYPE_MISALIGNMENT" );
                case EXCEPTION_FLT_DENORMAL_OPERAND:
                    return QStringLiteral( "EXCEPTION_FLT_DENORMAL_OPERAND" );
                case EXCEPTION_FLT_DIVIDE_BY_ZERO:
                    return QStringLiteral( "EXCEPTION_FLT_DIVIDE_BY_ZERO" );
                case EXCEPTION_FLT_INEXACT_RESULT:
                    return QStringLiteral( "EXCEPTION_FLT_INEXACT_RESULT" );
                case EXCEPTION_FLT_INVALID_OPERATION:
                    return QStringLiteral( "EXCEPTION_FLT_INVALID_OPERATION" );
                case EXCEPTION_FLT_OVERFLOW:
                    return QStringLiteral( "EXCEPTION_FLT_OVERFLOW" );
                case EXCEPTION_FLT_STACK_CHECK:
                    return QStringLiteral( "EXCEPTION_FLT_STACK_CHECK" );
                case EXCEPTION_FLT_UNDERFLOW:
                    return QStringLiteral( "EXCEPTION_FLT_UNDERFLOW" );
                case EXCEPTION_ILLEGAL_INSTRUCTION:
                    return QStringLiteral( "EXCEPTION_ILLEGAL_INSTRUCTION" );
                case EXCEPTION_IN_PAGE_ERROR:
                    return QStringLiteral( "EXCEPTION_IN_PAGE_ERROR" );
                case EXCEPTION_INT_DIVIDE_BY_ZERO:
                    return QStringLiteral( "EXCEPTION_INT_DIVIDE_BY_ZERO" );
                case EXCEPTION_INT_OVERFLOW:
                    return QStringLiteral( "EXCEPTION_INT_OVERFLOW" );
                case EXCEPTION_INVALID_DISPOSITION:
                    return QStringLiteral( "EXCEPTION_INVALID_DISPOSITION" );
                case EXCEPTION_NONCONTINUABLE_EXCEPTION:
                    return QStringLiteral( "EXCEPTION_NONCONTINUABLE_EXCEPTION" );
                case EXCEPTION_PRIV_INSTRUCTION:
                    return QStringLiteral( "EXCEPTION_PRIV_INSTRUCTION" );
                case EXCEPTION_SINGLE_STEP:
                    return QStringLiteral( "EXCEPTION_SINGLE_STEP" );
                case EXCEPTION_STACK_OVERFLOW:
                    return QStringLiteral( "EXCEPTION_STACK_OVERFLOW" );
                default:
                    return QStringLiteral( "Unrecognized Exception" );
            }
        } ( inExceptionInfo->ExceptionRecord->ExceptionCode );

        // If this is a stack overflow then we can't walk the stack, so just show
        // where the error happened
        QStringList frameInfoList;

        if ( inExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW )
        {
            // https://stackoverflow.com/a/38019482
#ifdef _M_IX86
            frameInfoList += _addressToLine( sProgramName, reinterpret_cast<void*>(inExceptionInfo->ContextRecord->Eip) );
#elif _M_X64
            frameInfoList += _addressToLine( sProgramName, reinterpret_cast<void*>(inExceptionInfo->ContextRecord->Rip) );
#else
#error You need to implement the call to _addressToLine for this architecture
#endif
        }
        else
        {
            frameInfoList += _stackTrace( inExceptionInfo->ContextRecord );
        }

        _writeLog( cExceptionType, frameInfoList );

        return EXCEPTION_EXECUTE_HANDLER;
    }
#elif defined(Q_OS_ANDROID)
    // no crash reporter for android
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    constexpr qint32  MAX_STACK_FRAMES = 64;
    static void    *sStackTraces[MAX_STACK_FRAMES];
    static uint8_t sAlternateStack[SIGSTKSZ];

    QStringList  _stackTrace()
    {
        qint32   traceSize = backtrace( sStackTraces, MAX_STACK_FRAMES );
        char  **messages = backtrace_symbols( sStackTraces, traceSize );
        // skip the first 2 stack frames (this function and our handler) and skip the last frame (always junk)
        QStringList frameList;
        qint32         frameNumber = 0;
        try
        {
            frameList.reserve( traceSize );
            for ( qint32 i = 2; i < (traceSize - 1); ++i )
            {
                QString  message( messages[i] );

                // match the mangled name if possible and replace with file & line number
                QRegularExpressionMatch match = sSymbolMatching.match( message );

                const QString  cSymbol( match.captured( 1 ) );

                if ( !cSymbol.isNull() )
                {
                    QString  locationStr = _addressToLine( sProgramName, sStackTraces[i] );

                    if ( !locationStr.isEmpty() )
                    {
                        qint32   matchStart = match.capturedStart( 1 );

                        message.replace( matchStart, message.length() - matchStart, locationStr );
                    }
                }

                frameList += message;

                ++frameNumber;
                // anything above isn't really helpful
                if (frameNumber > 10)
                {
                    break;
                }
            }
        } catch (...)
        {

        }
        if ( messages != nullptr )
        {
            free( messages );
        }

        return frameList;
    }

    // prototype to prevent warning about not returning
    void _posixSignalHandler( qint32 inSig, siginfo_t *inSigInfo, void *inContext ) __attribute__ ((noreturn));
    void _posixSignalHandler( qint32 inSig, siginfo_t *inSigInfo, void *inContext )
    {
        Q_UNUSED( inContext )

        const QString  cSignalType = [] ( qint32 sig, qint32 inSignalCode ) {
            switch( sig )
            {
                case SIGSEGV:
                    return QStringLiteral( "Caught SIGSEGV: Segmentation Fault" );
                case SIGINT:
                    return QStringLiteral( "Caught SIGINT: Interactive attention signal, (usually ctrl+c)" );
                case SIGFPE:
                    switch( inSignalCode )
                    {
                        case FPE_INTDIV:
                            return QStringLiteral( "Caught SIGFPE: (integer divide by zero)" );
                        case FPE_INTOVF:
                            return QStringLiteral( "Caught SIGFPE: (integer overflow)" );
                        case FPE_FLTDIV:
                            return QStringLiteral( "Caught SIGFPE: (floating-point divide by zero)" );
                        case FPE_FLTOVF:
                            return QStringLiteral( "Caught SIGFPE: (floating-point overflow)" );
                        case FPE_FLTUND:
                            return QStringLiteral( "Caught SIGFPE: (floating-point underflow)" );
                        case FPE_FLTRES:
                            return QStringLiteral( "Caught SIGFPE: (floating-point inexact result)" );
                        case FPE_FLTINV:
                            return QStringLiteral( "Caught SIGFPE: (floating-point invalid operation)" );
                        case FPE_FLTSUB:
                            return QStringLiteral( "Caught SIGFPE: (subscript out of range)" );
                        default:
                            return QStringLiteral( "Caught SIGFPE: Arithmetic Exception" );
                    }
                case SIGILL:
                    switch( inSignalCode )
                    {
                        case ILL_ILLOPC:
                            return QStringLiteral( "Caught SIGILL: (illegal opcode)" );
                        case ILL_ILLOPN:
                            return QStringLiteral( "Caught SIGILL: (illegal operand)" );
                        case ILL_ILLADR:
                            return QStringLiteral( "Caught SIGILL: (illegal addressing mode)" );
                        case ILL_ILLTRP:
                            return QStringLiteral( "Caught SIGILL: (illegal trap)" );
                        case ILL_PRVOPC:
                            return QStringLiteral( "Caught SIGILL: (privileged opcode)" );
                        case ILL_PRVREG:
                            return QStringLiteral( "Caught SIGILL: (privileged register)" );
                        case ILL_COPROC:
                            return QStringLiteral( "Caught SIGILL: (coprocessor error)" );
                        case ILL_BADSTK:
                            return QStringLiteral( "Caught SIGILL: (internal stack error)" );
                        default:
                            return QStringLiteral( "Caught SIGILL: Illegal Instruction" );
                    }
                case SIGTERM:
                    return QStringLiteral( "Caught SIGTERM: a termination request was sent to the program" );
                case SIGABRT:
                    return QStringLiteral( "Caught SIGABRT: usually caused by an abort() or assert()" );
            }

            return QStringLiteral( "Unrecognized Signal" );
        } ( inSig, inSigInfo->si_code );

        const QStringList cFrameInfoList = _stackTrace();

        _writeLog( cSignalType, cFrameInfoList );

        _Exit(1);
    }

    void _posixSetupSignalHandler()
    {
        // setup alternate stack
#ifdef Q_OS_LINUX
        stack_t ss {static_cast<void*>(sAlternateStack), 0, SIGSTKSZ};
#else
        stack_t ss {static_cast<void*>(sAlternateStack), SIGSTKSZ, 0};
#endif

        if ( sigaltstack( &ss, nullptr ) != 0 )
        {
            err( 1, "sigaltstack" );
        }

        // register our signal handlers
        struct sigaction sigAction;

        sigAction.sa_sigaction = _posixSignalHandler;

        sigemptyset( &sigAction.sa_mask );

#ifdef __APPLE__
        // backtrace() doesn't work on macOS when we use an alternate stack
        sigAction.sa_flags = SA_SIGINFO;
#else
        sigAction.sa_flags = SA_SIGINFO | SA_ONSTACK;
#endif

        if ( sigaction( SIGSEGV, &sigAction, nullptr ) != 0 ) { err( 1, "sigaction" ); }
        if ( sigaction( SIGFPE,  &sigAction, nullptr ) != 0 ) { err( 1, "sigaction" ); }
        if ( sigaction( SIGINT,  &sigAction, nullptr ) != 0 ) { err( 1, "sigaction" ); }
        if ( sigaction( SIGILL,  &sigAction, nullptr ) != 0 ) { err( 1, "sigaction" ); }
        if ( sigaction( SIGTERM, &sigAction, nullptr ) != 0 ) { err( 1, "sigaction" ); }
        if ( sigaction( SIGABRT, &sigAction, nullptr ) != 0 ) { err( 1, "sigaction" ); }
    }
#endif
    void  setSignalHandler(logWrittenCallback inLogWrittenCallback )
    {
        sProgramName = QCoreApplication::arguments().at(0);
        sCrashReportDirPath = QCoreApplication::applicationDirPath() + "/logs";
        sSymbolMatching.optimize();
        sLogWrittenCallback = inLogWrittenCallback;
        if ( sProcess == nullptr )
        {
            sProcess = new QProcess();
            sProcess->setObjectName("CrashreporterProcess");
            sProcess->setProcessChannelMode( QProcess::MergedChannels );
        }
#ifdef Q_OS_WIN
        SetUnhandledExceptionFilter( _winExceptionHandler );
#elif defined(Q_OS_ANDROID)
    // no crash reporter for android
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
        _posixSetupSignalHandler();
#else
    // no crash reporter for other os for now
#endif
    }

}
