#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageData.h"

namespace oxygine
{
    namespace operations
    {
        //based on memcpy
        void copy(const ImageData& src, const ImageData& dest);
        void blit(const ImageData& src, const ImageData& dest);

        template <class Op>
        void applyOperation(const Op& op, const ImageData& src, const ImageData& dest);

        class op_fill
        {
        public:
            op_fill() {color.rgba = 0xffffffff;}

            Pixel color;

            template<class Src, class Dest>
            void operator()(const Src&, Dest& d, const unsigned char*, unsigned char* destData) const
            {
                d.setPixel(destData, color);
            }
        };

        class op_premultipliedAlpha
        {
        public:
            template<class Src, class Dest>
            void operator()(const Src& srcPixelFormat, Dest& destPixelFormat, const unsigned char* srcData, unsigned char* destData) const
            {
                Pixel p;
                srcPixelFormat.getPixel(srcData, p);

                //we need correct "snapped" to pixel format alpha
                unsigned char na = destPixelFormat.snap_a(p.a);

                p.r = (p.r * na) / 255;
                p.g = (p.g * na) / 255;
                p.b = (p.b * na) / 255;

                destPixelFormat.setPixel(destData, p);
            }
        };

        class op_blit
        {
        public:
            template<class Src, class Dest>
            void operator()(const Src& srcPixelFormat, Dest& destPixelFormat, const unsigned char* srcData, unsigned char* destData) const
            {
                Pixel p;
                srcPixelFormat.getPixel(srcData, p);
                destPixelFormat.setPixel(destData, p);
            }
        };

        bool check(const ImageData& src, const ImageData& dest);

        template <class Op, class Src, class Dest>
        void applyOperationT(const Op& op, const Src& srcPixelFormat, Dest& destPixelFormat, const ImageData& src, const ImageData& dest)
        {
            if (!check(src, dest))
            {
                return;
            }
            const unsigned char* srcBuffer = (unsigned char*)src.m_data;
            unsigned char* destBuffer = (unsigned char*)dest.m_data;

            qint32 w = dest.m_w;
            qint32 h = dest.m_h;

            for (qint32 y = 0; y != h; ++y)
            {
                const unsigned char* srcLine = srcBuffer;
                unsigned char* destLine = destBuffer;

                for (qint32 x = 0; x != w; ++x)
                {
                    op(srcPixelFormat, destPixelFormat, srcLine, destLine);

                    destLine += dest.m_bytespp;
                    srcLine += src.m_bytespp;
                }

                srcBuffer += src.m_pitch;
                destBuffer += dest.m_pitch;
            }
        }


        template <class Op, class Dest>
        void applyOperationT(const Op& op, Dest& destPixelFormat, const ImageData& dest)
        {
            if (!check(dest, dest))
            {
                return;
            }

            unsigned char* destBuffer = (unsigned char*)dest.m_data;

            qint32 w = dest.m_w;
            qint32 h = dest.m_h;

            for (qint32 y = 0; y != h; ++y)
            {
                unsigned char* destLine = destBuffer;

                for (qint32 x = 0; x != w; ++x)
                {
                    op(destPixelFormat, destPixelFormat, destLine, destLine);
                    destLine += dest.m_bytespp;
                }

                destBuffer += dest.m_pitch;
            }
        }


        template<class Src, class Op>
        void SwitchSrcDestT(const Op& op, const Src& s, const ImageData& src, const ImageData& dest)
        {
            switch(dest.m_format)
            {
                case ImageData::TF_R8G8B8A8:
                {
                    PixelR8G8B8A8 d;
                    applyOperationT(op, s, d, src, dest);
                }
                    break;
                default:
                    Q_ASSERT(!"unknown format");
            }
        }

        template <class Op>
        void applyOperation(const Op& op, const ImageData& src, const ImageData& dest)
        {
            switch(dest.m_format)
            {
                case ImageData::TF_R8G8B8A8:
                {
                    PixelR8G8B8A8 s;
                    SwitchSrcDestT(op, s, src, dest);
                }
                    break;
                default:
                    Q_ASSERT(!"unknown format");
            }
        }

        template <class Op>
        void applyOperation(const Op& op, const ImageData& dest)
        {
            switch(dest.m_format)
            {
                case ImageData::TF_R8G8B8A8:
                {
                    PixelR8G8B8A8 d;
                    applyOperationT(op, d, dest);
                    break;
                }
                default:
                    Q_ASSERT(!"unknown format");
            }
        }
    }
}
