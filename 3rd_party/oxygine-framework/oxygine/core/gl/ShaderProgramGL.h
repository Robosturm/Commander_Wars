#pragma once
#include <QOpenGLShader>
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"

namespace oxygine
{
    class VertexDeclarationGL;


    class ShaderProgramGL;
    typedef oxygine::intrusive_ptr<ShaderProgramGL> spShaderProgramGL;

    class ShaderProgramGL : public ShaderProgram
    {
    public:
        explicit ShaderProgramGL(QString vsShader, QString fsShader, const VertexDeclarationGL* decl);
        virtual ~ShaderProgramGL() = default;
        virtual quint32 getID() const override;
        qint32 getUniformLocation(const char* id) const;
        virtual void bind() override;
    protected:
        void compileShader(QOpenGLShader& shader, QString data);
    private:
        QOpenGLShaderProgram m_program;
        QOpenGLShader m_vsShader;
        QOpenGLShader m_fsShader;
    };


}
