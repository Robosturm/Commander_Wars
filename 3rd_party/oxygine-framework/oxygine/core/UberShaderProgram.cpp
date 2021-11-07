#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/vertex.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

namespace oxygine
{

    UberShaderProgram::~UberShaderProgram()
    {
        releaseShaders();
    }

    void UberShaderProgram::init(const QString& fracShader, const QString& vertexShader, const QString& fracTableShader, const QString& fracMatrixShader)
    {
        m_fracShader = fracShader;
        m_vertexShader = vertexShader;
        m_fracTableShader = fracTableShader;
        m_fracMatrixShader = fracMatrixShader;
    }

    void UberShaderProgram::release()
    {
        releaseShaders();
    }

    ShaderProgram* UberShaderProgram::getShaderProgram(qint32 flags)
    {
        shader& s = m_shaders[flags];

        if (!s.program)
        {
            s.flags = flags;
            QString prepend;
            if (flags & ALPHA_PREMULTIPLY)
            {
                prepend += "#define ALPHA_PREMULTIPLY\n";
            }
            if (flags & SEPARATE_ALPHA)
            {
                prepend += "#define SEPARATE_ALPHA\n";
            }
            if (flags & MASK_R_CHANNEL)
            {
                prepend += "#define MASK_R_CHANNEL\n";
            }
            if (flags & MASK)
            {
                prepend += "#define MASK\n";
            }
            if (flags & ADD_COLOR)
            {
                prepend += "#define ADD_COLOR\n";
            }
            QString fs = prepend;
            if (flags & COLOR_MATRIX)
            {
                fs += m_fracMatrixShader;
            }
            else if (flags & COLOR_TABLE)
            {
                fs += m_fracTableShader;
            }
            else
            {
                fs  += m_fracShader;
            }
            QString vs = prepend + m_vertexShader;
            VideoDriver* driver = VideoDriver::instance.get();
            const VertexDeclaration* decl = driver->getVertexDeclaration();

            spShaderProgram pgl = spShaderProgram::create(vs, fs, decl);
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
            s.program = nullptr;
        }
    }

    void UberShaderProgram::apply(VideoDriver* driver, spTexture & base)
    {
        driver->setTexture(UberShaderProgram::SAMPLER_BASE, base);
    }
}
