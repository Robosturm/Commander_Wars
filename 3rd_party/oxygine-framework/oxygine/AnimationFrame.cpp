#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    constexpr qint32 BITS_PER_BYTE = 8;
    void AnimationFrame::init(ResAnim* rs, short col, short row, const spTexture& texture, const RectF& srcRect, const RectF& destRect, const Point& frame_size)
    {
        m_column = col;
        m_row = row;
        m_resAnim = rs;
        m_texture = texture;
        m_srcRect = srcRect;
        m_destRect = destRect;
        m_frameSize = frame_size;
        updateHittestdata();
    }

    void AnimationFrame::updateHittestdata()
    {
#ifdef GRAPHICSUPPORT
        constexpr qint32 alpha = 0;
        const QImage & image = m_texture->getImage();
        m_width = m_srcRect.getWidth() * image.width();
        m_height = m_srcRect.getHeight() * image.height();
        qint32 startX = m_srcRect.getX() * image.width();
        qint32 startY = m_srcRect.getY() * image.height();
        qint32 size = m_width * m_height / BITS_PER_BYTE + 1;
        m_data.resize(size, false);
        for (qint32 x = 0; x < m_width; ++x)
        {
            for (qint32 y = 0; y < m_height; ++y)
            {
                QColor color = image.pixelColor(startX + x, startY + y);
                if (color.alpha() != alpha)
                {
                    qint32 pos = x + y * m_width;
                    qint32 shift = pos % BITS_PER_BYTE;
                    quint8 bit =  1 << shift;
                    pos /= BITS_PER_BYTE;
                    m_data[pos] |= bit;
                }
            }
        }
#endif
    }

    AnimationFrame AnimationFrame::getClipped(const RectF& rect) const
    {
        AnimationFrame f = *this;
        float w = (float)m_texture->getWidth();
        float h = (float)m_texture->getHeight();

        f.m_destRect.clip(rect);
        if (f.m_destRect.isEmpty())
        {
            f.m_destRect = RectF(0, 0, 0, 0);
        }

        RectF srcRect = m_srcRect * Vector2(w, h);

        float sc = 1.0f;
        if (m_resAnim)
        {
            sc = m_resAnim->getScaleFactor();
        }

        f.m_srcRect.pos = srcRect.pos - (m_destRect.pos - f.m_destRect.pos) * sc;
        f.m_srcRect.size = srcRect.size - (m_destRect.size - f.m_destRect.size) * sc;
        f.m_srcRect = f.m_srcRect / Vector2(w, h);

        f.m_frameSize = rect.size.cast<Point>();

        return f;
    }

    bool AnimationFrame::getHits(Point pos) const
    {
        if (m_data.size() <= 0)
        {
            return true;
        }
        Rect r(0, 0, m_width, m_height);
        if (r.pointIn(pos))
        {
            qint32 arrPos = pos.x + pos.y * m_width;
            qint32 shift = arrPos % BITS_PER_BYTE;
            quint8 bit =  1 << shift;
            arrPos /= BITS_PER_BYTE;
            quint8 value = m_data[arrPos] & bit;
            return value > 0;
        }
        return false;
    }

    AnimationFrame AnimationFrame::getFlipped(bool vertical, bool horizontal) const
    {
        AnimationFrame f = *this;
        if (vertical)
        {
            f.flipY();
        }
        if (horizontal)
        {
            f.flipX();
        }
        return f;
    }

    void AnimationFrame::flipX()
    {
        m_srcRect.setX(m_srcRect.getRight());
        m_srcRect.setWidth(-m_srcRect.getWidth());
        m_destRect.pos.x = m_frameSize.x - m_destRect.getRight();
    }

    void AnimationFrame::flipY()
    {
        m_srcRect.setY(m_srcRect.getBottom());
        m_srcRect.setHeight(-m_srcRect.getHeight());
        m_destRect.pos.y = m_frameSize.y - m_destRect.getBottom();
    }

    AnimationFrame::AnimationFrame(spTexture & t)
    {
        m_row = m_column = 0;
        m_resAnim = 0;
        m_texture = t;
        m_srcRect = RectF(0, 0, 1, 1);
        m_destRect = RectF(0, 0, (float)t->getWidth(), (float)t->getHeight());
        m_frameSize = Point(t->getWidth(), t->getHeight());
    }
}
