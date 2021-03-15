#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"

namespace oxygine
{    
    struct Pixel
    {
        union
        {
            struct
            {
                unsigned char bytes[4];
            };

            struct
            {
                unsigned char r, g, b, a;
            };

            unsigned int rgba;
        };
    };

    inline Pixel initPixel(unsigned int rgba)
    {
        Pixel p;
        p.rgba = rgba;
        return p;
    }

    inline Pixel initPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        Pixel p;
        p.r = r;
        p.g = g;
        p.b = b;
        p.a = a;
        return p;
    }

    class PixelR8G8B8A8
    {
        /*
        in memory: R8 G8 B8 A8
        in dword: A8 B8 G8 R8
        */

    public:
        void getPixel(const unsigned char* data, Pixel& p) const
        {
            p.r = data[0];
            p.g = data[1];
            p.b = data[2];
            p.a = data[3];
        }

        void setPixel(unsigned char* data, const Pixel& p)
        {
            data[0] = p.r;
            data[1] = p.g;
            data[2] = p.b;
            data[3] = p.a;
        }

        void copy(const unsigned char* src, unsigned char* dst)
        {
            *((unsigned int*)dst) = *((unsigned int*)src);
        }

        unsigned char snap_a(unsigned char alpha) const
        {
            return alpha;
        }
    };
}
