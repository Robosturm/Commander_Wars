#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"

#include <qimage.h>

namespace oxygine
{
    class Image;
    using spImage = intrusive_ptr<Image>;
    class Image : public Texture
    {
    public:
        static qint32 HIT_TEST_DOWNSCALE;
        static const qint32 ALIGN = sizeof(int32_t);
        static const qint32 BITS = ALIGN * 8;

        explicit Image() = default;
        virtual ~Image() = default;
        bool init(QImage bf, bool premultiplied);
        void init(const ImageData& src);
        void init(qint32 w, qint32 h, ImageData::TextureFormat Format);
        void cleanup();
        void fillZero()
        {
            fill(0);
        }
        void fill(quint32 val);
        qint32 getWidth() const;
        qint32 getHeight() const;
        const Point& getSize() const;
        ImageData::TextureFormat   getFormat() const;
        ImageData lock(lock_flags f = 0, const Rect* pRect = 0);
        ImageData lock(const Rect* pRect);
        ImageData lock(const Rect& pRect);
        ImageData lock(qint32 x, qint32 y, qint32 w, qint32 h);
        ImageData lock(qint32 x, qint32 y);
        void unlock();
        void updateRegion(qint32 x, qint32 y, const ImageData& data);
        void apply(const Rect*);
        void swap(Image& r);
        static void makeAlpha(const ImageData& srcImage, Rect& bounds, QVector<unsigned char>& alpha, HitTestData& adata, bool hittest);
    private:
        ImageData m_image;
        size_t m_offset{0};//buffer offset
        QVector<unsigned char> m_buffer;
    };
}
