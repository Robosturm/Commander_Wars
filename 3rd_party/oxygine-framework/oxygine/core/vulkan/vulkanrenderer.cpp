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
        m_pWindow->updateData();
        if (m_pWindow->m_pauseMutex.tryLock())
        {
            oxygine::Stage::getStage()->updateStage();
            if (m_pWindow->beginRendering())
            {
                QColor clearColor(0, 255, 0, 255);
                QSize windowSize = m_pWindow->size();
                oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::Point(windowSize.width(), windowSize.height()));
                // Render all actors inside the stage. Actor::render will also be called for all its children
                oxygine::Stage::getStage()->renderStage(clearColor, viewport);
                swapDisplayBuffers();
                m_pWindow->m_repeatedFramesDropped = 0;
            }
            m_pWindow->m_pauseMutex.unlock();
        }
        else
        {
            ++m_pWindow->m_repeatedFramesDropped;
            if (m_pWindow->m_repeatedFramesDropped > 10)
            {
                m_pWindow->requestUpdate();
            }
        }
        // check for termination
        if (m_pWindow->m_quit)
        {
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
