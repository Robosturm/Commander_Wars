#pragma once
#include "../../oxygine-include.h"
#include "../VertexDeclaration.h"
#include <qopengl.h>

namespace oxygine
{
    class VertexDeclarationGL: public VertexDeclaration
    {
    public:
        void init(bvertex_format);

        struct Element
        {
            QString name;
            int index;
            bool normalized;
            GLint size;
            GLenum elemType;
            int offset;
        };

        Element elements[8];

    private:

    };
}
