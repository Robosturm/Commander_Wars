#include "3rd_party/oxygine-framework/oxygine/core/renderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.h"

using namespace oxygine;

void Renderer::start()
{
    if (!m_window.m_noUi)
    {
        if (m_window.context()->makeCurrent(&m_window))
        {    
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

            FontManager::getInstance();
            // load ressources by creating the singletons
            BackgroundManager::getInstance();
        }
        else
        {
            CONSOLE_PRINT("failed to initialize oxygine", GameConsole::eFATAL);
        }
    }
}

void Renderer::quit()
{
    auto* context = m_window.context();
    context->doneCurrent();
    context->moveToThread(m_window.thread());
}

void Renderer::resize(qint32 w, qint32 h)
{
    CONSOLE_PRINT("core::restore()", GameConsole::eDEBUG);
    VideoDriver::instance->restore();
    STDRenderer::restore();
    CONSOLE_PRINT("core::restore() done", GameConsole::eDEBUG);
}

bool Renderer::beginRendering()
{
    if (!m_window.m_renderEnabled)
    {
        return false;
    }
    
    bool ready = STDRenderer::isReady();
    if (ready)
    {
        if (!m_window.m_noUi)
        {
            ready = m_window.context()->makeCurrent(&m_window);
            if (ready)
            {
                rsCache().reset();
                VideoDriver::instance->reset();
            }
            else
            {
                CONSOLE_PRINT("Failed to make current context", GameConsole::eDEBUG);
            }
        }
    }
    else
    {
        CONSOLE_PRINT("!ready", GameConsole::eDEBUG);
    }
    return ready;
}


void Renderer::onPaint()
{
    if (!m_window.m_noUi)
    {
        if (m_window.m_pausedCounter == 0)
        {
            if (m_window.m_renderSync.tryLock())
            {
                if (!m_window.m_terminating && m_window.m_pausedCounter == 0)
                {
                    m_window.updateData();
                    if (oxygine::Stage::getStage().get() != nullptr)
                    {
                        oxygine::Stage::getStage()->updateStage();
                        if (beginRendering())
                        {
                            auto* context = m_window.context();
                            QColor clearColor(0, 0, 0, 255);
                            QSize windowSize = m_window.size();
                            QRect viewport(0, 0, windowSize.width(), windowSize.height());
                            // Render all actors inside the stage. Actor::render will also be called for all its children
                            oxygine::Stage::getStage()->renderStage(clearColor, viewport);
                            context->swapBuffers(context->surface());
                            m_repeatedFramesDropped = 0;
                        }
                    }
                }
                m_window.m_renderSync.unlock();
            }
        }
    }
}
