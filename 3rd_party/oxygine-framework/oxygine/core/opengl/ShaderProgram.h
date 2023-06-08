#pragma once
#include <QOpenGLShader>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

namespace oxygine
{
    class ShaderProgram;
    using spShaderProgram = oxygine::intrusive_ptr<ShaderProgram>;
    class ShaderProgram final : public ref_counter
    {
    public:
        explicit ShaderProgram(const QString & vsShader, const QString & fsShader, const VertexDeclaration* decl);
       virtual ~ShaderProgram() = default;
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
