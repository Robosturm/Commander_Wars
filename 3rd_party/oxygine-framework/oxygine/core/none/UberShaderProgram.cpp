#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "coreengine/memorymanagement.h"
#include "texture.h"

namespace oxygine
{

    UberShaderProgram::~UberShaderProgram()
    {
    }

    void UberShaderProgram::init()
    {
    }

    void UberShaderProgram::release()
    {
    }

    ShaderProgram* UberShaderProgram::getShaderProgram(ColorMode fracShader)
    {
        shader& s = m_shaders[fracShader];
        if (!s.program)
        {
            s.fracShader = fracShader;
            VideoDriver* driver = VideoDriver::instance.get();
            spShaderProgram pgl = MemoryManagement::create<ShaderProgram>();
            driver->setShaderProgram(pgl.get());
            driver->setUniformInt("base_texture", UberShaderProgram::SAMPLER_BASE);
            driver->setUniformInt("colorTable", UberShaderProgram::SAMPLER_TABLE);
            driver->setUniformInt("mask_texture", UberShaderProgram::SAMPLER_MASK);
            s.program = pgl;
        }
        return s.program.get();
    }

    void UberShaderProgram::releaseShaders()
    {
        for (qint32 i = 0; i < _SIZE; ++i)
        {
            shader& s = m_shaders[i];
            s.program.reset();
        }
    }

    void UberShaderProgram::apply(VideoDriver* driver, spTexture & base)
    {
        driver->setTexture(UberShaderProgram::SAMPLER_BASE, base);
    }
}
