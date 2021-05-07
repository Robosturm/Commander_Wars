#include "3rd_party/oxygine-framework/oxygine/utils/AtlasBuilder.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"

namespace oxygine
{
    AtlasBuilder::AtlasBuilder()
        : m_bounds(0, 0, 0, 0),
          m_skipSize(3)
    {

    }

    void AtlasBuilder::clean()
    {
        m_free.clear();
    }

    void AtlasBuilder::init(qint32 w, qint32 h, qint32 skipSize)
    {
        m_skipSize = skipSize;
        m_bounds = Rect(0, 0, 0, 0);
        m_free.clear();
        m_free.push_back(Rect(0, 0, w, h));
    }

    bool sortRects(const Rect& a, const Rect& b)
    {
        return std::min(a.size.x, a.size.y) < std::min(b.size.x, b.size.y);
    }

    bool AtlasBuilder::add(Texture* dest, const ImageData& src, Rect& srcRect, const Point& offset_)
    {
        Point offset = offset_;
        if (dest)
        {
            if (src.m_w == dest->getWidth())
            {
                offset.x = 0;
            }
            if (src.m_h == dest->getHeight())
            {
                offset.y = 0;
            }
        }

        qint32 w = src.m_w + offset.x * 2;
        qint32 h = src.m_h + offset.y * 2;

        Point size(w, h);

        for (qint32 i = 0; i != m_free.size(); ++i)
        {
            const Rect& rect_ = m_free[i];
            if (rect_.getWidth() >= w && rect_.getHeight() >= h)
            {
                Rect rect = rect_;

                srcRect.pos = rect.pos + offset;
                srcRect.size = Point(src.m_w, src.m_h);

                if (m_bounds.isEmpty())
                {
                    m_bounds = srcRect;
                }
                else
                {
                    m_bounds.unite(srcRect);
                }
                if (dest)
                {
                    dest->updateRegion(srcRect.pos.x, srcRect.pos.y, src);
                }
                m_free.erase(m_free.begin() + i);

                Point ds = rect.size - size;


                Rect a = rect;
                Rect b = rect;

                if (ds.x > ds.y)
                {
                    a.setWidth(w);
                }
                else
                {
                    b.setHeight(h);
                }
                b.setWidth(ds.x);
                a.setHeight(ds.y);

                if (!a.isEmpty() && std::min(a.getWidth(), a.getHeight()) > m_skipSize)
                {
                    a.pos.y = rect.pos.y + h;
                    rects::iterator i = std::lower_bound(m_free.begin(), m_free.end(), a, sortRects);
                    m_free.insert(i, a);
                }


                if (!b.isEmpty() && std::min(b.getWidth(), b.getHeight()) > m_skipSize)
                {
                    b.pos.x = rect.pos.x + w;
                    rects::iterator i = std::lower_bound(m_free.begin(), m_free.end(), b, sortRects);
                    m_free.insert(i, b);
                }

                return true;
            }
        }
        return false;
    }

    MultiAtlas::MultiAtlas(createTextureCallback cb)
        : m_cb(cb),
          m_bounds(0, 0, 0, 0),
          m_skipSize(3)
    {

    }

    void MultiAtlas::clean()
    {
        m_free.clear();
        m_bounds = Rect(0, 0, 0, 0);
    }

    void MultiAtlas::init(qint32 skipSize)
    {
        m_skipSize = skipSize;
    }

    bool MultiAtlas::sortRects(const rect& a, const rect& b)
    {
        return std::min(a.rct.size.x, a.rct.size.y) < std::min(b.rct.size.x, b.rct.size.y);
    }

    void MultiAtlas::place(const rect& rct, qint32 w, qint32 h, const ImageData& src, spTexture& t, Rect& srcRect)
    {
        const Rect& rc = rct.rct;
        srcRect.pos = rc.pos;
        srcRect.size = Point(src.m_w, src.m_h);
        t = rct.texture;


        Point size(w, h);

        Point ds = rc.size - size;
        rct.texture->updateRegion(rc.pos.x, rc.pos.y, src);
        rect a = rct;
        rect b = rct;

        if (ds.x > ds.y)
        {
            a.rct.setWidth(w);
        }
        else
        {
            b.rct.setHeight(h);
        }
        b.rct.setWidth(ds.x);
        a.rct.setHeight(ds.y);

        if (!a.rct.isEmpty() && std::min(a.rct.getWidth(), a.rct.getHeight()) > m_skipSize)
        {
            a.rct.pos.y = rct.rct.pos.y + h;
            rects::iterator i = std::lower_bound(m_free.begin(), m_free.end(), a, sortRects);
            m_free.insert(i, a);
        }


        if (!b.rct.isEmpty() && std::min(b.rct.getWidth(), b.rct.getHeight()) > m_skipSize)
        {
            b.rct.pos.x = rct.rct.pos.x + w;
            rects::iterator i = std::lower_bound(m_free.begin(), m_free.end(), b, sortRects);
            m_free.insert(i, b);
        }
    }

    bool MultiAtlas::add(const ImageData& src, Rect& srcRect, spTexture& t)
    {
        Point offset(2, 2);

        qint32 w = src.m_w + offset.x;
        qint32 h = src.m_h + offset.y;

        Point size(w, h);

        for (qint32 i = 0; i != m_free.size(); ++i)
        {
            const rect& rect_ = m_free[i];
            if (rect_.rct.getWidth() >= w && rect_.rct.getHeight() >= h)
            {
                rect rct = rect_;

                m_bounds.unite(srcRect);

                m_free.erase(m_free.begin() + i);
                place(rct, w, h, src, t, srcRect);

                return true;
            }
        }

        rect rc;
        rc.texture = m_cb(w, h);
        if (!rc.texture)
        {
            return false;
        }
        rc.rct = Rect(0, 0, rc.texture->getWidth(), rc.texture->getHeight());


        place(rc, w, h, src, t, srcRect);
        return true;
    }
}
