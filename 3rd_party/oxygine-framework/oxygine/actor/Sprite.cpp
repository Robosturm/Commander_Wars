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
    Sprite::Sprite(): m_localScale(1.0f, 1.0f)
    {

    }

    Sprite::~Sprite()
    {
        if (m_flags & flag_manageResAnim)
        {
            ResAnim* rs = m_frame.getResAnim();
            if (rs)
            {
                rs->getAtlas()->unload();
            }
        }
    }

    void Sprite::setManageResAnim(bool manage)
    {
        m_flags &= ~flag_manageResAnim;
        if (manage)
        {
            m_flags |= flag_manageResAnim;
        }
    }

    void Sprite::setAnimFrame(const AnimationFrame& f)
    {
        changeAnimFrame(f);
    }


    extern qint32 HIT_TEST_DOWNSCALE;

    bool Sprite::isOn(const Vector2& localPosition, float localScale)
    {
        if (!Actor::isOn(localPosition, localScale))
        {
            return false;
        }
        if (m_extendedIsOn)
        {
            return true;
        }
        const HitTestData& ad = m_frame.getHitTestData();
        if (!ad.data)
        {
            return true;
        }
        const qint32 BITS = (sizeof(int32_t) * 8);

        const unsigned char* buff = ad.data;
        if (buff != nullptr)
        {
            Vector2 pos = localPosition * m_frame.getResAnim()->getAppliedScale();
            pos = pos.div(m_localScale);
            Point lp = pos.cast<Point>() / HIT_TEST_DOWNSCALE;
            Rect r(0, 0, ad.w, ad.h);
            if (r.pointIn(lp))
            {
                const int32_t* ints = reinterpret_cast<const int32_t*>(buff + lp.y * ad.pitch);

                qint32 n = lp.x / BITS;
                qint32 b = lp.x % BITS;

                return (ints[n] >> b) & 1;
            }
        }
        return false;
    }

    void Sprite::setFlippedX(bool flippedX)
    {
        if (flippedX != isFlippedX())
        {
            m_frame.flipX();
            m_flags ^= flag_flipX;
            animFrameChanged(m_frame);
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
            m_frame.flipY();
            m_flags ^= flag_flipY;
            animFrameChanged(m_frame);
        }
    }

    void Sprite::setFlipped(bool flippedX, bool flippedY)
    {
        bool fx = flippedX != isFlippedX();
        bool fy = flippedY != isFlippedY();

        if (fx)
        {
            m_frame.flipX();
            m_flags ^= flag_flipX;
        }

        if (fy)
        {
            m_frame.flipY();
            m_flags ^= flag_flipY;
        }

        if (fx || fy)
        {
            animFrameChanged(m_frame);
        }
    }

    void Sprite::setColumn(qint32 column)
    {
        const ResAnim* rs = getResAnim();
        setAnimFrame(rs, column, getRow());
    }

    void Sprite::setRow(qint32 row)
    {
        const ResAnim* rs = getResAnim();
        setAnimFrame(rs, getColumn(), row);

    }

    void Sprite::setColumnRow(qint32 column, qint32 row)
    {
        const ResAnim* rs = getResAnim();
        setAnimFrame(rs, column, row);
    }

    void Sprite::setLocalScale(const Vector2& s)
    {
        m_localScale = s;
        __setSize(m_frame.getSize().mult(m_localScale));
    }

    void Sprite::setResAnim(const ResAnim* resanim, qint32 col, qint32 row)
    {
        setAnimFrame(resanim, col, row);
    }

    void Sprite::setColorTable(const oxygine::spResAnim pAnim)
    {
        m_colorTable = pAnim;
        if (pAnim.get() != nullptr)
        {
            const auto & frame = pAnim->getFrame(0, 0);
            if (m_mat->m_table != frame.getDiffuse().base)
            {
                m_mat = dynamic_pointer_cast<STDMaterial>(m_mat->clone());
                m_mat->m_table = frame.getDiffuse().base;
                m_mat = MaterialCache::mc().cache(*m_mat.get());
                matChanged();
            }
        }
        else
        {
            m_mat->m_table = nullptr;
        }
    }

    void Sprite::setAnimFrame(const ResAnim* resanim, qint32 col, qint32 row)
    {
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
        if (m_flags & flag_manageResAnim)
        {
            ResAnim* rs = m_frame.getResAnim();
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

        bool flipX = (m_flags & flag_flipX) != 0;
        bool flipY = (m_flags & flag_flipY) != 0;
        if (flipX || flipY)
        {
            m_frame = frame.getFlipped(flipY, flipX);
        }
        else
        {
            m_frame = frame;
        }
        __setSize(m_frame.getSize().mult(m_localScale));


        const Diffuse& df = m_frame.getDiffuse();
        if (df.base  != m_mat->m_base ||
            df.alpha != m_mat->m_alpha)
        {
            spSTDMaterial mat = dynamic_pointer_cast<STDMaterial>(m_mat->clone());

            mat->m_base  = df.base;
            mat->m_alpha = df.alpha;
            mat->m_flags = df.flags;
            m_mat = MaterialCache::mc().cache(*mat.get());
        }
        animFrameChanged(m_frame);
    }

    void Sprite::animFrameChanged(const AnimationFrame&)
    {

    }

    void Sprite::sizeChanged(const Vector2& size)
    {
        Actor::sizeChanged(size);
        const Vector2& sz = m_frame.getSize();
        if (sz.x != 0)
        {
            m_localScale.x = size.x / sz.x;
        }
        else
        {
            m_localScale.x = 1.0f;
        }

        if (sz.y != 0)
        {
            m_localScale.y = size.y / sz.y;
        }
        else
        {
            m_localScale.y = 1.0f;
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
        return m_invertFlipX;
    }

    void Sprite::setInvertFlipX(bool value)
    {
        m_invertFlipX = value;
    }

    RectF Sprite::getDestRect() const
    {
        if (!m_frame.getDiffuse().base)
        {
            return Actor::getDestRect();
        }

        RectF r = m_frame.getDestRect();
        r.pos = r.pos.mult(m_localScale);
        r.size = r.size.mult(m_localScale);

        r.pos += m_DestRecModifier.pos;
        r.size += m_DestRecModifier.size;

        return r;
    }


    void Sprite::doRender(const RenderState& rs)
    {
        m_rdelegate->doRender(this, rs);
    }
}
