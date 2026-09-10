#include "QCoreApplication"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include "coreengine/gameconsole.h"

namespace oxygine
{
    WindowBase::WindowBase()
        : m_renderer(*this)
    {
#ifdef GRAPHICSUPPORT
        m_renderThread = MemoryManagement::createNamedQObject<QThread>("QThread");
        m_renderThread->setObjectName("Renderthread");
#endif
    }

    void WindowBase::setupRendering()
    {
        makeCurrent();
        doneCurrent();
        QOpenGLContext *context = QOpenGLWindow::context();
        context->moveToThread(m_renderThread.get());
        m_renderer.moveToThread(m_renderThread.get());
        m_renderThread->start(QThread::Priority::HighestPriority);
        m_renderingInitialized = true;
        emit m_renderer.sigStart();
        //m_renderer.start();
    }

    QSurfaceFormat::RenderableType WindowBase::getRenderableType()
    {
        return QSurfaceFormat::RenderableType::DefaultRenderableType;
    }

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
        if (!m_pausedCounter.is_lock_free())
        {
            CONSOLE_PRINT("m_pausedCounter is not lock free. This may result in worse performance.", GameConsole::eWARNING);
        }       
        
        launchGame();
    }

    void WindowBase::redrawUi()
    {
        if (!m_noUi && m_renderingInitialized)
        {
            // check for termination
            if (m_quit && !m_terminating)
            {
                m_terminating = true;
                CONSOLE_PRINT("Quiting game normally", GameConsole::eDEBUG);
                QCoreApplication::exit();
            }
            else
            {
                emit m_renderer.sigPaintGl();
            }
        }
    }

    void WindowBase::resizeGL(qint32 w, qint32 h)
    {
        if (m_renderingInitialized)
        {
            emit m_renderer.sigResize(w, h);
        }
    }

    void WindowBase::swapDisplayBuffers()
    {
    }

    void WindowBase::paintGL()
    {
    }
}
