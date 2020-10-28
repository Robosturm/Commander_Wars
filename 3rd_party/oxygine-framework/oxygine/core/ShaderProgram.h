#pragma once
#include "../oxygine-include.h"
#include "Object.h"

namespace oxygine
{

    class ShaderProgram;
    typedef oxygine::intrusive_ptr<ShaderProgram> spShaderProgram;

    class ShaderProgram: public Object
    {
    public:
        virtual void bind() = 0;

        virtual unsigned int getID() const = 0;
    };
}
