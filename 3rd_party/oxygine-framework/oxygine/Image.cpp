#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include "coreengine/globalutils.h"

namespace oxygine
{
    qint32 Image::HIT_TEST_DOWNSCALE = 4;

    void Image::cleanup()
    {
        m_buffer.clear();
        m_image = ImageData();
    }

    void Image::fill(quint32 val)
    {
        if (m_buffer.empty())
        {
            return;
        }
        memset(&m_buffer.front(), val, m_buffer.size());
    }

    bool Image::init(QImage buffer, bool premultiplied)
    {
        cleanup();

        if (buffer.width() > 0 && buffer.height() > 0)
        {
            buffer = buffer.convertToFormat(QImage::Format::Format_RGBA8888);
            // resize image data and imahe
            init(buffer.width(), buffer.height(), ImageData::TF_R8G8B8A8);
            ImageData dest = lock();
            qint32 pitch = ImageData::getBytesPerPixel(ImageData::TF_R8G8B8A8) * buffer.width();
            ImageData src(buffer.width(), 1, pitch, ImageData::TF_R8G8B8A8,  nullptr);
            dest.m_h = 1;
            if (premultiplied)
            {
                operations::op_premultipliedAlpha op;
                for (qint32 i = 0; i < buffer.height(); i++)
                {
                    src.m_data = buffer.scanLine(i);
                    operations::applyOperation(op, src, dest);
                    dest.m_data += dest.m_pitch;
                }
            }
            else
            {
                operations::op_blit op;
                for (qint32 i = 0; i < buffer.height(); i++)
                {
                    src.m_data = buffer.scanLine(i);
                    operations::applyOperation(op, src, dest);
                    dest.m_data += dest.m_pitch;
                }
            }
        }
        else
        {
            qWarning("Image. can't unpack data unknown file format");
            init(16, 16, ImageData::TF_R8G8B8A8);
            fillZero();
        }
        return false;
    }

    void Image::init(const ImageData& src)
    {
        init(src.m_w, src.m_h, src.m_format);
        updateRegion(0, 0, src);
    }

    void Image::init(qint32 w, qint32 h, ImageData::TextureFormat Format)
    {
        qint32 bytesPerPixel = ImageData::getBytesPerPixel(Format);
        qint32 size = h * w * bytesPerPixel;
        m_buffer.resize(size);
        m_image = ImageData(w, h, w * bytesPerPixel, Format, size ? &m_buffer.front() : 0);
    }



    qint32 Image::getWidth() const
    {
        return m_image.m_w;
    }

    qint32 Image::getHeight() const
    {
        return m_image.m_h;
    }

    const Point& Image::getSize() const
    {
        return *((Point*)&m_image.m_w);
    }

    ImageData::TextureFormat Image::getFormat() const
    {
        return m_image.m_format;
    }

    ImageData Image::lock(lock_flags, const Rect* pRect)
    {
        Rect rect(0, 0, m_image.m_w, m_image.m_h);
        if (pRect)
        {
            rect = *pRect;
            if (rect.getX() < 0 ||
                rect.getY() < 0 ||
                rect.getX() + rect.getWidth() > m_image.m_w ||
                rect.getY() + rect.getHeight() > m_image.m_h)
            {
                handleErrorPolicy(ep_show_error, "Image::lock border error");
            }
        }

        ImageData im = m_image;

        unsigned char* ptr = nullptr;
        if (!m_buffer.empty())//zero size image
        {
            ptr = &m_buffer.front() + rect.getX() * m_image.m_bytespp + rect.getY() * m_image.m_pitch + m_offset;
        }
        return ImageData(rect.getWidth(), rect.getHeight(), m_image.m_pitch, m_image.m_format, ptr);
    }

    ImageData Image::lock(const Rect* pRect)
    {
        return lock(lock_read | lock_write, pRect);
    }

    ImageData Image::lock(const Rect& rect)
    {
        return lock(lock_read | lock_write, &rect);
    }

    ImageData Image::lock(qint32 x, qint32 y, qint32 w, qint32 h)
    {
        return lock(Rect(x, y, w, h));
    }

    ImageData Image::lock(qint32 x, qint32 y)
    {
        return lock(Rect(x, y, m_image.m_w - x, m_image.m_h - y));
    }

    void Image::unlock()
    {

    }

    void Image::updateRegion(qint32 x, qint32 y, const ImageData& src)
    {
        Rect r(x, y, src.m_w, src.m_h);
        ImageData dest = lock(&r);
        operations::blit(src, dest);
        unlock();
    }

    void Image::apply(const Rect*)
    {

    }

    void Image::swap(Image& r)
    {
        ImageData copy = m_image;
        m_image = r.m_image;
        r.m_image = copy;

        std::swap(m_buffer, r.m_buffer);
    }

    void Image::makeAlpha(const ImageData& srcImage, Rect& bounds, QVector<unsigned char>& alpha, HitTestData& adata, bool hittest)
    {
        qint32 w = srcImage.m_w;
        qint32 h = srcImage.m_h;

        size_t pos = alpha.size();
        adata.data = reinterpret_cast<unsigned char*>(pos);
        adata.w = GlobalUtils::roundUpInt(w, HIT_TEST_DOWNSCALE) / HIT_TEST_DOWNSCALE;
        adata.h = GlobalUtils::roundUpInt(h, HIT_TEST_DOWNSCALE) / HIT_TEST_DOWNSCALE;

        qint32 lineInts = GlobalUtils::roundUpInt(adata.w, BITS) / BITS;

        qint32 destPitch = lineInts * ALIGN;

        qint32 size = adata.h * destPitch;

        alpha.resize(pos + size + 10);


        const unsigned char* srcData = srcImage.m_data;
        qint32 srcStep = srcImage.m_bytespp;
        qint32 srcPitch = srcImage.m_pitch;

        unsigned char* destData = &alpha[pos];

        adata.pitch = destPitch;

        const unsigned char* srcRow = srcData;
        unsigned char* destRow = destData;


        qint32 minX = w;
        qint32 minY = h;
        qint32 maxX = 0;
        qint32 maxY = 0;

        bool hasAlpha = false;

        for (qint32 y = 0; y != h; y += 1)
        {
            const unsigned char* srcLine = srcRow;
            int32_t* destLine = reinterpret_cast<int32_t*>(destRow);

            bool lineWithAlpha = false;


            for (qint32 x = 0; x != w; x += 1)
            {
                PixelR8G8B8A8 pd;
                Pixel p;
                pd.getPixel(srcLine, p);
                if (p.a > 5)
                {
                    hasAlpha = true;

                    qint32 dx = x / HIT_TEST_DOWNSCALE;
                    qint32 n = dx / BITS;
                    qint32 b = dx % BITS;

                    destLine[n] |= 1 << b;

                    lineWithAlpha = true;
                    if (x > maxX)
                    {
                        maxX = x;
                    }
                    else if (x < minX)
                    {
                        minX = x;
                    }
                }
                srcLine += srcStep;
            }

            if (lineWithAlpha)
            {
                if (minY == h)
                    minY = y;
                maxY = y;
            }

            if (y % HIT_TEST_DOWNSCALE == HIT_TEST_DOWNSCALE - 1)
            {
                //reset line
                destRow += destPitch;
            }

            srcRow += srcPitch;
        }

        //if image is transparent
        if (minX == w && maxX == 0)
        {
            minX = 0;
            maxX = 0;
        }

        if (minY == h && maxY == 0)
        {
            minY = 0;
            maxY = 0;
        }

        bounds = Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);

        if (!hasAlpha || !hittest)
        {
            alpha.resize(pos);
            adata = HitTestData();
        }
    }
}
