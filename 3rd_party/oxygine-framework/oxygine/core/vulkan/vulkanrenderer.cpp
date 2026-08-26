#include <QCoreApplication>

#include "3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include "coreengine/gameconsole.h"

namespace oxygine
{
    QVulkanDeviceFunctions* VulkanRenderer::m_pDeviceFunctions = nullptr;

    VulkanRenderer::VulkanRenderer(WindowBase* pWindow)
        : m_pWindow(pWindow)
    {
    }

    void VulkanRenderer::initResources()
    {
        m_pDeviceFunctions = m_pWindow->vulkanInstance()->deviceFunctions(m_pWindow->device());
        // init oxygine engine
        CONSOLE_PRINT("initialize oxygine", GameConsole::eDEBUG);
        VideoDriver::instance = spVideoDriver::create();
        VideoDriver::instance->setDefaultSettings();
        rsCache().setDriver(VideoDriver::instance.get());

        STDRenderer::initialize();

        STDRenderer::instance = spSTDRenderer::create();
        RenderDelegate::instance = spRenderDelegate::create();
        Material::null = spMaterial::create();
        Material::current = Material::null;

        STDRenderer::current = STDRenderer::instance;
        m_pWindow->launchGame();
    }

    void VulkanRenderer::startNextFrame()
    {
        if (!m_pWindow->m_quit && !m_pWindow->m_terminating)
        {
            m_pWindow->updateData();
            if (m_pWindow->m_renderSync.tryLock())
            {
                if (oxygine::Stage::getStage().get() != nullptr)
                {
                    oxygine::Stage::getStage()->updateStage();
                    if (m_pWindow->beginRendering())
                    {
                        QColor clearColor(0, 0, 0, 255);
                        QSize windowSize = m_pWindow->size();
                        oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::Point(windowSize.width(), windowSize.height()));
                        oxygine::Stage::getStage()->renderStage(clearColor, viewport);
                        swapDisplayBuffers();
                        m_pWindow->m_repeatedFramesDropped = 0;
                    }
                }
                m_pWindow->m_renderSync.unlock();
            }
            else
            {
                ++m_pWindow->m_repeatedFramesDropped;
                if (m_pWindow->m_repeatedFramesDropped > 10)
                {
                    m_pWindow->requestUpdate();
                }
            }
        }
        // check for termination
        if (m_pWindow->m_quit && !m_pWindow->m_terminating)
        {
            m_pWindow->m_terminating = true;
            CONSOLE_PRINT("Quiting game normally", GameConsole::eDEBUG);
            QCoreApplication::exit();
        }
        m_pWindow->frameReady();
    }

    void VulkanRenderer::swapDisplayBuffers()
    {
        m_pDeviceFunctions->vkCmdEndRenderPass(m_pWindow->currentCommandBuffer());
        VideoDriver::m_stats.duration = Clock::getTimeMS() - VideoDriver::m_stats.start;
    }
}
