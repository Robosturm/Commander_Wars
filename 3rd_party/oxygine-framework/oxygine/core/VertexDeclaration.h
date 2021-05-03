#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/vertex.h"

namespace oxygine
{
    class VertexDeclaration
    {
    public:
        VertexDeclaration(): bformat(0), numElements(0), size(0) {}

        bvertex_format bformat;
        qint32 numElements;
        qint32 size;
    };

    template <class T>
    class VertexDeclarations
    {
    public:
        const T* get(bvertex_format bformat)
        {
            for (qint32 i = 0; i < NUM; ++i)
            {
                T& decl = _declarations[i];
                if (decl.bformat == 0)
                {
                    decl.init(bformat);
                }
                if (decl.bformat == bformat)
                {
                    return &decl;
                }
            }

            return 0;
        }

    protected:
        enum {NUM = 8};
        T _declarations[NUM];
    };
}
