#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include "3rd_party/oxygine-framework/oxygine/core/pixel.h"

namespace oxygine
{
    class ImageData
    {
    public:
        enum TextureFormat
        {
            TF_UNDEFINED,
            TF_R8G8B8A8,//default
        };
        ImageData();
        ImageData(qint32 W, qint32 H, qint32 Pitch, TextureFormat Format, unsigned char* Data = nullptr);
        ImageData(const ImageData& b, unsigned char* Data);
        ~ImageData();

        ImageData getRect(const Rect& r) const;
        ImageData getRect(qint32 x, qint32 y, qint32 w, qint32 h) const;
        ImageData getRect(qint32 x, qint32 y) const;
        unsigned char* getPixelPtr(qint32 x, qint32 y) const;
        static qint32 getBytesPerPixel(TextureFormat tf);
    public:
        qint32 m_w;
        qint32 m_h;
        qint32 m_bytespp;
        qint32 m_pitch;

        unsigned char* m_data;
        TextureFormat m_format;
    };
}
