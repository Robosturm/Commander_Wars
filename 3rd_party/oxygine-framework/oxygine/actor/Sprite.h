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

        const AnimationFrame&   getAnimFrame() const {return _frame;}
        RectF                   getDestRect() const override;
        const Diffuse&          getDiffuse() const {return _frame.getDiffuse();}
        bool                    getManageResAnim() const {return (_flags & flag_manageResAnim) != 0;}
        const RectF&            getSrcRect() const {return _frame.getSrcRect();}
        const ResAnim*          getResAnim() const {return _frame.getResAnim();}
        int                     getColumn() const {return _frame.getColumn();}
        int                     getRow() const {return _frame.getRow();}
        const Vector2&          getLocalScale() const { return _localScale; }

        /**load/unload atlas automatically or not*/
        void                    setManageResAnim(bool manage);
        /**Changes sprite image*/
        void                    setAnimFrame(const AnimationFrame& f);
        /**Takes AnimationFrame from ResAnim and set it as current to Sprite. Shows assert is resanim is null. Using this method is more safe than 'setAnimFrame(const AnimationFrame &f)'*/
        void                    setAnimFrame(const ResAnim* resanim, int col = 0, int row = 0);
        void                    setColorTable(const oxygine::spResAnim pAnim);
        virtual void            setResAnim(const ResAnim* resanim, int col = 0, int row = 0);
        void                    setRow(int row);
        void                    setColumn(int column);
        void                    setColumnRow(int column, int row);
        void                    setLocalScale(const Vector2& s);

        bool                    isOn(const Vector2& localPosition, float localScale) override;

        bool                    isFlippedX() const {return (_flags & flag_flipX) != 0;}
        bool                    isFlippedY() const {return (_flags & flag_flipY) != 0;}
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

        Vector2 _localScale;
        AnimationFrame _frame;
        oxygine::spResAnim m_colorTable;

        oxygine::RectF m_DestRecModifier{oxygine::RectF(0.0f, 0.0f, 0.0f, 0.0f)};

        bool invertFlipX{false};
    };
}
