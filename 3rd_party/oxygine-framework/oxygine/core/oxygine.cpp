#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/Input.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResBuffer.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFontBM.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGLES20.h"
#include <qmutex.h>

#include "coreengine/console.h"

namespace oxygine
{

    namespace core
    {

        void reset()
        {
            Console::print("core::reset()", Console::eDEBUG);
            clearPostProcessItems();
            Restorable::releaseAll();
            PostProcess::freeShaders();
            STDRenderer::reset();
            IVideoDriver::instance->reset();
            Console::print("core::reset() done", Console::eDEBUG);
        }
    }
    void handleErrorPolicy(error_policy ep, QString message)
    {
        switch (ep)
        {
            case ep_show_error:
                Console::print(message, Console::eERROR);
#ifdef GAMEDEBUG
                Q_ASSERT(false);
#endif
                break;
            case ep_show_warning:
                Console::print(message, Console::eWARNING);
                break;
            case ep_ignore_error:
                break;
            default:
                Q_ASSERT(!"not implemented");
        }
    }
}
