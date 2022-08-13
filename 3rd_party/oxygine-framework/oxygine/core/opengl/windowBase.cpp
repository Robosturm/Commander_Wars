#include "QCoreApplication"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include "coreengine/console.h"

namespace oxygine
{
    void WindowBase::initializeGL()
    {
        initializeOpenGLFunctions();
        if (!hasOpenGLFeature(QOpenGLFunctions::Shaders))
        {
            CONSOLE_PRINT("Shaders are not supported by open gl. This may result in a black screen.", Console::eWARNING);
        }
        if (!hasOpenGLFeature(QOpenGLFunctions::Multitexture))
        {
            CONSOLE_PRINT("Multitextures are not supported by open gl. This may result in a black screen.", Console::eWARNING);
        }
        // init oxygine engine
        CONSOLE_PRINT("initialize oxygine", Console::eDEBUG);
        VideoDriver::instance = spVideoDriver::create();
        VideoDriver::instance->setDefaultSettings();
        rsCache().setDriver(VideoDriver::instance.get());

        STDRenderer::initialize();

        STDRenderer::instance = spSTDRenderer::create();
        RenderDelegate::instance = spRenderDelegate::create();
        Material::null = spMaterial::create();
        Material::current = Material::null;

        STDRenderer::current = STDRenderer::instance;
        launchGame();
    }

    void WindowBase::timerEvent(QTimerEvent *)
    {
        // Request an update
        update();
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
        CONSOLE_PRINT("core::restore()", Console::eDEBUG);
        VideoDriver::instance->restore();
        STDRenderer::restore();
        CONSOLE_PRINT("core::restore() done", Console::eDEBUG);
    }

    void WindowBase::swapDisplayBuffers()
    {
        VideoDriver::m_stats.duration = Clock::getTimeMS() - VideoDriver::m_stats.start;
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
            VideoDriver::m_stats.start = Clock::getTimeMS();
            rsCache().reset();
        }
        else
        {
            CONSOLE_PRINT("!ready", Console::eDEBUG);
        }

        return ready;
    }

    void WindowBase::paintGL()
    {
        updateData();
        if (m_pauseMutex.tryLock())
        {
            oxygine::Stage::getStage()->updateStage();
            if (beginRendering())
            {
                QColor clearColor(0, 0, 0, 255);
                QSize windowSize = size();
                oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::Point(windowSize.width(), windowSize.height()));
                // Render all actors inside the stage. Actor::render will also be called for all its children
                oxygine::Stage::getStage()->renderStage(clearColor, viewport);
                swapDisplayBuffers();
                m_repeatedFramesDropped = 0;
            }
            m_pauseMutex.unlock();
        }
        else
        {
            ++m_repeatedFramesDropped;
            if (m_repeatedFramesDropped > 10)
            {
                update();
            }
        }
        // check for termination
        if (m_quit)
        {
            CONSOLE_PRINT("Quiting game normally", Console::eDEBUG);
            QCoreApplication::exit();
        }
    }
}
