#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{

    Image::Image(): m_offset(0)
    {

    }

    Image::~Image()
    {

    }

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
            Q_ASSERT(rect.getX() >= 0);
            Q_ASSERT(rect.getY() >= 0);
            Q_ASSERT(rect.getX() + rect.getWidth() <= m_image.m_w);
            Q_ASSERT(rect.getY() + rect.getHeight() <= m_image.m_h);
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

    void Image::toPOT(Image& dest)
    {
        Q_ASSERT(this != &dest);
        dest.init(nextPOT(m_image.m_w), nextPOT(m_image.m_h), m_image.m_format);
        dest.fillZero();
        dest.updateRegion(0, 0, m_image);
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
}
