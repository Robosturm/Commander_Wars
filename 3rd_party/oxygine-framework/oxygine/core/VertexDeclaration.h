#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/vertex.h"
#include <qopengl.h>
#include <QString>

namespace oxygine
{
    class VertexDeclaration
    {
    public:
        struct Element
        {
            QString name;
            qint32 index;
            bool normalized;
            GLint size;
            GLenum elemType;
            qint32 offset;
        };

        VertexDeclaration()
            : size(0)
        {
            qint32 offset = 0;
            m_elements[VertexPCT2::Fields::Position].name = "position";
            m_elements[VertexPCT2::Fields::Position].elemType = GL_FLOAT;
            m_elements[VertexPCT2::Fields::Position].size = 3;
            m_elements[VertexPCT2::Fields::Position].offset = offset;
            m_elements[VertexPCT2::Fields::Position].index = VertexPCT2::Fields::Position;
            m_elements[VertexPCT2::Fields::Position].normalized = false;
            offset += sizeof(float) * 3;

            m_elements[VertexPCT2::Fields::Color].name = "color";
            m_elements[VertexPCT2::Fields::Color].elemType = GL_UNSIGNED_BYTE;
            m_elements[VertexPCT2::Fields::Color].size = 4;
            m_elements[VertexPCT2::Fields::Color].offset = offset;
            m_elements[VertexPCT2::Fields::Color].index = VertexPCT2::Fields::Color;
            m_elements[VertexPCT2::Fields::Color].normalized = true;
            offset += 4;

            m_elements[VertexPCT2::Fields::Uv].name = "uv";
            m_elements[VertexPCT2::Fields::Uv].elemType = GL_FLOAT;
            m_elements[VertexPCT2::Fields::Uv].size = VertexPCT2::Fields::VERTEX_TEXCOORD_SIZE;
            m_elements[VertexPCT2::Fields::Uv].offset = offset;
            m_elements[VertexPCT2::Fields::Uv].index = VertexPCT2::Fields::Uv;
            m_elements[VertexPCT2::Fields::Uv].normalized = false;
            offset += sizeof(float) * VertexPCT2::Fields::VERTEX_TEXCOORD_SIZE;

            size = offset;
        }
        qint32 size;
        Element m_elements[VertexPCT2::Fields::Max];
    };
}
