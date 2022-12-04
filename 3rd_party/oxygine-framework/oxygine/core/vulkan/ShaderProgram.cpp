#include "3rd_party/oxygine-framework/oxygine/core/vulkan/ShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/vulkan/vulkanrenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "coreengine/gameconsole.h"

namespace oxygine
{
    ShaderProgram::ShaderProgram(const QString & vsShader, const QString & fsShader)
    {
        m_vertexShaderModule = createShader(vsShader);
        m_fracmentShaderModule = createShader(fsShader);
    }

    VkShaderModule ShaderProgram::createShader(const QString &name)
    {
        QFile file(name);
        if (!file.open(QIODevice::ReadOnly))
        {
            CONSOLE_PRINT("Unable to load shader: " + name, GameConsole::eFATAL);
            return VK_NULL_HANDLE;
        }

        QByteArray data = file.readAll();
        file.close();

        VkShaderModuleCreateInfo shaderInfo;
        memset(&shaderInfo, 0, sizeof(shaderInfo));
        shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderInfo.codeSize = data.size();
        shaderInfo.pCode = reinterpret_cast<const uint32_t *>(data.constData());
        VkShaderModule shaderModule;
        VkResult err = m_devFuncs->vkCreateShaderModule(m_window->device(), &shaderInfo, nullptr, &shaderModule);
        if (err != VK_SUCCESS) {
            CONSOLE_PRINT("Failed to create shader module: " + QString::number(err), GameConsole::eFATAL);
            return VK_NULL_HANDLE;
        }
        return shaderModule;
    }
}
