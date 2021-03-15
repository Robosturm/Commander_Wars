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
        ImageData(int W, int H, int Pitch, TextureFormat Format, void* Data = nullptr);
        ImageData(const ImageData& b, void* Data);
        ~ImageData();

        ImageData getRect(const Rect& r) const;
        ImageData getRect(int x, int y, int w, int h) const;
        ImageData getRect(int x, int y) const;
        unsigned char* getPixelPtr(int x, int y) const;

        static int getBytesPerPixel(TextureFormat tf);
    public:
        int w;
        int h;
        int bytespp;
        int pitch;

        unsigned char* data;
        TextureFormat format;
    };
}
