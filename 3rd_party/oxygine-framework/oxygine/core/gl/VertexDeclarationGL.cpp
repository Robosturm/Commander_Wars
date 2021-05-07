#include "3rd_party/oxygine-framework/oxygine/core/gl/VertexDeclarationGL.h"

namespace oxygine
{
    void VertexDeclarationGL::init(bvertex_format fmt)
    {
        bformat = fmt;
        qint32 i = 0;
        qint32 offset = 0;
        Element* dest = m_elements;
        if (fmt & VERTEX_POSITION)
        {
            dest->name = "position";
            dest->elemType = GL_FLOAT;
            dest->size = 3;
            dest->offset = offset;
            dest->index = i;
            dest->normalized = false;

            offset += sizeof(float) * 3;
            ++i;
            ++dest;
        }

        if (fmt & VERTEX_COLOR)
        {
            dest->name = "color";
            dest->elemType = GL_UNSIGNED_BYTE;
            dest->size = 4;
            dest->offset = offset;
            dest->index = i;
            dest->normalized = true;

            offset += 4;
            ++i;
            ++dest;
        }

        qint32 numTexCoords = numTextureCoordinates(fmt);
        for (qint32 j = 0; j < numTexCoords; ++j)
        {
            qint32 coordSize = texCoordSize(j, fmt);
            if (j == 0)
            {
                dest->name = "uv";
            }
            else
            {
                dest->name = "uv" + QString::number(j + 1);
            }

            dest->elemType = GL_FLOAT;
            dest->size = coordSize;
            dest->offset = offset;
            dest->index = i;
            dest->normalized = false;

            offset += sizeof(float) * coordSize;
            ++i;
            ++dest;
        }

        qint32 userSize = userDataSize(fmt);
        if (userSize > 0)
        {
            dest->elemType = GL_FLOAT;
            dest->size = userSize;
            dest->offset = offset;
            dest->index = i;
            dest->normalized = false;

            offset += sizeof(float) * userSize;
            ++i;
            ++dest;
        }

        numElements = i;
        size = offset;
    }
}
