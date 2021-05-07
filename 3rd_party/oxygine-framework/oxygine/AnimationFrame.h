#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{
    class Diffuse
    {
    public:
        Diffuse(): flags(0) {}
        spNativeTexture base;
        spNativeTexture alpha;
        //bool premultiplied;
        qint32 flags;
    };

    class HitTestData
    {
    public:
        HitTestData(): data(0), w(0), h(0), pitch(0) {}

        const unsigned char* data;
        short w, h;
        unsigned char pitch;
    };

    class AnimationFrame
    {
    public:
        AnimationFrame()
            : m_srcRect(0, 0, 1, 1),
              m_destRect(0, 0, 1, 1),
              m_resAnim(0),
              m_row(0),
              m_column(0)
        {
        }
        AnimationFrame(spNativeTexture t);

        void init(ResAnim* rs, const Diffuse& df,
                  const RectF& srcRect, const RectF& destRect, const Vector2& frame_size);
        void init2(ResAnim* rs, short column, short row, const Diffuse& df,
                   const RectF& srcRect, const RectF& destRect, const Vector2& frame_size);
        /**ResAnim should be valid!*/
        AnimationFrame getClipped(const RectF& rect) const;
        AnimationFrame getFlipped(bool vertical, bool horizontal) const;

        /**Returns size of frame in pixels*/
        const Vector2&  getSize() const {return m_frameSize;}
        float           getWidth() const {return m_frameSize.x;}
        float           getHeight() const {return m_frameSize.y;}

        ResAnim*        getResAnim() const {return m_resAnim;}
        short           getColumn() const {return m_column;}
        short           getRow() const {return m_row;}
        const RectF&    getSrcRect() const {return m_srcRect;}
        const RectF&    getDestRect() const {return m_destRect;}
        const Diffuse&  getDiffuse() const {return m_diffuse;}
        const HitTestData& getHitTestData()const { return m_hittest; }

        void            setSrcRect(const RectF& r) {m_srcRect = r;}
        void            setDestRect(const RectF& r) {m_destRect = r;}
        void            setResAnim(ResAnim* rs) {m_resAnim = rs;}
        void            setDiffuse(const Diffuse& d) { m_diffuse = d; }
        void            setSize(const Vector2& size) {m_frameSize = size;}
        void            setSize(float w, float h) { setSize(Vector2(w, h)); }
        void            setHitTestData(const HitTestData& ad) { m_hittest = ad; }
        void            setRow(qint32 v) {m_row = v;}
        void            setColumn(qint32 v) {m_column = v;}

        void            flipX();
        void            flipY();

    private:
        enum flags
        {
            clipped = 0x01,
        };

        Diffuse         m_diffuse;
        RectF           m_srcRect;
        RectF           m_destRect;
        Vector2         m_frameSize;//real size without clipping
        ResAnim*        m_resAnim;
        short           m_row;
        short           m_column;
        HitTestData     m_hittest;
    };
}
