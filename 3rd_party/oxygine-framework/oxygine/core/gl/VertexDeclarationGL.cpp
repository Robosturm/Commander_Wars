#include "3rd_party/oxygine-framework/oxygine/core/gl/VertexDeclarationGL.h"

namespace oxygine
{
    void VertexDeclarationGL::init(bvertex_format fmt)
    {
        bformat = fmt;
        int i = 0;
        int offset = 0;
        Element* dest = elements;
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

        int numTexCoords = numTextureCoordinates(fmt);
        for (int j = 0; j < numTexCoords; ++j)
        {
            int coordSize = texCoordSize(j, fmt);
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

        int userSize = userDataSize(fmt);
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
