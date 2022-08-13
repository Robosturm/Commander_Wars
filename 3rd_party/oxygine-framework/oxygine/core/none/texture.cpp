#include "texture.h"

namespace oxygine
{
    Texture::~Texture()
    {
        Texture::release();
    }

    void Texture::setLinearFilter(quint32 filter)
    {
    }

    void Texture::setClamp2Edge(bool clamp2edge)
    {
    }

    quint32 Texture::getHighestTextureCount()
    {
        return 0;
    }

    void Texture::release()
    {
    }

    void Texture::init(const QImage &)
    {
    }
}
