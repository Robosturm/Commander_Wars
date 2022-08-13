#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include "texture.h"

namespace oxygine
{
    class AnimationFrame
    {
    public:
        explicit AnimationFrame() = default;
        explicit AnimationFrame(spTexture & t);
        virtual ~AnimationFrame() = default;
        void init(ResAnim* rs, short column, short row, const spTexture& texture,
                   const RectF& srcRect, const RectF& destRect, const Vector2& frame_size);
        /**ResAnim should be valid!*/
        AnimationFrame getClipped(const RectF& rect) const;
        AnimationFrame getFlipped(bool vertical, bool horizontal) const;

        /**Returns size of frame in pixels*/
        const Vector2&  getSize() const
        {
            return m_frameSize;
        }
        float getWidth() const
        {
            return m_frameSize.x;
        }
        float getHeight() const
        {
            return m_frameSize.y;
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
        const RectF& getSrcRect() const
        {
            return m_srcRect;
        }
        const RectF& getDestRect() const
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
        bool getHits(Point pos) const;

        void setSrcRect(const RectF& r)
        {
            m_srcRect = r;
        }
        void setDestRect(const RectF& r)
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
        void setSize(const Vector2& size)
        {
            m_frameSize = size;
        }
        void setSize(float w, float h)
        {
            setSize(Vector2(w, h));
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
        spTexture m_texture;
        RectF m_srcRect{0, 0, 1, 1};
        RectF m_destRect{0, 0, 1, 1};
        Vector2 m_frameSize;
        ResAnim*  m_resAnim{nullptr};
        short m_row{0};
        short m_column{0};
        std::vector<quint8> m_data;
        short m_width{0};
        short m_height{0};
    };
}

