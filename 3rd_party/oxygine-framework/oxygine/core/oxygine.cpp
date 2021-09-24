#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/Input.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResBuffer.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFontBM.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
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
    void handleErrorPolicy(error_policy ep, QString message)
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
