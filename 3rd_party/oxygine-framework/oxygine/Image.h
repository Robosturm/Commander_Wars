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
        void init(qint32 w, qint32 h, ImageData::TextureFormat Format);

        void cleanup();

        void fillZero() { fill(0); }
        void fill(quint32 val);

        quint32    getSizeVRAM() const {return (unsigned int)m_buffer.size();}
        qint32             getWidth() const;
        qint32             getHeight() const;
        const Point&    getSize() const;
        ImageData::TextureFormat   getFormat() const;

        ImageData   lock(lock_flags f = 0, const Rect* pRect = 0);
        ImageData   lock(const Rect* pRect);
        ImageData   lock(const Rect& pRect);
        ImageData   lock(qint32 x, qint32 y, qint32 w, qint32 h);
        ImageData   lock(qint32 x, qint32 y);

        void        unlock();
        void        toPOT(Image& dest);

        void        updateRegion(qint32 x, qint32 y, const ImageData& data);
        void        apply(const Rect*);

        void        swap(Image& r);

    private:
        ImageData m_image;
        size_t m_offset;//buffer offset
        QVector<unsigned char> m_buffer;
    };
}
