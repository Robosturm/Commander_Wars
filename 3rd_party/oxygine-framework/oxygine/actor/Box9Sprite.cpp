#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/math/ScalarMath.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"

namespace oxygine
{

    Box9Sprite::Box9Sprite() :
        m_prepared(false),
        m_vertMode(STRETCHING),
        m_horzMode(STRETCHING)

    {
        m_guideX[0] = 0.0f;
        m_guideX[1] = 0.0f;
        m_guideY[0] = 0.0f;
        m_guideY[1] = 0.0f;

    }

    oxygine::RectF Box9Sprite::getInnerArea() const
    {
        if (!m_prepared)
        {
            prepare();
        }
        RectF rect;
        rect.pos = Vector2(m_guideX[0], m_guideY[0]);
        
        Vector2 rb;
        rb.x = getWidth() - (m_frame.getWidth() - m_guideX[1]);
        rb.y = getHeight() - (m_frame.getHeight() - m_guideY[1]);

        rect.setSize(rb - rect.pos);

        return rect;                             
    }

    void Box9Sprite::setVerticalMode(StretchMode m)
    {
        m_vertMode = m;
        m_prepared = false;
    }

    void Box9Sprite::setHorizontalMode(StretchMode m)
    {
        m_horzMode = m;
        m_prepared = false;
    }

    void Box9Sprite::setGuides(float x1, float x2, float y1, float y2)
    {
        m_guideX[0] = x1;
        m_guideX[1] = x2;
        m_guideY[0] = y1;
        m_guideY[1] = y2;
        m_prepared = false;
    }

    void Box9Sprite::setVerticalGuides(float x1, float x2)
    {
        m_guideX[0] = x1;
        m_guideX[1] = x2;
        m_prepared = false;
    }

    void Box9Sprite::setHorizontalGuides(float y1, float y2)
    {
        m_guideY[0] = y1;
        m_guideY[1] = y2;
        m_prepared = false;
    }

    void Box9Sprite::changeAnimFrame(const AnimationFrame& f)
    {
        Vector2 size = getSize();
        Sprite::changeAnimFrame(f);
        setSize(size);
    }

    void Box9Sprite::animFrameChanged(const AnimationFrame& f)
    {
        m_prepared = false;

        ResAnim* resanim = f.getResAnim();
        if (resanim)
        {
            float scaleFactor = resanim->getScaleFactor();

            QString attr = resanim->getAttribute("guideX1");
            bool ok = false;
            m_guideX[0] = attr.toFloat(&ok);
            if (!ok)
            {
                m_guideX[0] = 0;
            }
            m_guideX[0] *= scaleFactor;

            attr = resanim->getAttribute("guideX2");
            m_guideX[1] = attr.toFloat(&ok);
            if (!ok)
            {
                m_guideX[1] = resanim->getWidth();
            }
            m_guideX[1] *= scaleFactor;

            attr = resanim->getAttribute("guideY1");
            m_guideY[0] = attr.toFloat(&ok);
            if (!ok)
            {
                m_guideY[0] = 0;
            }
            m_guideY[0] *= scaleFactor;

            attr = resanim->getAttribute("guideY2");
            m_guideY[1] = attr.toFloat(&ok);
            if (!ok)
            {
                m_guideY[1] = resanim->getHeight();
            }
            m_guideY[1] *= scaleFactor;

            attr = resanim->getAttribute("vertical");
            m_vertMode = static_cast<StretchMode>(attr.toUInt(&ok));
            if (!ok)
            {
                m_vertMode = STRETCHING;
            }

            attr = resanim->getAttribute("horizontal");
            m_horzMode = static_cast<StretchMode>(attr.toUInt(&ok));
            if (!ok)
            {
                m_horzMode = STRETCHING;
            }
        }
        Sprite::animFrameChanged(f);
    }

    RectF Box9Sprite::getDestRect() const
    {
        return Actor::getDestRect();
    }

    bool Box9Sprite::isOn(const Vector2& localPosition, float localScale)
    {
        return Actor::isOn(localPosition, localScale);
    }

