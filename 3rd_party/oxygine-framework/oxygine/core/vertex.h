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
        float x{0.0f};
        float y{0.0f};
        float z{0.0f};
        quint32 color{0};
        float u{0.0f};
        float v{0.0f};
    };
}
