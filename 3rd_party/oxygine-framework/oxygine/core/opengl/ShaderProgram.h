#pragma once
#include <QOpenGLShader>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

namespace oxygine
{
    class ShaderProgram;
    using spShaderProgram = std::shared_ptr<ShaderProgram>;
    class ShaderProgram final
    {
    public:           
        static constexpr const char* const getTypeName()
        {
            return "ShaderProgram";
        }
        explicit ShaderProgram(const QString & vsShader, const QString & fsShader, const VertexDeclaration* decl);
        ~ShaderProgram() = default;
        void bind();
        quint32 getID() const;
        qint32 getUniformLocation(const char* id) const;
    protected:
        void compileShader(QOpenGLShader& shader, const QString & data);
    private:
        QOpenGLShaderProgram m_program;
        QOpenGLShader m_vsShader;
        QOpenGLShader m_fsShader;
    };
}
