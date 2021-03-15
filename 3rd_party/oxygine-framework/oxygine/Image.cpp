#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{

    Image::Image(): _offset(0)
    {

    }

    Image::~Image()
    {

    }

    void Image::cleanup()
    {
        _buffer.clear();
        _image = ImageData();
    }

    void Image::convert(Image& dest, ImageData::TextureFormat format)
    {
        dest.init(getWidth(), getHeight(), format);
        ImageData src = lock();
        ImageData dst = dest.lock();

        operations::blit(src, dst);
    }

    void Image::fill(unsigned int val)
    {
        if (_buffer.empty())
            return;
        memset(&_buffer.front(), val, _buffer.size());
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
            int pitch = ImageData::getBytesPerPixel(ImageData::TF_R8G8B8A8) * buffer.width();
            ImageData src(buffer.width(), 1, pitch, ImageData::TF_R8G8B8A8,  nullptr);
            dest.h = 1;
            if (premultiplied)
            {
                operations::op_premultipliedAlpha op;
                for (qint32 i = 0; i < buffer.height(); i++)
                {
                    src.data = buffer.scanLine(i);
                    operations::applyOperation(op, src, dest);
                    dest.data += dest.pitch;
                }
            }
            else
            {
                operations::op_blit op;
                for (qint32 i = 0; i < buffer.height(); i++)
                {
                    src.data = buffer.scanLine(i);
                    operations::applyOperation(op, src, dest);
                    dest.data += dest.pitch;
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
        init(src.w, src.h, src.format);
        updateRegion(0, 0, src);
    }

    void Image::init(int w, int h, ImageData::TextureFormat Format)
    {
        int bytesPerPixel = ImageData::getBytesPerPixel(Format);
        int size = h * w * bytesPerPixel;
        _buffer.resize(size);
        _image = ImageData(w, h, w * bytesPerPixel, Format, size ? &_buffer.front() : 0);
    }



    int Image::getWidth() const
    {
        return _image.w;
    }

    int Image::getHeight() const
    {
        return _image.h;
    }

    const Point& Image::getSize() const
    {
        return *((Point*)&_image.w);
    }

    ImageData::TextureFormat Image::getFormat() const
    {
        return _image.format;
    }

    ImageData Image::lock(lock_flags, const Rect* pRect)
    {
        Rect rect(0, 0, _image.w, _image.h);
        if (pRect)
        {
            rect = *pRect;
            Q_ASSERT(rect.getX() >= 0);
            Q_ASSERT(rect.getY() >= 0);
            Q_ASSERT(rect.getX() + rect.getWidth() <= _image.w);
            Q_ASSERT(rect.getY() + rect.getHeight() <= _image.h);
        }

        ImageData im = _image;

        void* ptr = nullptr;
        if (!_buffer.empty())//zero size image
            ptr = &_buffer.front() + rect.getX() * _image.bytespp + rect.getY() * _image.pitch + _offset;

        return ImageData(rect.getWidth(), rect.getHeight(), _image.pitch, _image.format, ptr);
    }

    ImageData Image::lock(const Rect* pRect)
    {
        return lock(lock_read | lock_write, pRect);
    }

    ImageData Image::lock(const Rect& rect)
    {
        return lock(lock_read | lock_write, &rect);
    }

    ImageData Image::lock(int x, int y, int w, int h)
    {
        return lock(Rect(x, y, w, h));
    }

    ImageData Image::lock(int x, int y)
    {
        return lock(Rect(x, y, _image.w - x, _image.h - y));
    }

    void Image::unlock()
    {

    }

    void Image::toPOT(Image& dest)
    {
        Q_ASSERT(this != &dest);
        dest.init(nextPOT(_image.w), nextPOT(_image.h), _image.format);
        dest.fillZero();
        dest.updateRegion(0, 0, _image);
    }

    void Image::updateRegion(int x, int y, const ImageData& src)
    {
        Rect r(x, y, src.w, src.h);
        ImageData dest = lock(&r);
        operations::blit(src, dest);
        unlock();
    }

    void Image::apply(const Rect*)
    {

    }


    void Image::swap(Image& r)
    {
        ImageData copy = _image;
        _image = r._image;
        r._image = copy;

        std::swap(_buffer, r._buffer);
    }
}
