#include "coreengine/crashreporter.h"
#include <csignal>
#include <err.h>
#include <execinfo.h>

#if defined(SIGSTKSZ)
    #define STACK_SIZE SIGSTKSZ
#elif defined(_SC_SIGSTKSZ)
    #define STACK_SIZE _SC_SIGSTKSZ
#else
    #error("unsupported build environment")
#endif

static uint8_t sAlternateStack[STACK_SIZE];

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

    CrashReporter::_writeLog(cSignalType);

    _Exit(1);
}

void CrashReporter::setOsSignalHandler()
{
    // setup alternate stack
#ifdef Q_OS_LINUX
    stack_t ss {static_cast<void*>(sAlternateStack), 0, STACK_SIZE};
#else
    stack_t ss {static_cast<void*>(sAlternateStack), STACK_SIZE, 0};
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

    if ( sigaction( SIGSEGV, &sigAction, nullptr ) != 0 )
    {
        err( 1, "sigaction" );
    }
    if ( sigaction( SIGFPE,  &sigAction, nullptr ) != 0 )
    {
        err( 1, "sigaction" );
    }
    if ( sigaction( SIGINT,  &sigAction, nullptr ) != 0 )
    {
        err( 1, "sigaction" );
    }
    if ( sigaction( SIGILL,  &sigAction, nullptr ) != 0 )
    {
        err( 1, "sigaction" );
    }
    if ( sigaction( SIGTERM, &sigAction, nullptr ) != 0 )
    {
        err( 1, "sigaction" );
    }
    if ( sigaction( SIGABRT, &sigAction, nullptr ) != 0 )
    {
        err( 1, "sigaction" );
    }
}
