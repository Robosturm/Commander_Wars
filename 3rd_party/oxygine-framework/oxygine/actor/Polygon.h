#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    class VertexDeclaration;
    class Polygon;
    using spPolygon = intrusive_ptr<Polygon>;
    class Polygon : public Sprite
    {
    public:
        explicit Polygon();
        virtual ~Polygon();
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
