#include "3rd_party/oxygine-framework/oxygine/core/ImageData.h"

namespace oxygine
{
    qint32 ImageData::getBytesPerPixel(TextureFormat tf)
    {
        switch (tf)
        {
            case TF_R8G8B8A8:
                return 4;
            case TF_UNDEFINED:
                return 0;
            default:
                Q_ASSERT(!"unknown type");
        }
        return 0;
    }

    ImageData::ImageData(): m_w(0), m_h(0), m_bytespp(0), m_pitch(0), m_data(0), m_format(TF_UNDEFINED)
    {

    }

    ImageData::ImageData(qint32 W, qint32 H, qint32 Pitch, TextureFormat Format, unsigned char* pData)
        : m_w(W),
          m_h(H),
          m_pitch(Pitch),
          m_data(pData),
          m_format(Format)
    {
        m_bytespp = getBytesPerPixel(Format);
    }

    ImageData::ImageData(const ImageData& b, unsigned char* pData)
        : ImageData(b.m_w, b.m_h, b.m_pitch, b.m_format, pData)
    {
    }

    ImageData::~ImageData()
    {
    }

    ImageData ImageData::getRect(const Rect& r) const
    {
        Q_ASSERT(r.getX() >= 0 && r.getX() <= m_w);
        Q_ASSERT(r.getY() >= 0 && r.getY() <= m_h);
        Q_ASSERT(r.getX() + r.getWidth() <= m_w);
        Q_ASSERT(r.getY() + r.getHeight() <= m_h);

        unsigned char* ptr = m_data + r.getX() * m_bytespp + r.getY() * m_pitch;
        ImageData buffer(r.getWidth(), r.getHeight(), m_pitch, m_format, ptr);

        return buffer;
    }

    ImageData ImageData::getRect(qint32 x, qint32 y, qint32 w, qint32 h) const
    {
        return getRect(Rect(x, y, w, h));
    }

    ImageData ImageData::getRect(qint32 x, qint32 y) const
    {
        return getRect(x, y, m_w - x, m_h - y);
    }

    unsigned char* ImageData::getPixelPtr(qint32 x, qint32 y) const
    {
        return (unsigned char*)m_data + x * m_bytespp + y * m_pitch;
    }
}
