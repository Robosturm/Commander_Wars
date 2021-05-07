#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include <qqueue.h>

namespace oxygine
{
    DECLARE_SMART(Texture, spTexture);

    class AnimationFrame;
    typedef QVector<AnimationFrame> frames;

    class AtlasBuilder
    {
    public:
        AtlasBuilder();

        void init(qint32 w, qint32 h, qint32 skipSize = 3);
        void clean();

        bool add(Texture* dest, const ImageData& src, Rect& srcRect, const Point& offset);

        const Rect& getBounds() const { return m_bounds; }

    protected:
        typedef QQueue<Rect> rects;
        Rect m_bounds;
        rects m_free;
        qint32 m_skipSize;
    };

    typedef AtlasBuilder Atlas2;

    class MultiAtlas
    {
    public:
        using createTextureCallback = Closure<spTexture, int, int>;

        MultiAtlas(createTextureCallback);

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
        typedef QQueue<rect> rects;
        Rect m_bounds;
        rects m_free;
        qint32 m_skipSize;
    };
}
