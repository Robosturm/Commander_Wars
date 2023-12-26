#include <windows.h>
#include <cxxabi.h>
#include <imagehlp.h>
#include <QProcess>
#include <QCoreApplication>
#include <QStringList>
#include <QRegularExpressionMatch>
#include <QFile>
#include "coreengine/crashreporter.h"

QString getExeptionName(EXCEPTION_POINTERS *inExceptionInfo)
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
    return cExceptionType;
}

QString _addressToLine( const QString &inProgramName, void const * const inAddr )
{
    QProcess* crashProcess = CrashReporter::getCrashProcess();
    if (crashProcess->state() == QProcess::NotRunning)
    {
        const QString  cAddrStr = QStringLiteral("0x%1").arg(quintptr(inAddr), 16, 16, QChar('0'));
        // Uses addr2line
        const QString  cProgram = QStringLiteral("%1/addr2line.exe").arg(QCoreApplication::applicationDirPath());
        if (QFile::exists(cProgram))
        {
            const QStringList  cArguments =
                {
                    "-f",
                    "-p",
                    "-e", inProgramName,
                    cAddrStr
                };
            crashProcess->setProgram(cProgram);
            crashProcess->setArguments(cArguments);
            crashProcess->start(QIODevice::ReadOnly);
            if (!crashProcess->waitForFinished())
            {
                return QStringLiteral( "* Error running command\n   %1 %2\n   %3" ).arg(
                    crashProcess->program(),
                    crashProcess->arguments().join( ' ' ),
                    crashProcess->errorString() );
            }
            const QString  cLocationStr = QString( crashProcess->readAll() ).trimmed();
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
            return cAddrStr;
        }
    }
    else
    {
        return "Crash reporting already running.\n Critical error caused another thread to crash. Only one crash can be gathered and reported";
    }
}

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
#else \
    // see http://theorangeduck.com/page/printing-stack-trace-mingw
#error You need to define the stack frame layout for this architecture
#endif
    QRegularExpression sSymbolMatching("^.*(_Z[^ ]+).*$");
    QStringList frameList;
    try
    {
        qint32         frameNumber = 0;
        while (StackWalk64(image, process, thread,
                           &stackFrame, context, nullptr,
                           SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
        {
            QString  locationStr = _addressToLine(CrashReporter::getProgramName(), reinterpret_cast<void*>(stackFrame.AddrPC.Offset));
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
            if (frameNumber > CrashReporter::STACKSIZE)
            {
                break;
            }
        }
    }
    catch (...)
    {
        frameList.append("Error while determing stacktrace crashing without stacktrace.");
    }
    SymCleanup(GetCurrentProcess());
    return frameList;
}

LONG WINAPI _winExceptionHandler( EXCEPTION_POINTERS *inExceptionInfo )
{
    // If this is a stack overflow then we can't walk the stack, so just show
    // where the error happened
    QStringList frameInfoList;
    if ( inExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW )
    {
        // https://stackoverflow.com/a/38019482
#ifdef _M_IX86
        frameInfoList += _addressToLine( CrashReporter::getProgramName(), reinterpret_cast<void*>(inExceptionInfo->ContextRecord->Eip) );
#elif _M_X64
        frameInfoList += _addressToLine( CrashReporter::getProgramName(), reinterpret_cast<void*>(inExceptionInfo->ContextRecord->Rip) );
#else
#error You need to implement the call to _addressToLine for this architecture
#endif
    }
    else
    {
        frameInfoList += _stackTrace( inExceptionInfo->ContextRecord );
    }
    CrashReporter::_writeLog(getExeptionName(inExceptionInfo), frameInfoList);
    return EXCEPTION_EXECUTE_HANDLER;
}

void CrashReporter::setOsSignalHandler()
{
    SetUnhandledExceptionFilter( _winExceptionHandler );
}
