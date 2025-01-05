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

    static QString loadShader(QString name) {
        QString filepath = oxygine::Resource::RCC_PREFIX_PATH + name;
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            return stream.readAll();
        }
        else
        {
            CONSOLE_PRINT("Unable to find shader: " + filepath, GameConsole::eERROR);
            return "";
        }
    }

    void UberShaderProgram::init()
    {
        m_fracShader = loadShader("system/frac_shader.glsl");
        m_vertexShader = loadShader("system/vertex_shader.glsl");
        m_fracTableShader = loadShader("system/frac_table_shader.glsl");
        m_fracMatrixShader = loadShader("system/frac_matrix_shader.glsl");
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
