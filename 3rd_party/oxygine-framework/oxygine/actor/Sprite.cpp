#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/slidingsprite.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

namespace oxygine
{
#ifndef GRAPHICSUPPORT
    QRectF Sprite::m_dummyRectF;
#endif

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

    bool Sprite::isOn(const QPoint& localPosition)
    {
#ifdef GRAPHICSUPPORT
        if (!Actor::isOn(localPosition))
        {
            return false;
        }
        ResAnim* pAnim = m_frame.getResAnim();
        if (pAnim == nullptr)
        {
            return false;
        }
        QPointF pos = localPosition * pAnim->getAppliedScale();
        if (pAnim->getScaleFactor() > 0.0f)
        {
            pos /= pAnim->getScaleFactor();
        }
        QPoint lp = pos.toPoint();
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
        oxygine::spSprite pActorSprite = std::dynamic_pointer_cast<oxygine::Sprite>(pActor);
        if (pActorSprite.get() != nullptr)
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
            oxygine::spSprite pSprite1 = std::dynamic_pointer_cast<oxygine::Sprite>(child);
            if (pSprite1.get() != nullptr)
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
                oxygine::spSlidingSprite pSprite2 = std::dynamic_pointer_cast<oxygine::SlidingSprite>(child);
                if (pSprite2.get() != nullptr)
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

    void Sprite::setResAnim(const ResAnim* resanim, qint32 col, qint32 row)
    {
        setAnimFrame(resanim, col, row);
    }

    void Sprite::setColorTable(const oxygine::spResAnim pAnim, bool matrix)
    {
#ifdef GRAPHICSUPPORT
        if (requiresThreadChange())
        {
            emit MemoryManagement::getInstance().sigSetColorTable(getSharedPtr<Sprite>(), pAnim, matrix);
        }
        else
        {
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
            else if (m_mat->m_table.get() != nullptr)
            {
                m_mat = m_mat->clone();
                m_mat->m_table.reset();
                m_mat->setMatrixMode(matrix);
                m_mat = MaterialCache::mc().cache(*m_mat.get());
                matChanged();
            }
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
        if (requiresThreadChange())
        {
            emit MemoryManagement::getInstance().sigChangeAnimFrame(getSharedPtr<Sprite>(), frame);
        }
        else
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
            __setSize(m_frame.getSize());


            const spTexture& texture = m_frame.getTexture();
            if (texture != m_mat->m_base)
            {

                spMaterial mat = m_mat->clone();
                mat->m_base  = texture;
                auto newMat = MaterialCache::mc().cache(*mat.get());
                setMaterial(newMat);
            }
            animFrameChanged(m_frame);
        }
#endif
    }

    void Sprite::animFrameChanged(const AnimationFrame&)
    {
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

    QRect Sprite::getDestRect() const
    {
#ifdef GRAPHICSUPPORT
        if (!m_frame.getTexture())
        {
            return Actor::getDestRect();
        }
        return m_frame.getDestRect();
#else
        return QRect();
#endif
    }


    void Sprite::doRender(const RenderState& rs)
    {
        RenderDelegate::instance->doRender(this, rs);
    }
}
