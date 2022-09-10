#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

namespace oxygine
{
    class ShaderProgram;
    using spShaderProgram = oxygine::intrusive_ptr<ShaderProgram>;
    class ShaderProgram final : public ref_counter
    {
    public:
        explicit ShaderProgram();
        ~ShaderProgram() = default;
        void bind();
        quint32 getID() const;
        qint32 getUniformLocation(const char* id) const;
    };
}
