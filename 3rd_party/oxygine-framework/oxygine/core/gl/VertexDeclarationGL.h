#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
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

        Element m_elements[8];

    private:

    };
}
