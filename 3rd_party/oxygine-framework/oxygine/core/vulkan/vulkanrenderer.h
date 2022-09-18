#pragma once

#include <QVulkanWindowRenderer>
#include <QVulkanDeviceFunctions>

#include "3rd_party/oxygine-framework/oxygine/core/vulkan/windowBase.h"

namespace oxygine
{
    class VulkanRenderer : public QVulkanWindowRenderer
    {
    public:
        VulkanRenderer(WindowBase* pWindow);

        virtual void initResources() override;
        virtual void startNextFrame() override;
        static QVulkanDeviceFunctions* getDeviceFunctions()
        {
            return m_pDeviceFunctions;
        }
    private:
        void swapDisplayBuffers();
    private:
        WindowBase* m_pWindow;
        static QVulkanDeviceFunctions* m_pDeviceFunctions;
    };
}
