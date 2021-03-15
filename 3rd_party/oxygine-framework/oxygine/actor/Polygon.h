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
        void setVertices(const void* data, int size, int bformat, bool own);
    protected:
        void doRender(const RenderState&) override;

        const VertexDeclaration* _vdecl;
        bool _own;
        const unsigned char* _verticesData;
        int _verticesSize;
    };
}
