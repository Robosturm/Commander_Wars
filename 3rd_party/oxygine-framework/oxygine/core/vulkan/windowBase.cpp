#include <QVulkanInstance>

#include "3rd_party/oxygine-framework/oxygine/core/vulkan/windowBase.h"
#include "3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include "coreengine/console.h"

namespace oxygine
{
    QVulkanInstance WindowBase::m_vulkanInstance;

    WindowBase::WindowBase()
    {
        setup();
        setVulkanInstance(&m_vulkanInstance);
    }

    void WindowBase::setup()
    {
        m_vulkanInstance.setLayers({ "VK_LAYER_KHRONOS_validation" });
        if (!m_vulkanInstance.create())
        {
            CONSOLE_PRINT("Failed to create Vulkan instance: " + QString::number(m_vulkanInstance.errorCode()), Console::eFATAL);
        }
    }

    void WindowBase::redrawUi()
    {
        if (!m_noUi)
        {
            requestUpdate();
        }
    }

    void WindowBase::timerEvent(QTimerEvent *)
    {
        requestUpdate();
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

    QVulkanWindowRenderer *WindowBase::createRenderer()
    {
        return new VulkanRenderer(this);
    }
}
