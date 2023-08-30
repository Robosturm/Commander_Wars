#include "QCoreApplication"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include "coreengine/gameconsole.h"

namespace oxygine
{
    void WindowBase::initializeGL()
    {
        initializeOpenGLFunctions();
        if (!hasOpenGLFeature(QOpenGLFunctions::Shaders))
        {
            CONSOLE_PRINT("Shaders are not supported by open gl. This may result in a black screen.", GameConsole::eWARNING);
        }
        if (!hasOpenGLFeature(QOpenGLFunctions::Multitexture))
        {
            CONSOLE_PRINT("Multitextures are not supported by open gl. This may result in a black screen.", GameConsole::eWARNING);
        }
        // init oxygine engine
        CONSOLE_PRINT("initialize oxygine", GameConsole::eDEBUG);
        VideoDriver::instance = MemoryManagement::create<VideoDriver>();
        VideoDriver::instance->setDefaultSettings();
        rsCache().setDriver(VideoDriver::instance.get());

        STDRenderer::initialize();

        STDRenderer::instance = MemoryManagement::create<STDRenderer>();
        RenderDelegate::instance = MemoryManagement::create<RenderDelegate>();
        Material::null = MemoryManagement::create<Material>();
        Material::current = Material::null;

        STDRenderer::current = STDRenderer::instance;
        launchGame();
    }

    void WindowBase::redrawUi()
    {
        if (!m_noUi)
        {
            update();
        }
    }
    void WindowBase::resizeGL(qint32 w, qint32 h)
    {
        CONSOLE_PRINT("core::restore()", GameConsole::eDEBUG);
        VideoDriver::instance->restore();
        STDRenderer::restore();
        CONSOLE_PRINT("core::restore() done", GameConsole::eDEBUG);
    }

    void WindowBase::swapDisplayBuffers()
    {
    }

    bool WindowBase::beginRendering()
    {
        if (!m_renderEnabled)
        {
            return false;
        }

        bool ready = STDRenderer::isReady();
        if (ready)
        {
            rsCache().reset();
        }
        else
        {
            CONSOLE_PRINT("!ready", GameConsole::eDEBUG);
        }

        return ready;
    }

    void WindowBase::paintGL()
    {
        // check for termination
        if (m_quit && !m_terminating)
        {
            m_terminating = true;
            CONSOLE_PRINT("Quiting game normally", GameConsole::eDEBUG);
            QCoreApplication::exit();
        }
        if (!m_terminating && m_pausedCounter == 0)
        {
            updateData();
            if (oxygine::Stage::getStage().get() != nullptr)
            {
                oxygine::Stage::getStage()->updateStage();
                if (beginRendering())
                {
                    QColor clearColor(0, 0, 0, 255);
                    QSize windowSize = size();
                    QRect viewport(0, 0, windowSize.width(), windowSize.height());
                    // Render all actors inside the stage. Actor::render will also be called for all its children
                    oxygine::Stage::getStage()->renderStage(clearColor, viewport);
                    swapDisplayBuffers();
                    m_repeatedFramesDropped = 0;
                }
            }
        }
    }
}
