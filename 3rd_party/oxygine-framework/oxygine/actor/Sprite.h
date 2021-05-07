#pragma once
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/VisualStyle.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/tween/TweenAnim.h"

namespace oxygine
{
    DECLARE_SMART(Sprite, spSprite);
    class Sprite : public VStyleActor
    {
    public:
        Sprite();
        virtual ~Sprite() override;

        const AnimationFrame&   getAnimFrame() const {return m_frame;}
        RectF                   getDestRect() const override;
        const Diffuse&          getDiffuse() const {return m_frame.getDiffuse();}
        bool                    getManageResAnim() const {return (m_flags & flag_manageResAnim) != 0;}
        const RectF&            getSrcRect() const {return m_frame.getSrcRect();}
        const ResAnim*          getResAnim() const {return m_frame.getResAnim();}
        qint32                     getColumn() const {return m_frame.getColumn();}
        qint32                     getRow() const {return m_frame.getRow();}
        const Vector2&          getLocalScale() const { return m_localScale; }

        /**load/unload atlas automatically or not*/
        void                    setManageResAnim(bool manage);
        /**Changes sprite image*/
        void                    setAnimFrame(const AnimationFrame& f);
        /**Takes AnimationFrame from ResAnim and set it as current to Sprite. Shows assert is resanim is null. Using this method is more safe than 'setAnimFrame(const AnimationFrame &f)'*/
        void                    setAnimFrame(const ResAnim* resanim, qint32 col = 0, qint32 row = 0);
        void                    setColorTable(const oxygine::spResAnim pAnim);
        virtual void            setResAnim(const ResAnim* resanim, qint32 col = 0, qint32 row = 0);
        void                    setRow(qint32 row);
        void                    setColumn(qint32 column);
        void                    setColumnRow(qint32 column, qint32 row);
        void                    setLocalScale(const Vector2& s);

        bool                    isOn(const Vector2& localPosition, float localScale) override;

        bool                    isFlippedX() const {return (m_flags & flag_flipX) != 0;}
        bool                    isFlippedY() const {return (m_flags & flag_flipY) != 0;}
        void                    setFlippedX(bool flippedX);
        virtual void            flipActorsX(bool flippedX){flipActorsX(this, flippedX);}
        static void             flipActorsX(oxygine::spActor pActor, bool flippedX);
        void                    setFlippedY(bool flippedY);
        void                    setFlipped(bool flippedX, bool flippedY);
        bool getInvertFlipX() const;
        void setInvertFlipX(bool value);

        typedef oxygine::TweenAnim TweenAnim;

        void doRender(const RenderState&) override;

        oxygine::RectF getDestRecModifier() const;
        void setDestRecModifier(const oxygine::RectF &DestRecModifier);

    protected:
        enum
        {
            flag_manageResAnim = flag_last << 1,
            flag_flipX = flag_last << 2,
            flag_flipY = flag_last << 3
        };
        virtual void changeAnimFrame(const AnimationFrame& f);
        virtual void animFrameChanged(const AnimationFrame& f);
        void sizeChanged(const Vector2& size) override;

    protected:
        Vector2 m_localScale;
        AnimationFrame m_frame;
        oxygine::spResAnim m_colorTable;
        oxygine::RectF m_DestRecModifier{oxygine::RectF(0.0f, 0.0f, 0.0f, 0.0f)};
        bool m_invertFlipX{false};
    };
}
