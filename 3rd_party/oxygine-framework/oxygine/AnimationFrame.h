#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "texture.h"

namespace oxygine
{
class AnimationFrame final
{
public:
    explicit AnimationFrame() = default;
    ~AnimationFrame() = default;
    void init(ResAnim* rs, short column, short row, const spTexture& texture,
              const QRectF& srcRect, const QRect& destRect, const QSize& frame_size);
    /**ResAnim should be valid!*/
    AnimationFrame getFlipped(bool vertical, bool horizontal) const;

    /**Returns size of frame in pixels*/
    const QSize&  getSize() const
    {
        return m_frameSize;
    }
    qint32 getWidth() const
    {
        return m_frameSize.width();
    }
    qint32 getHeight() const
    {
        return m_frameSize.height();
    }
    ResAnim* getResAnim() const
    {
        return m_resAnim;
    }
    short getColumn() const
    {
        return m_column;
    }
    short getRow() const
    {
        return m_row;
    }
    const QRectF& getSrcRect() const
    {
        return m_srcRect;
    }
    const QRect& getDestRect() const
    {
        return m_destRect;
    }
    const spTexture& getTexture() const
    {
        return m_texture;
    }
    /**
         * @brief getHits
         * @param x
         * @param y
         * @return
         */
    bool getHits(const QPoint & pos) const;

    void setSrcRect(const QRectF& r)
    {
        m_srcRect = r;
    }
    void setDestRect(const QRect& r)
    {
        m_destRect = r;
    }
    void setResAnim(ResAnim* rs)
    {
        m_resAnim = rs;
    }
    void setTexture(const spTexture& tex)
    {
        m_texture = tex;
    }
    void setSize(const QSize& size)
    {
        m_frameSize = size;
    }
    void setSize(qint32 w, qint32 h)
    {
        setSize(QSize(w, h));
    }
    void setRow(qint32 v)
    {
        m_row = v;
    }
    void setColumn(qint32 v)
    {
        m_column = v;
    }
    void flipX();
    void flipY();
private:
    void updateHittestdata();
private:
    spTexture m_texture{nullptr};
    QRectF m_srcRect{0, 0, 1, 1};
    QRect m_destRect{0, 0, 1, 1};
    QSize m_frameSize;
    ResAnim*  m_resAnim{nullptr};
    short m_row{0};
    short m_column{0};
    std::vector<quint8> m_data;
    short m_width{0};
    short m_height{0};
};
}

