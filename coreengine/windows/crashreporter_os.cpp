#include <windows.h>
#include "coreengine/crashreporter.h"

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
    CrashReporter::_writeLog( cExceptionType);
    return EXCEPTION_EXECUTE_HANDLER;
}

void CrashReporter::setOsSignalHandler()
{
    SetUnhandledExceptionFilter( _winExceptionHandler );
}
