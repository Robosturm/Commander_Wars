#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include <qmutex.h>

#include "coreengine/console.h"

namespace oxygine
{
    namespace core
    {
        void reset()
        {
            CONSOLE_PRINT("core::reset()", Console::eDEBUG);
            STDRenderer::reset();
            VideoDriver::instance->reset();
            CONSOLE_PRINT("core::reset() done", Console::eDEBUG);
        }
    }
    void handleErrorPolicy(error_policy ep, const QString & message)
    {
        switch (ep)
        {
            case ep_show_error:
                CONSOLE_PRINT(message, Console::eERROR);
#ifdef GAMEDEBUG
                Q_ASSERT(false);
#endif
                break;
            case ep_show_warning:
                CONSOLE_PRINT(message, Console::eWARNING);
                break;
            case ep_ignore_error:
                break;
            default:
                Q_ASSERT(!"not implemented");
        }
    }
}
