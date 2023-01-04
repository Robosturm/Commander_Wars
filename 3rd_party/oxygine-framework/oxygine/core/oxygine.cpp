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
    void handleErrorPolicy(error_policy ep, const QString & message, quint64 module)
    {
        switch (ep)
        {
            case ep_show_error:
                CONSOLE_PRINT(message, GameConsole::eERROR);
                OXY_ASSERT(false);
                break;
            case ep_show_warning:
                CONSOLE_PRINT_MODULE(message, GameConsole::eWARNING, module);
                break;
            case ep_ignore_error:
                CONSOLE_PRINT_MODULE(message, GameConsole::eDEBUG, module);
                break;
            default:
                OXY_ASSERT(!"not implemented");
        }
    }
}
