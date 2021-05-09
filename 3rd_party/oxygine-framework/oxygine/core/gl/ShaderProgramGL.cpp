#include "3rd_party/oxygine-framework/oxygine/core/gl/ShaderProgramGL.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VertexDeclarationGL.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "coreengine/console.h"

namespace oxygine
{
    ShaderProgramGL::ShaderProgramGL(QString vsShader, QString fsShader, const VertexDeclarationGL* decl)
        : m_program(oxygine::GameWindow::getWindow()),
          m_vsShader(QOpenGLShader::Vertex, oxygine::GameWindow::getWindow()),
          m_fsShader(QOpenGLShader::Fragment, oxygine::GameWindow::getWindow())
    {
        compileShader(m_vsShader, vsShader);
        compileShader(m_fsShader, fsShader);
        m_program.addShader(&m_vsShader);
        m_program.addShader(&m_fsShader);
        for (qint32 i = 0; i < decl->numElements; ++i)
        {
            m_program.bindAttributeLocation(decl->m_elements[i].name, decl->m_elements[i].index);
        }
        bool success = m_program.link();
        QString log = m_program.log();
        if (success)
        {
            Console::print("compiled shader program", Console::eDEBUG);
        }
        else
        {
            Console::print("can't compile program: " + log, Console::eFATAL);
        }
    }

    ShaderProgramGL::~ShaderProgramGL()
    {
    }

    quint32 ShaderProgramGL::getID() const
    {
        return m_program.programId();
    }

    void ShaderProgramGL::bind()
    {
        m_program.bind();
    }

    qint32 ShaderProgramGL::getUniformLocation(const char* id) const
    {
        return m_program.uniformLocation(id);
    }

    void ShaderProgramGL::compileShader(QOpenGLShader& shader, QString data)
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

}
