#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"

namespace oxygine
{
    namespace operations
    {
        bool check(const ImageData& src, const ImageData& dest)
        {
            if (dest.m_w != src.m_w  ||
                dest.m_h != src.m_h)
            {
                oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "ImageDataOperations::check size missmatch");
            }
            if (src.m_w)
            {
                if (src.m_data == 0 ||
                    dest.m_data == 0 ||
                    src.m_pitch == 0 ||
                    dest.m_pitch == 0)
                {
                    oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "ImageDataOperations::check unset image data");
                }
            }
            if (src.m_bytespp == 0 ||
                dest.m_bytespp == 0)
            {
                oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "ImageDataOperations::check bytespp unset");
            }
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
            if (src.m_format != dest.m_format)
            {
                oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "ImageDataOperations::copy format error");
            }

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
