#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

#include "coreengine/gameconsole.h"

#include <QFile>
#include <QTextStream>

namespace oxygine
{

    UberShaderProgram::~UberShaderProgram()
    {
        releaseShaders();
    }

    void UberShaderProgram::init()
    {
        QString fracShaderBody;
        QString fracTableShaderBody;
        QString fracMatrixShaderBody;
        QString vertexShaderBody;
        QString filepath = "system/frac_shader.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            fracShaderBody = stream.readAll();
        }
        else
        {
            CONSOLE_PRINT("Unable to find frac shader: " + filepath, GameConsole::eERROR);
        }
        filepath = "system/vertex_shader.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            vertexShaderBody = stream.readAll();
        }
        else
        {
            CONSOLE_PRINT("Unable to find vertex shader: " + filepath, GameConsole::eERROR);
        }
        filepath = "system/frac_table_shader.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            fracTableShaderBody = stream.readAll();
        }
        else
        {
            CONSOLE_PRINT("Unable to find frac shader: " + filepath, GameConsole::eERROR);
        }
        filepath = "system/frac_matrix_shader.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            fracMatrixShaderBody = stream.readAll();
        }
        else
        {
            CONSOLE_PRINT("Unable to find frac shader: " + filepath, GameConsole::eERROR);
        }
        m_fracShader = fracShaderBody;
        m_vertexShader = vertexShaderBody;
        m_fracTableShader = fracTableShaderBody;
        m_fracMatrixShader = fracMatrixShaderBody;
    }

    void UberShaderProgram::release()
    {
        releaseShaders();
    }

    ShaderProgram* UberShaderProgram::getShaderProgram(ColorMode fracShader)
    {
        shader& s = m_shaders[fracShader];

        if (!s.program)
        {
            s.fracShader = fracShader;
            QString fs;
            if (fracShader == COLOR_MATRIX)
            {
                fs = m_fracMatrixShader;
            }
            else if (fracShader == COLOR_TABLE)
            {
                fs = m_fracTableShader;
            }
            else
            {
                fs  = m_fracShader;
            }
            QString vs = m_vertexShader;
            VideoDriver* driver = VideoDriver::instance.get();
            const VertexDeclaration* decl = driver->getVertexDeclaration();

            spShaderProgram pgl = MemoryManagement::create<ShaderProgram>(vs, fs, decl);
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
