#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    DECLARE_SMART(Polygon, spPolygon);
    class Polygon : public Sprite
    {
    public:
        Polygon();
        ~Polygon();
        /**
        if *own* is true Polygon will delete[] data array;
        */
        void setVertices(const void* data, qint32 size, qint32 bformat, bool own);
    protected:
        void doRender(const RenderState&) override;

    protected:
        const VertexDeclaration* m_vdecl;
        bool m_own;
        const unsigned char* m_verticesData;
        qint32 m_verticesSize;
    };
}