    void Box9Sprite::prepare() const
    {
        m_guidesX.resize(4);
        m_guidesY.resize(4);
        m_pointsX.clear();
        m_pointsY.clear();

        float fFrameWidth = m_frame.getWidth();
        float fFrameHeight = m_frame.getHeight();

        float fActorWidth = getSize().x;
        float fActorHeight = getSize().y;

        if (m_guideX[1] == 0.0f)
        {
            m_guideX[1] = fFrameWidth;
        }
        if (m_guideY[1] == 0.0f)
        {
            m_guideY[1] = fFrameHeight;
        }

        RectF srcFrameRect = m_frame.getSrcRect();

        m_guidesX[0] = srcFrameRect.getLeft(); // these guides contains floats from 0.0 to 1.0, compared to original guides which contain floats in px
        m_guidesX[1] = lerp(srcFrameRect.getLeft(), srcFrameRect.getRight(), m_guideX[0] / fFrameWidth); // lerp is needed here cuz the frame might be in an atlas
        m_guidesX[2] = lerp(srcFrameRect.getLeft(), srcFrameRect.getRight(), m_guideX[1] / fFrameWidth);
        m_guidesX[3] = srcFrameRect.getRight();

        m_guidesY[0] = srcFrameRect.getTop();
        m_guidesY[1] = lerp(srcFrameRect.getTop(), srcFrameRect.getBottom(), m_guideY[0] / fFrameHeight);
        m_guidesY[2] = lerp(srcFrameRect.getTop(), srcFrameRect.getBottom(), m_guideY[1] / fFrameHeight);
        m_guidesY[3] = srcFrameRect.getBottom();

        // filling X axis
        m_pointsX.push_back(0.0f);
        m_pointsX.push_back(m_guideX[0]);

        if (m_horzMode == STRETCHING)
        {
            m_pointsX.push_back(fActorWidth - (fFrameWidth - m_guideX[1]));
            m_pointsX.push_back(fActorWidth);
        }
        else if (m_horzMode == TILING || m_horzMode == TILING_FULL)
        {
            float curX = m_guideX[0];
            float rightB = fActorWidth - (fFrameWidth - m_guideX[1]); // right bound (in px)
            float centerPart = m_guideX[1] - m_guideX[0]; // length of the center piece (in px)

            // now we add a center piece every time until we reach right bound
            while (1)
            {
                curX += centerPart;

                if (curX <= rightB)
                {
                    m_pointsX.push_back(curX);
                }
                else
                {
                    if (m_horzMode == TILING_FULL)
                    {
                        m_pointsX.push_back(rightB);
                        m_pointsX.push_back(fActorWidth);
                    }
                    else
                    {
                        m_pointsX.push_back(curX - centerPart + (fFrameWidth - m_guideX[1]));
                    }
                    break;
                }
            }
        }

        // filling Y axis
        m_pointsY.push_back(0.0f);
        m_pointsY.push_back(m_guideY[0]);

        if (m_vertMode == STRETCHING)
        {
            m_pointsY.push_back(fActorHeight - (fFrameHeight - m_guideY[1]));
            m_pointsY.push_back(fActorHeight);
        }
        else if (m_vertMode == TILING || m_vertMode == TILING_FULL)
        {
            float curY = m_guideY[0];
            float bottomB = fActorHeight - (fFrameHeight - m_guideY[1]); // bottom bound (in px)
            float centerPart = m_guideY[1] - m_guideY[0]; // length of the center piece (in px)

            // now we add a center piece every time until we reach right bound
            while (true)
            {
                curY += centerPart;

                if (curY <= bottomB)
                {
                    m_pointsY.push_back(curY);
                }
                else
                {
                    if (m_vertMode == TILING_FULL)
                    {
                        m_pointsY.push_back(bottomB);
                        m_pointsY.push_back(fActorHeight);
                    }
                    else
                    {
                        m_pointsY.push_back(curY - centerPart + (fFrameHeight - m_guideY[1]));
                    }
                    break;
                }
            }
        }

        m_prepared = true;
    }

    void Box9Sprite::sizeChanged(const Vector2&)
    {
        m_prepared = false;
    }

    void Box9Sprite::doRender(const RenderState& rs)
    {
        if (!m_prepared)
        {
            prepare();
        }

        m_mat->apply();

        STDRenderer* renderer = STDRenderer::getCurrent();

        if (m_mat->m_base)
        {
            if (m_guidesX.size() >= 2 || m_guidesY.size() >= 2)
            {
                renderer->setTransform(rs.transform);

                QColor color = rs.getFinalColor(getColor());

                // number of vertical blocks
                qint32 vc = (int)m_pointsX.size() - 1;
                // number of horizontal blocks
                qint32 hc = (int)m_pointsY.size() - 1;

                qint32 xgi = 0; // x guide index
                qint32 ygi = 0;
                for (qint32 yc = 0; yc < hc; yc++)
                {
                    for (qint32 xc = 0; xc < vc; xc++)
                    {
                        if (xc == 0) // select correct index for _guides% arrays
                        {
                            xgi = 0;
                        }
                        else if (xc == (int)m_pointsX.size() - 2)
                        {
                            xgi = 2;
                        }
                        else
                        {
                            xgi = 1;
                        }
                        if (yc == 0)
                        {
                            ygi = 0;
                        }
                        else if (yc == (int)m_pointsY.size() - 2)
                        {
                            ygi = 2;
                        }
                        else
                        {
                            ygi = 1;
                        }
                        RectF srcRect(m_guidesX[xgi], m_guidesY[ygi], m_guidesX[xgi + 1] - m_guidesX[xgi], m_guidesY[ygi + 1] - m_guidesY[ygi]);
                        RectF destRect(m_pointsX[xc], m_pointsY[yc], m_pointsX[xc + 1] - m_pointsX[xc], m_pointsY[yc + 1] - m_pointsY[yc]);

                        renderer->addQuad(color, srcRect, destRect);

                    }
                }
            }
        }
    }
}
