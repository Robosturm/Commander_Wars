#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"

namespace oxygine
{

    class ShaderProgram;
    typedef oxygine::intrusive_ptr<ShaderProgram> spShaderProgram;

    class ShaderProgram: public Object
    {
    public:
        virtual void bind() = 0;

        virtual quint32 getID() const = 0;
    };
}
