#include "3rd_party/oxygine-framework/oxygine/core/ImageData.h"

namespace oxygine
{
    int ImageData::getBytesPerPixel(TextureFormat tf)
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

    ImageData::ImageData(): w(0), h(0), bytespp(0), pitch(0), data(0), format(TF_UNDEFINED)
    {

    }

    ImageData::ImageData(int W, int H, int Pitch, TextureFormat Format, void* Data): w(W), h(H), pitch(Pitch), format(Format), data((unsigned char*)Data)
    {
        bytespp = getBytesPerPixel(Format);
    }

    ImageData::ImageData(const ImageData& b, void* Data)
    {
        *this = ImageData(b.w, b.h, b.pitch, b.format, Data);
    }

    ImageData::~ImageData()
    {
    }

    ImageData ImageData::getRect(const Rect& r) const
    {
        Q_ASSERT(r.getX() >= 0 && r.getX() <= w);
        Q_ASSERT(r.getY() >= 0 && r.getY() <= h);
        Q_ASSERT(r.getX() + r.getWidth() <= w);
        Q_ASSERT(r.getY() + r.getHeight() <= h);

        void* ptr = (unsigned char*)data + r.getX() * bytespp + r.getY() * pitch;
        ImageData buffer(r.getWidth(), r.getHeight(), pitch, format, ptr);

        return buffer;
    }

    ImageData ImageData::getRect(int x, int y, int w, int h) const
    {
        return getRect(Rect(x, y, w, h));
    }

    ImageData ImageData::getRect(int x, int y) const
    {
        return getRect(x, y, w - x, h - y);
    }

    unsigned char* ImageData::getPixelPtr(int x, int y) const
    {
        return (unsigned char*)data + x * bytespp + y * pitch;
    }
}
