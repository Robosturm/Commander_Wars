#include "3rd_party/oxygine-framework/oxygine/core/gl/ShaderProgramGL.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VertexDeclarationGL.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    ShaderProgramGL::ShaderProgramGL(QString vsShader, QString fsShader, const VertexDeclarationGL* decl)
        : _program(oxygine::GameWindow::getWindow()),
          _vsShader(QOpenGLShader::Vertex, oxygine::GameWindow::getWindow()),
          _fsShader(QOpenGLShader::Fragment, oxygine::GameWindow::getWindow())
    {
        compileShader(_vsShader, vsShader);
        compileShader(_fsShader, fsShader);
        _program.addShader(&_vsShader);
        _program.addShader(&_fsShader);
        for (int i = 0; i < decl->numElements; ++i)
        {
            _program.bindAttributeLocation(decl->elements[i].name, decl->elements[i].index);
        }
        bool success = _program.link();
        QString log = _program.log();
        if (success)
        {
            qDebug("compiled program:\n %s", log.toStdString().c_str());
        }
        else
        {
            qCritical("can't compile program: %s", log.toStdString().c_str());
        }
    }

    ShaderProgramGL::~ShaderProgramGL()
    {
    }

    unsigned int ShaderProgramGL::getID() const
    {
        return _program.programId();
    }

    void ShaderProgramGL::bind()
    {
        _program.bind();
    }

    int ShaderProgramGL::getUniformLocation(const char* id) const
    {
        return _program.uniformLocation(id);
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
