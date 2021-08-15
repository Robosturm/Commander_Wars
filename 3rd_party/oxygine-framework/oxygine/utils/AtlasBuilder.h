#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include <qqueue.h>

namespace oxygine
{
    class Texture;
    using spTexture = intrusive_ptr<Texture>;
    class AtlasBuilder
    {
    public:
        explicit AtlasBuilder();
        virtual ~AtlasBuilder() = default;
        void init(qint32 w, qint32 h, qint32 skipSize = 3);
        void clean();
        bool add(Image* dest, const ImageData& src, Rect& srcRect, const Point& offset);
        const Rect& getBounds() const
        {
            return m_bounds;
        }

    protected:
        using rects = QQueue<Rect>;
        Rect m_bounds;
        rects m_free;
        qint32 m_skipSize;
    };

    class MultiAtlas
    {
    public:
        using createTextureCallback = Closure<spTexture, int, int>;

        explicit MultiAtlas(createTextureCallback);
        virtual ~MultiAtlas() = default;

        void init(qint32 skipSize = 3);
        void clean();

        bool add(const ImageData& src, Rect& srcRect, spTexture& t);

        const Rect& getBounds() const { return m_bounds; }

    protected:
        struct rect
        {
            spTexture texture;
            Rect rct;
        };
        static bool sortRects(const rect& a, const rect& b);
        void place(const rect& dest, qint32 w, qint32 h, const ImageData& src, spTexture& t, Rect& srcRect);

    protected:
        createTextureCallback m_cb;
        using rects = QQueue<rect>;
        Rect m_bounds;
        rects m_free;
        qint32 m_skipSize;
    };
}
