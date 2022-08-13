#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "coreengine/console.h"

namespace oxygine
{
    ShaderProgram::ShaderProgram(const QString & vsShader, const QString & fsShader, const VertexDeclaration* decl)
#ifdef GRAPHICSUPPORT
        : m_program(oxygine::GameWindow::getWindow()),
          m_vsShader(QOpenGLShader::Vertex, oxygine::GameWindow::getWindow()),
          m_fsShader(QOpenGLShader::Fragment, oxygine::GameWindow::getWindow())
#endif
    {
#ifdef GRAPHICSUPPORT
        compileShader(m_vsShader, vsShader);
        compileShader(m_fsShader, fsShader);
        m_program.addShader(&m_vsShader);
        m_program.addShader(&m_fsShader);
        for (const auto & el : decl->m_elements)
        {
            m_program.bindAttributeLocation(el.name, el.index);
        }
        bool success = m_program.link();
        QString log = m_program.log();
        if (success)
        {
            CONSOLE_PRINT("compiled shader program", Console::eDEBUG);
        }
        else
        {
            CONSOLE_PRINT("can't compile program: " + log, Console::eFATAL);
        }
#endif
    }

    quint32 ShaderProgram::getID() const
    {
#ifdef GRAPHICSUPPORT
        return m_program.programId();
#else
        return 0;
#endif
    }

    void ShaderProgram::bind()
    {
#ifdef GRAPHICSUPPORT
        m_program.bind();
#endif
    }

    qint32 ShaderProgram::getUniformLocation(const char* id) const
    {
#ifdef GRAPHICSUPPORT
        return m_program.uniformLocation(id);
#else
        return 0;
#endif
    }

#ifdef GRAPHICSUPPORT
    void ShaderProgram::compileShader(QOpenGLShader& shader, const QString & data)
    {

        bool success = shader.compileSourceCode(data);
        QString log = shader.log();
        if (success)
        {
            qDebug("compiled shader. Log:\n %s", log.toStdString().c_str());
        }
        else
        {
            qCritical("can't compile shader. Log:\n%s", log.toStdString().c_str());
        }
    }
#endif

}
