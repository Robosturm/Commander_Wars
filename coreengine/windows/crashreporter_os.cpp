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

void _fetchError(QStringList & frameList)
{
    DWORD dw = GetLastError();
    frameList += (QString("Failed error: ") + QString::number(dw));
}

QStringList _stackTrace( CONTEXT* context )
{
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    QStringList frameList;
    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(STACKFRAME64));

    DWORD   image;
#ifdef _M_IX86
    image                 = IMAGE_FILE_MACHINE_I386;
    stackFrame.AddrPC.Offset    = context->Eip;
    stackFrame.AddrPC.Mode      = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Ebp;
    stackFrame.AddrFrame.Mode   = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Esp;
    stackFrame.AddrStack.Mode   = AddrModeFlat;
#elif _M_X64
    image                 = IMAGE_FILE_MACHINE_AMD64;
    stackFrame.AddrPC.Offset    = context->Rip;
    stackFrame.AddrPC.Mode      = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Rsp;
    stackFrame.AddrFrame.Mode   = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Rsp;
    stackFrame.AddrStack.Mode   = AddrModeFlat;
#elif _M_IA64
    image                 = IMAGE_FILE_MACHINE_IA64;
    stackFrame.AddrPC.Offset    = context->StIIP;
    stackFrame.AddrPC.Mode      = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->IntSp;
    stackFrame.AddrFrame.Mode   = AddrModeFlat;
    stackFrame.AddrBStore.Offset= context->RsBSP;
    stackFrame.AddrBStore.Mode  = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->IntSp;
    stackFrame.AddrStack.Mode   = AddrModeFlat;
#else
#error "Unsupported platform"
#endif
    try
    {
        qint32         frameNumber = 0;
        while (StackWalk64(image, process, thread, &stackFrame, context, nullptr,
                         SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
        {
            char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
            PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
            DWORD64  dwDisplacement = 0;
            pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            pSymbol->MaxNameLen = MAX_SYM_NAME;
            QString symbolName = "Unknown";
            if (!SymFromAddr(process, stackFrame.AddrPC.Offset, &dwDisplacement, pSymbol))
            {
                _fetchError(frameList);
            }
            else
            {
                symbolName = QString(pSymbol->Name);
            }
            frameList += QStringLiteral("Frame : %1 PC address: 0x%2 Stack address: 0x%3 Frame address:  0x%4 Symbol name: %5")
                             .arg(frameNumber)
                             .arg(stackFrame.AddrPC.Offset, 16, 16, QChar('0'))
                             .arg(stackFrame.AddrStack.Offset  , 16, 16, QChar('0'))
                             .arg(stackFrame.AddrFrame.Offset, 16, 16, QChar('0'))
                             .arg(symbolName);
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
    SymCleanup(process);
    return frameList;
}

LONG WINAPI _winExceptionHandler( EXCEPTION_POINTERS *inExceptionInfo )
{
    // If this is a stack overflow then we can't walk the stack, so just show
    // where the error happened
    QStringList frameInfoList;
    if ( inExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW )
    {
        CrashReporter::_writeLog(getExeptionName(inExceptionInfo), frameInfoList);
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

    HANDLE process = GetCurrentProcess();
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
    if (!SymInitialize(process, nullptr, true))
    {
        qWarning("SymInitialize returned error : %d", GetLastError());
    }
}
