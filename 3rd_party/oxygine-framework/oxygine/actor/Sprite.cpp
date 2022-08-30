#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/slidingsprite.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{

    Sprite::~Sprite()
    {
#ifdef GRAPHICSUPPORT
        if (m_flags & flag_manageResAnim)
        {
            ResAnim* rs = m_frame.getResAnim();
            if (rs)
            {
                rs->getAtlas()->unload();
            }
        }
#endif
    }

    void Sprite::setManageResAnim(bool manage)
    {
#ifdef GRAPHICSUPPORT
        m_flags &= ~flag_manageResAnim;
        if (manage)
        {
            m_flags |= flag_manageResAnim;
        }
#endif
    }

    void Sprite::setAnimFrame(const AnimationFrame& f)
    {
        changeAnimFrame(f);
    }

    bool Sprite::isOn(const Vector2& localPosition, float localScale)
    {
#ifdef GRAPHICSUPPORT
        if (!Actor::isOn(localPosition, localScale))
        {
            return false;
        }
        if (m_extendedIsOn)
        {
            return true;
        }
        ResAnim* pAnim = m_frame.getResAnim();
        if (pAnim == nullptr)
        {
            return false;
        }
        Vector2 pos = localPosition * pAnim->getAppliedScale();
        pos = pos.div(m_localScale * pAnim->getScaleFactor());
        Point lp = pos.cast<Point>();
        return m_frame.getHits(lp);
#else
        return false;
#endif
    }

    void Sprite::setFlippedX(bool flippedX)
    {
#ifdef GRAPHICSUPPORT
        if (flippedX != isFlippedX())
        {
            m_frame.flipX();
            m_flags ^= flag_flipX;
            animFrameChanged(m_frame);
        }
#endif
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
        auto & children = pActor->getChildren();
        for (auto & child : children)
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
                oxygine::SlidingSprite* pSprite2 = dynamic_cast<oxygine::SlidingSprite*>(child.get());
                if (pSprite2 != nullptr)
                {
                    if (pSprite2->getFlippedX())
                    {
                        pSprite2->setFlippedX(!flippedX);
                    }
                    else
                    {
                        pSprite2->setFlippedX(flippedX);
                    }
                    if (pSprite2->getLocked())
                    {
                        continue;
                    }
                }
            }
            flipActorsX(child, flippedX);
        }
    }

    void Sprite::setFlippedY(bool flippedY)
    {
#ifdef GRAPHICSUPPORT
        if (flippedY != isFlippedY())
        {
            m_frame.flipY();
            m_flags ^= flag_flipY;
            animFrameChanged(m_frame);
        }
#endif
    }

    void Sprite::setFlipped(bool flippedX, bool flippedY)
    {
#ifdef GRAPHICSUPPORT
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
#endif
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
#ifdef GRAPHICSUPPORT
        m_localScale = s;
        __setSize(m_frame.getSize().mult(m_localScale));
#endif
    }

    void Sprite::setResAnim(const ResAnim* resanim, qint32 col, qint32 row)
    {
        setAnimFrame(resanim, col, row);
    }

    void Sprite::setColorTable(const oxygine::spResAnim pAnim, bool matrix)
    {
#ifdef GRAPHICSUPPORT
        QMutexLocker lock(&m_Locked);
        m_colorTable = pAnim;
        if (pAnim.get() != nullptr)
        {
            const auto & frame = pAnim->getFrame(0, 0);
            if (m_mat->m_table != frame.getTexture())
            {
                m_mat = m_mat->clone();
                m_mat->m_table = frame.getTexture();
                m_mat->setMatrixMode(matrix);
                m_mat = MaterialCache::mc().cache(*m_mat.get());
                matChanged();
            }
        }
        else
        {
            m_mat->m_table = nullptr;
            m_mat->setMatrixMode(false);
        }
#endif
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
#ifdef GRAPHICSUPPORT
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


        const spTexture& texture = m_frame.getTexture();
        if (texture != m_mat->m_base)
        {

            spMaterial mat = m_mat->clone();
            mat->m_base  = texture;
            if (GameWindow::getWindow()->isWorker())
            {
                QMutexLocker lock(&m_Locked);
                auto newMat = MaterialCache::mc().cache(*mat.get());
                setMaterial(newMat);
            }
            else
            {
                auto newMat = MaterialCache::mc().cache(*mat.get());
                setMaterial(newMat);
            }
        }
        animFrameChanged(m_frame);
#endif
    }

    void Sprite::animFrameChanged(const AnimationFrame&)
    {
    }

    void Sprite::sizeChanged(const Vector2& size)
    {
#ifdef GRAPHICSUPPORT
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
#endif
    }

    bool Sprite::getInvertFlipX() const
    {
#ifdef GRAPHICSUPPORT
        return m_invertFlipX;
#else
        return false;
#endif
    }

    void Sprite::setInvertFlipX(bool value)
    {
#ifdef GRAPHICSUPPORT
        m_invertFlipX = value;
#endif
    }

    RectF Sprite::getDestRect() const
    {
#ifdef GRAPHICSUPPORT
        if (!m_frame.getTexture())
        {
            return Actor::getDestRect();
        }
        RectF r = m_frame.getDestRect();
        r.pos = r.pos.mult(m_localScale);
        r.size = r.size.mult(m_localScale);
        return r;
#else
        return RectF();
#endif
    }


    void Sprite::doRender(const RenderState& rs)
    {
        RenderDelegate::instance->doRender(this, rs);
    }
}
