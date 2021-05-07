#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"

namespace oxygine
{
    namespace operations
    {
        bool check(const ImageData& src, const ImageData& dest)
        {
            Q_ASSERT(dest.m_w == src.m_w);
            Q_ASSERT(dest.m_h == src.m_h);
            if (src.m_w)
            {
                Q_ASSERT(src.m_data);
                Q_ASSERT(dest.m_data);
                Q_ASSERT(src.m_pitch);
                Q_ASSERT(dest.m_pitch);
            }
            Q_ASSERT(src.m_bytespp);
            Q_ASSERT(dest.m_bytespp);
            if (dest.m_w != src.m_w ||
                    dest.m_h != src.m_h ||
                    !src.m_data ||
                    !dest.m_data ||
                    !src.m_pitch ||
                    !dest.m_pitch ||
                    !src.m_bytespp ||
                    !dest.m_bytespp)
            {
                return false;
            }
            return true;
        }

        void copy(const ImageData& src, const ImageData& dest)
        {
            if (!check(src, dest))
            {
                return;
            }
            Q_ASSERT(src.m_format == dest.m_format);

            qint32 bppPitch = src.m_w * src.m_bytespp;

            if (src.m_pitch == dest.m_pitch && bppPitch == dest.m_pitch)
            {
                memcpy(dest.m_data, src.m_data, bppPitch * src.m_h);
            }
            else
            {
                const unsigned char* srcLine = src.m_data;
                unsigned char* destLine = dest.m_data;

                const qint32 srch = src.m_h;
                const qint32 srcpitch = src.m_pitch;
                const qint32 destpitch = dest.m_pitch;
                for (qint32 h = 0; h < srch; h++)
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

        void blit(const ImageData& src, const ImageData& dest)
        {
            if (src.m_format == dest.m_format)
            {
                copy(src, dest);
                return;
            }

            op_blit op;
            applyOperation(op, src, dest);
        }
    }
}
