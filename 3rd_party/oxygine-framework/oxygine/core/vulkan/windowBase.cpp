#include <QVulkanInstance>

#include "3rd_party/oxygine-framework/oxygine/core/vulkan/windowBase.h"
#include "3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include "coreengine/gameconsole.h"

namespace oxygine
{
    QVulkanInstance WindowBase::m_vulkanInstance;

    QSurfaceFormat::RenderableType WindowBase::getRenderableType()
    {
        return QSurfaceFormat::RenderableType::OpenVG;
    }

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
            CONSOLE_PRINT("Failed to create Vulkan instance: " + QString::number(m_vulkanInstance.errorCode()), GameConsole::eFATAL);
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
        }
        else
        {
            CONSOLE_PRINT("!ready", GameConsole::eDEBUG);
        }

        return ready;
    }

    QVulkanWindowRenderer *WindowBase::createRenderer()
    {
        return new VulkanRenderer(this);
    }
}
