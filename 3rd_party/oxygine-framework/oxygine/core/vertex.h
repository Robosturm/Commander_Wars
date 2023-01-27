#pragma once

#include <QtGlobal>
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
        qint32 x{0};
        qint32 y{0};
        qint32 z{0};
        quint32 color{0};
        float u{0.0f};
        float v{0.0f};
    };
}
