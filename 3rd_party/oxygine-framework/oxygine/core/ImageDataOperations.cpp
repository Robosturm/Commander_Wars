#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"

namespace oxygine
{
    namespace operations
    {
        bool check(const ImageData& src, const ImageData& dest)
        {
            Q_ASSERT(dest.w == src.w);
            Q_ASSERT(dest.h == src.h);
            if (src.w)
            {
                Q_ASSERT(src.data);
                Q_ASSERT(dest.data);
                Q_ASSERT(src.pitch);
                Q_ASSERT(dest.pitch);
            }
            Q_ASSERT(src.bytespp);
            Q_ASSERT(dest.bytespp);
            if (dest.w != src.w ||
                    dest.h != src.h ||
                    !src.data ||
                    !dest.data ||
                    !src.pitch ||
                    !dest.pitch ||
                    !src.bytespp ||
                    !dest.bytespp)
                return false;

            return true;
        }

        void copy(const ImageData& src, const ImageData& dest)
        {
            if (!check(src, dest))
                return;

            Q_ASSERT(src.format == dest.format);

            int bppPitch = src.w * src.bytespp;

            if (src.pitch == dest.pitch && bppPitch == dest.pitch)
                memcpy(dest.data, src.data, bppPitch * src.h);
            else
            {
                const unsigned char* srcLine = src.data;
                unsigned char* destLine = dest.data;

                const int srch = src.h;
                const int srcpitch = src.pitch;
                const int destpitch = dest.pitch;
                for (int h = 0; h < srch; h++)
                {
                    if (bppPitch >= 0)
                    {
                        memcpy(destLine, srcLine, static_cast<size_t>(bppPitch));
                    }
                    srcLine += srcpitch;
                    destLine += destpitch;
                }
            }
        }

        void move(const ImageData& src, const ImageData& dest)
        {
            if (!check(src, dest))
                return;

            Q_ASSERT(src.format == dest.format);

            int bppPitch = src.w * src.bytespp;

            if (src.pitch == dest.pitch && bppPitch == dest.pitch)
                memmove(dest.data, src.data, bppPitch * src.h);
            else
            {
                const unsigned char* srcLine = src.data;
                unsigned char* destLine = dest.data;

                const int srch = src.h;
                const int srcpitch = src.pitch;
                const int destpitch = dest.pitch;
                for (int h = 0; h < srch; h++)
                {
                    memmove(destLine, srcLine, bppPitch);
                    srcLine += srcpitch;
                    destLine += destpitch;
                }
            }
        }

        void blit(const ImageData& src, const ImageData& dest)
        {
            if (src.format == dest.format)
            {
                copy(src, dest);
                return;
            }

            op_blit op;
            applyOperation(op, src, dest);
        }

        void blitPremultiply(const ImageData& src, const ImageData& dest)
        {
            op_premultipliedAlpha op;
            applyOperation(op, src, dest);
        }

        void premultiply(const ImageData& dest)
        {
            blitPremultiply(dest, dest);
        }

        void blitColored(const ImageData& src, const ImageData& dest, const QColor& c)
        {
            Pixel p;
            p.r = c.red();
            p.g = c.green();
            p.b = c.blue();
            p.a = c.alpha();

            op_blit_colored op(p);
            applyOperation(op, src, dest);
        }

        void flipY(const ImageData& src, const ImageData& dest)
        {
            if (!check(src, dest))
                return;

            const unsigned char* srcLine = src.data;
            unsigned char* destLine = dest.data + dest.pitch * dest.h - dest.pitch;

            int bppPitch = src.w * src.bytespp;



            const int srch = src.h;
            const int srcpitch = src.pitch;
            const int destpitch = dest.pitch;
            for (int h = 0; h < srch; h++)
            {
                memcpy(destLine, srcLine, bppPitch);
                srcLine += srcpitch;
                destLine -= destpitch;
            }
        }

        void blend(const ImageData& src, const ImageData& dest)
        {
            op_blend_srcAlpha_invSrcAlpha op;
            applyOperation(op, src, dest);
        }
    }
}
