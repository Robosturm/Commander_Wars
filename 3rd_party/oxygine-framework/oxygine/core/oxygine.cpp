#include "oxygine.h"
#include "qmutex.h"
#include "VideoDriver.h"
#include "../Image.h"
#include "../Input.h"
#include "../KeyEvent.h"
#include "../MaterialCache.h"
#include "../PointerState.h"
#include "../PostProcess.h"
#include "../STDRenderDelegate.h"
#include "../actor/Stage.h"
#include "../actor/TextField.h"
#include "../res/CreateResourceContext.h"
#include "../res/ResAtlas.h"
#include "../res/ResBuffer.h"
#include "../res/ResFontBM.h"
#include "../res/Resources.h"
#include "gl/VideoDriverGLES20.h"
#include <stdio.h>

namespace oxygine
{

    namespace core
    {

        void reset()
        {
            qDebug("core::reset()");
            clearPostProcessItems();
            Restorable::releaseAll();
            PostProcess::freeShaders();
            STDRenderer::reset();
            IVideoDriver::instance->reset();
            qDebug("core::reset() done");
        }
    }
    void handleErrorPolicy(error_policy ep, QString message)
    {
        switch (ep)
        {
            case ep_show_error:
                qCritical() << message;
                // Q_ASSERT(!"handleErrorPolicy error.");
                break;
            case ep_show_warning:
                qWarning() << message;
                break;
            case ep_ignore_error:
                break;
            default:
                Q_ASSERT(!"not implemented");
        }
    }
}
