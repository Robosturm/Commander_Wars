#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    void AnimationFrame::init(ResAnim* rs, const Diffuse& df, const RectF& srcRect, const RectF& destRect, const Vector2& frame_size)
    {
        m_resAnim = rs;
        m_diffuse = df;
        m_srcRect = srcRect;
        m_destRect = destRect;
        m_frameSize = frame_size.cast<PointS>();
    }

    void AnimationFrame::init2(ResAnim* rs, short col, short row, const Diffuse& df, const RectF& srcRect, const RectF& destRect, const Vector2& frame_size)
    {
        m_column = col;
        m_row = row;
        m_resAnim = rs;
        m_diffuse = df;
        m_srcRect = srcRect;
        m_destRect = destRect;
        m_frameSize = frame_size.cast<PointS>();
    }

    AnimationFrame AnimationFrame::getClipped(const RectF& rect) const
    {
        AnimationFrame f = *this;
        float w = (float)m_diffuse.base->getWidth();
        float h = (float)m_diffuse.base->getHeight();

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

        f.m_frameSize = rect.size;

        return f;
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

    AnimationFrame::AnimationFrame(spNativeTexture t)
    {
        m_row = m_column = 0;
        m_resAnim = 0;
        m_diffuse.base = t;
        m_srcRect = RectF(0, 0, 1, 1);
        m_destRect = RectF(0, 0, (float)t->getWidth(), (float)t->getHeight());
        m_frameSize = Vector2((float)t->getWidth(), (float)t->getHeight());
    }
}
