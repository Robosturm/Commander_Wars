#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"

#include <qimage.h>

namespace oxygine
{
    DECLARE_SMART(Image, spImage);

    class Image : public Texture
    {
    public:
        Image();
        ~Image();

        bool init(QImage bf, bool premultiplied);
        void init(const ImageData& src);
        void init(int w, int h, ImageData::TextureFormat Format);

        void cleanup();
        void convert(Image& dest, ImageData::TextureFormat format);

        void fillZero() { fill(0); }
        void fill(unsigned int val);

        unsigned int    getSizeVRAM() const {return (unsigned int)_buffer.size();}
        int             getWidth() const;
        int             getHeight() const;
        const Point&    getSize() const;
        ImageData::TextureFormat   getFormat() const;

        ImageData   lock(lock_flags f = 0, const Rect* pRect = 0);
        ImageData   lock(const Rect* pRect);
        ImageData   lock(const Rect& pRect);
        ImageData   lock(int x, int y, int w, int h);
        ImageData   lock(int x, int y);

        void        unlock();
        void        toPOT(Image& dest);

        void        updateRegion(int x, int y, const ImageData& data);
        void        apply(const Rect*);

        void        swap(Image& r);

    private:
        ImageData _image;
        size_t _offset;//buffer offset
        QVector<unsigned char> _buffer;
    };
}
