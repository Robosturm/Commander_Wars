#pragma once
#ifndef OXYGINE_VERTEX
#define OXYGINE_VERTEX

#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/math/Vector2.h"

namespace oxygine
{
    enum VertexFormatFlags
    {
        VERTEX_POSITION = 0x0001,
        VERTEX_COLOR    = 0x0002,

        VERTEX_LAST_BIT = 8,

        USER_DATA_SIZE_BIT = VERTEX_LAST_BIT + 4,
        NUM_TEX_COORD_BIT = VERTEX_LAST_BIT + 7,
        TEX_COORD_SIZE_BIT = VERTEX_LAST_BIT + 10,

        USER_DATA_SIZE_MASK = (0x7 << USER_DATA_SIZE_BIT),
        NUM_TEX_COORD_MASK = (0x7 << NUM_TEX_COORD_BIT),
        TEX_COORD_SIZE_MASK = (0x7 << TEX_COORD_SIZE_BIT),
    };
    /**
     * @brief VERTEX_USERDATA_SIZE
     * @param _n
     * @return
     */
    static constexpr qint32 VERTEX_USERDATA_SIZE(qint32 _n)
    {
        return ((_n) << USER_DATA_SIZE_BIT);
    }
    /**
     * @brief VERTEX_NUM_TEXCOORDS
     * @param _n
     * @return
     */
    static constexpr qint32 VERTEX_NUM_TEXCOORDS(qint32 _n)
    {
        return ((_n) << NUM_TEX_COORD_BIT);
    }
    /**
     * @brief VERTEX_TEXCOORD_SIZE
     * @param _stage
     * @param _n
     * @return
     */
    static constexpr qint32 VERTEX_TEXCOORD_SIZE(qint32 _stage ,qint32 _n)
    {
        return ((_n) << (TEX_COORD_SIZE_BIT + (3 * _stage)));
    }

    typedef quint32 bvertex_format;

    inline qint32 vertexFlags(bvertex_format vertexFormat)
    {
        return vertexFormat & ((1 << (VERTEX_LAST_BIT + 1)) - 1);
    }

    inline qint32 numTextureCoordinates(bvertex_format vertexFormat)
    {
        return (vertexFormat >> NUM_TEX_COORD_BIT) & 0x7;
    }

    inline qint32 userDataSize(bvertex_format vertexFormat)
    {
        return (vertexFormat >> USER_DATA_SIZE_BIT) & 0x7;
    }

    inline qint32 texCoordSize(qint32 stage, bvertex_format vertexFormat)
    {
        return (vertexFormat >> (TEX_COORD_SIZE_BIT + 3 * stage)) & 0x7;
    }



    inline quint32 getVertexSize(bvertex_format fmt)
    {
        qint32 offset = 0;

        if (fmt & VERTEX_POSITION)
        {
            offset += sizeof(float) * 3;
        }

        if (fmt & VERTEX_COLOR)
        {
            offset += 4;
        }

        qint32 numTexCoords = numTextureCoordinates(fmt);
        for (qint32 j = 0; j < numTexCoords; ++j)
        {
            qint32 coordSize = texCoordSize(j, fmt);
            offset += sizeof(float) * coordSize;
        }

        qint32 ds = userDataSize(fmt);
        if (ds > 0)
        {
            offset += sizeof(float) * ds;
        }
        return offset;
    }

    struct vertexPT2
    {
        enum { FORMAT = VERTEX_POSITION | VERTEX_NUM_TEXCOORDS(1) | VERTEX_TEXCOORD_SIZE(0, 2) };
        float x, y, z;
        float u, v;
    };

    struct vertexP2C
    {
        float x, y;
        quint32 color;

        Vector2& getPos() {return *((Vector2*)&x);}
    };

    struct vertexPCT2
    {
        enum { FORMAT = VERTEX_POSITION | VERTEX_COLOR | VERTEX_NUM_TEXCOORDS(1) | VERTEX_TEXCOORD_SIZE(0, 2) };
        float x, y, z;
        quint32 color;
        float u, v;
    };

    struct vertexPCT2T2 : public vertexPCT2
    {
        enum { FORMAT = VERTEX_POSITION | VERTEX_COLOR | VERTEX_NUM_TEXCOORDS(2) | VERTEX_TEXCOORD_SIZE(0, 2) | VERTEX_TEXCOORD_SIZE(1, 2) };
        float u2, v2;
    };

    struct vertexPCT2T2T2 : public vertexPCT2T2
    {
        enum { FORMAT = VERTEX_POSITION | VERTEX_COLOR | VERTEX_NUM_TEXCOORDS(3) | VERTEX_TEXCOORD_SIZE(0, 2) | VERTEX_TEXCOORD_SIZE(1, 2) | VERTEX_TEXCOORD_SIZE(2, 2) };
        float u3, v3;
    };
}
#endif // OXYGINE_VERTEX
