#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include <qmutex.h>

#include "coreengine/gameconsole.h"

namespace oxygine
{
    namespace core
    {
        void reset()
        {
            CONSOLE_PRINT("core::reset()", GameConsole::eDEBUG);
            STDRenderer::reset();
            VideoDriver::instance->reset();
            CONSOLE_PRINT("core::reset() done", GameConsole::eDEBUG);
        }
    }
    void handleErrorPolicy(error_policy ep, const QString & message)
    {
        switch (ep)
        {
            case ep_show_error:
                CONSOLE_PRINT(message, GameConsole::eERROR);
                OXY_ASSERT(false);
                break;
            case ep_show_warning:
                CONSOLE_PRINT(message, GameConsole::eWARNING);
                break;
            case ep_ignore_error:
                CONSOLE_PRINT(message, GameConsole::eDEBUG);
                break;
            default:
                OXY_ASSERT(!"not implemented");
        }
    }
}
