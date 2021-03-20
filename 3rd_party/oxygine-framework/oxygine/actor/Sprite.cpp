#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/slidingsprite.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
    Sprite::Sprite(): _localScale(1.0f, 1.0f)
    {

    }

    Sprite::~Sprite()
    {
        if (_flags & flag_manageResAnim)
        {
            ResAnim* rs = _frame.getResAnim();
            if (rs)
                rs->getAtlas()->unload();
        }
    }

    void Sprite::setManageResAnim(bool manage)
    {
        _flags &= ~flag_manageResAnim;
        if (manage)
            _flags |= flag_manageResAnim;
    }

    void Sprite::setAnimFrame(const AnimationFrame& f)
    {
        changeAnimFrame(f);
    }


    extern int HIT_TEST_DOWNSCALE;

    bool Sprite::isOn(const Vector2& localPosition, float localScale)
    {
        if (!Actor::isOn(localPosition, localScale))
            return false;

        if (_extendedIsOn)
            return true;

        const HitTestData& ad = _frame.getHitTestData();
        if (!ad.data)
            return true;

        const int BITS = (sizeof(int32_t) * 8);

        const unsigned char* buff = ad.data;
        if (buff != nullptr)
        {
            Vector2 pos = localPosition * _frame.getResAnim()->getAppliedScale();
            pos = pos.div(_localScale);
            Point lp = pos.cast<Point>() / HIT_TEST_DOWNSCALE;
            Rect r(0, 0, ad.w, ad.h);
            if (r.pointIn(lp))
            {
                const int32_t* ints = reinterpret_cast<const int32_t*>(buff + lp.y * ad.pitch);

                int n = lp.x / BITS;
                int b = lp.x % BITS;

                return (ints[n] >> b) & 1;
            }
        }
        return false;
    }

    void Sprite::setFlippedX(bool flippedX)
    {
        if (flippedX != isFlippedX())
        {
            _frame.flipX();
            _flags ^= flag_flipX;
            animFrameChanged(_frame);
        }        
    }

    void Sprite::flipActorsX(oxygine::spActor pActor, bool flippedX)
    {
        oxygine::Sprite* pActorSprite = dynamic_cast<oxygine::Sprite*>(pActor.get());
        if (pActorSprite != nullptr)
        {
            if (pActorSprite->getInvertFlipX())
            {
                pActorSprite->setFlippedX(!flippedX);
            }
            else
            {
                pActorSprite->setFlippedX(flippedX);
            }
        }
        oxygine::spActor child = pActor->getFirstChild();
        while (child)
        {
            oxygine::Sprite* pSprite1 = dynamic_cast<oxygine::Sprite*>(child.get());
            if (pSprite1 != nullptr)
            {
                if (pSprite1->getInvertFlipX())
                {
                    pSprite1->setFlippedX(!flippedX);
                }
                else
                {
                    pSprite1->setFlippedX(flippedX);
                }
            }
            else
            {
                oxygine::SlidingSprite* pSprite1 = dynamic_cast<oxygine::SlidingSprite*>(child.get());
                if (pSprite1 != nullptr)
                {
                    if (pSprite1->getFlipped())
                    {
                        pSprite1->setFlipped(!flippedX);
                    }
                    else
                    {
                        pSprite1->setFlipped(flippedX);
                    }
                }
            }
            flipActorsX(child, flippedX);
            child = child->getNextSibling();
        }
    }

    void Sprite::setFlippedY(bool flippedY)
    {
        if (flippedY != isFlippedY())
        {
            _frame.flipY();
            _flags ^= flag_flipY;
            animFrameChanged(_frame);
        }
    }

    void Sprite::setFlipped(bool flippedX, bool flippedY)
    {
        bool fx = flippedX != isFlippedX();
        bool fy = flippedY != isFlippedY();

        if (fx)
        {
            _frame.flipX();
            _flags ^= flag_flipX;
        }

        if (fy)
        {
            _frame.flipY();
            _flags ^= flag_flipY;
        }

        if (fx || fy)
            animFrameChanged(_frame);
    }

    void Sprite::setColumn(int column)
    {
        const ResAnim* rs = getResAnim();
        setAnimFrame(rs, column, getRow());
    }

    void Sprite::setRow(int row)
    {
        const ResAnim* rs = getResAnim();
        setAnimFrame(rs, getColumn(), row);

    }

    void Sprite::setColumnRow(int column, int row)
    {
        const ResAnim* rs = getResAnim();
        setAnimFrame(rs, column, row);
    }

    void Sprite::setLocalScale(const Vector2& s)
    {
        _localScale = s;
        _setSize(_frame.getSize().mult(_localScale));
    }

    void Sprite::setResAnim(const ResAnim* resanim, int col, int row)
    {
        setAnimFrame(resanim, col, row);
    }

    void Sprite::setColorTable(const oxygine::spResAnim pAnim)
    {
        m_colorTable = pAnim;
        if (pAnim.get() != nullptr)
        {
            const auto & frame = pAnim->getFrame(0, 0);
            if (_mat->_table != frame.getDiffuse().base)
            {
                _mat = _mat->clone();
                _mat->_table = frame.getDiffuse().base;
                _mat = MaterialCache::mc().cache(*_mat.get());
                matChanged();
            }
        }
        else
        {
            _mat->_table = nullptr;
        }
    }

    void Sprite::setAnimFrame(const ResAnim* resanim, int col, int row)
    {
        //Q_ASSERT(resanim);
        if (!resanim)
        {
            changeAnimFrame(AnimationFrame());
            return;
        }

        if (resanim->getTotalFrames())
        {
            const AnimationFrame& frame = resanim->getFrame(col, row);
            changeAnimFrame(frame);
        }
        else
        {
            AnimationFrame frame;
            frame.setSize(getSize());
            changeAnimFrame(frame);
        }
    }


    void Sprite::changeAnimFrame(const AnimationFrame& frame)
    {
        if (_flags & flag_manageResAnim)
        {
            ResAnim* rs = _frame.getResAnim();
            if (rs)
            {
                rs->getAtlas()->unload();
            }

            rs = frame.getResAnim();
            if (rs)
            {
                rs->getAtlas()->load();
            }
        }

        bool flipX = (_flags & flag_flipX) != 0;
        bool flipY = (_flags & flag_flipY) != 0;
        if (flipX || flipY)
        {
            _frame = frame.getFlipped(flipY, flipX);
        }
        else
        {
            _frame = frame;
        }
        _setSize(_frame.getSize().mult(_localScale));


        const Diffuse& df = _frame.getDiffuse();
        if (df.base  != _mat->_base ||
            df.alpha != _mat->_alpha)
        {
            spSTDMaterial mat = _mat->clone();

            mat->_base  = df.base;
            mat->_alpha = df.alpha;
            mat->_flags = df.flags;           
            _mat = MaterialCache::mc().cache(*mat.get());
        }
        animFrameChanged(_frame);
    }

    void Sprite::animFrameChanged(const AnimationFrame&)
    {

    }

    void Sprite::sizeChanged(const Vector2& size)
    {
        Actor::sizeChanged(size);
        const Vector2& sz = _frame.getSize();
        if (sz.x != 0)
        {
            _localScale.x = size.x / sz.x;
        }
        else
        {
            _localScale.x = 1.0f;
        }

        if (sz.y != 0)
        {
            _localScale.y = size.y / sz.y;
        }
        else
        {
            _localScale.y = 1.0f;
        }
    }

    oxygine::RectF Sprite::getDestRecModifier() const
    {
        return m_DestRecModifier;
    }

    void Sprite::setDestRecModifier(const oxygine::RectF &DestRecModifier)
    {
        m_DestRecModifier = DestRecModifier;
    }

    bool Sprite::getInvertFlipX() const
    {
        return invertFlipX;
    }

    void Sprite::setInvertFlipX(bool value)
    {
        invertFlipX = value;
    }

    RectF Sprite::getDestRect() const
    {
        if (!_frame.getDiffuse().base)
        {
            return Actor::getDestRect();
        }

        RectF r = _frame.getDestRect();
        r.pos = r.pos.mult(_localScale);
        r.size = r.size.mult(_localScale);

        r.pos += m_DestRecModifier.pos;
        r.size += m_DestRecModifier.size;

        return r;
    }


    void Sprite::doRender(const RenderState& rs)
    {
        _rdelegate->doRender(this, rs);
    }
}
