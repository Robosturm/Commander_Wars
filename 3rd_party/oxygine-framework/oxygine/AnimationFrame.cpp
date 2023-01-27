#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    constexpr qint32 BITS_PER_BYTE = 8;
    void AnimationFrame::init(ResAnim* rs, short col, short row, const spTexture& texture,
                              const QRectF& srcRect, const QRect& destRect, const QSize& frame_size)
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
        m_width = m_srcRect.width() * image.width();
        m_height = m_srcRect.height() * image.height();
        qint32 startX = m_srcRect.x() * image.width();
        qint32 startY = m_srcRect.y() * image.height();
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

    bool AnimationFrame::getHits(const QPoint & pos) const
    {
        if (m_data.size() <= 0)
        {
            return true;
        }
        QRect r(0, 0, m_width, m_height);
        if (r.contains(pos))
        {
            qint32 arrPos = pos.x() + pos.y() * m_width;
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
        m_srcRect = QRectF(m_srcRect.right(), m_srcRect.y(),
                          -m_srcRect.width(), m_srcRect.height());
        m_destRect = QRect(m_frameSize.width() - m_destRect.right(), m_destRect.y(),
                           m_destRect.width(), m_destRect.height());
    }

    void AnimationFrame::flipY()
    {
        m_srcRect = QRectF(m_srcRect.x(), m_srcRect.bottom(),
                          m_srcRect.width(), -m_srcRect.height());
        m_destRect  = QRect(m_destRect.x(), m_frameSize.height() - m_destRect.bottom(),
                            m_destRect.width(), m_destRect.height());
    }

}
