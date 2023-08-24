#pragma once

#include <QtTypes>
#include <memory>

namespace oxygine
{
    class ShaderProgram;
    using spShaderProgram = std::shared_ptr<ShaderProgram>;
    class ShaderProgram final
    {
    public:
        explicit ShaderProgram();
        ~ShaderProgram() = default;
        void bind();
        quint32 getID() const;
        qint32 getUniformLocation(const char* id) const;
    };
}
