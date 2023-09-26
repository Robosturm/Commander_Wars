#pragma once

#include <QVulkanFunctions>

namespace oxygine
{
    class ShaderProgram;
    using spShaderProgram = std::shared_ptr<ShaderProgram>;
    class ShaderProgram: public ref_counter
    {
    public:
        explicit ShaderProgram(const QString & vsShader, const QString & fsShader);
        virtual ~ShaderProgram() = default;
    private:
        VkShaderModule createShader(const QString &name);
    private:
        VkShaderModule m_vertexShaderModule;
        VkShaderModule m_fracmentShaderModule;
    };
}
