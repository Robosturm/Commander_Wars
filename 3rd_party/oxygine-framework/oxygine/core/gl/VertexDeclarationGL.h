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
            qint32 index;
            bool normalized;
            GLint size;
            GLenum elemType;
            qint32 offset;
        };

        Element elements[8];

    private:

    };
}
