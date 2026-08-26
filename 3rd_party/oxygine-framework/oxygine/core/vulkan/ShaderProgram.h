#pragma once

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

#include <QVulkanDeviceFunctions>
#include <QVulkanWindow>

namespace oxygine
{
    class ShaderProgram;
    using spShaderProgram = std::shared_ptr<ShaderProgram>;
    class ShaderProgram: public ref_counter
    {
    public:            
        static constexpr const char* const getTypeName()
        {
            return "ShaderProgram";
        }
        explicit ShaderProgram(const QString & vsShader, const QString & fsShader);
        virtual ~ShaderProgram();
    private:
        VkShaderModule createShader(const QString &name);
    private:
        QVulkanDeviceFunctions* m_devFuncs{nullptr};
        QVulkanWindow* m_window{nullptr};
        VkShaderModule m_vertexShaderModule{VK_NULL_HANDLE};
        VkShaderModule m_fracmentShaderModule{VK_NULL_HANDLE};
    };
}
