#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

namespace oxygine
{
    struct VertexPCT2
    {
        enum Fields
        {
            Position,
            Color,
            Uv,
            Max,
            VERTEX_TEXCOORD_SIZE = 2,
        };
        float x, y, z;
        quint32 color;
        float u, v;
    };
}
