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
        _prepared(false),
        _vertMode(STRETCHING),
        _horzMode(STRETCHING)

    {
        _guideX[0] = 0.0f;
        _guideX[1] = 0.0f;
        _guideY[0] = 0.0f;
        _guideY[1] = 0.0f;

    }

    oxygine::RectF Box9Sprite::getInnerArea() const
    {
        if (!_prepared)
        {
            prepare();
        }
        RectF rect;
        rect.pos = Vector2(_guideX[0], _guideY[0]);
        
        Vector2 rb;
        rb.x = getWidth() - (_frame.getWidth() - _guideX[1]);
        rb.y = getHeight() - (_frame.getHeight() - _guideY[1]);

        rect.setSize(rb - rect.pos);

        return rect;                             
    }

    void Box9Sprite::setVerticalMode(StretchMode m)
    {
        _vertMode = m;
        _prepared = false;
    }

    void Box9Sprite::setHorizontalMode(StretchMode m)
    {
        _horzMode = m;
        _prepared = false;
    }

    void Box9Sprite::setGuides(float x1, float x2, float y1, float y2)
    {
        _guideX[0] = x1;
        _guideX[1] = x2;
        _guideY[0] = y1;
        _guideY[1] = y2;
        _prepared = false;
    }

    void Box9Sprite::setVerticalGuides(float x1, float x2)
    {
        _guideX[0] = x1;
        _guideX[1] = x2;
        _prepared = false;
    }

    void Box9Sprite::setHorizontalGuides(float y1, float y2)
    {
        _guideY[0] = y1;
        _guideY[1] = y2;
        _prepared = false;
    }

    void Box9Sprite::changeAnimFrame(const AnimationFrame& f)
    {
        Vector2 size = getSize();
        Sprite::changeAnimFrame(f);
        setSize(size);
    }

    void Box9Sprite::animFrameChanged(const AnimationFrame& f)
    {
        _prepared = false;

        ResAnim* resanim = f.getResAnim();
        if (resanim)
        {
            float scaleFactor = resanim->getScaleFactor();

            QString attr = resanim->getAttribute("guideX1");
            bool ok = false;
            _guideX[0] = attr.toFloat(&ok);
            if (!ok)
            {
                _guideX[0] = 0;
            }
            _guideX[0] *= scaleFactor;

            attr = resanim->getAttribute("guideX2");
            _guideX[1] = attr.toFloat(&ok);
            if (!ok)
            {
                _guideX[1] = resanim->getWidth();
            }
            _guideX[1] *= scaleFactor;

            attr = resanim->getAttribute("guideY1");
            _guideY[0] = attr.toFloat(&ok);
            if (!ok)
            {
                _guideY[0] = 0;
            }
            _guideY[0] *= scaleFactor;

            attr = resanim->getAttribute("guideY2");
            _guideY[1] = attr.toFloat(&ok);
            if (!ok)
            {
                _guideY[1] = resanim->getHeight();
            }
            _guideY[1] *= scaleFactor;

            attr = resanim->getAttribute("vertical");
            _vertMode = static_cast<StretchMode>(attr.toUInt(&ok));
            if (!ok)
            {
                _vertMode = STRETCHING;
            }

            attr = resanim->getAttribute("horizontal");
            _horzMode = static_cast<StretchMode>(attr.toUInt(&ok));
            if (!ok)
            {
                _horzMode = STRETCHING;
            }
        }
        Sprite::animFrameChanged(f);
    }

    RectF Box9Sprite::getDestRect() const
    {
        //return Sprite::getDestRect();
        return Actor::getDestRect();
    }

    bool Box9Sprite::isOn(const Vector2& localPosition, float localScale)
    {
        return Actor::isOn(localPosition, localScale);
    }

    void Box9Sprite::prepare() const
    {
        _guidesX.resize(4);
        _guidesY.resize(4);
        _pointsX.clear();
        _pointsY.clear();

        float fFrameWidth = _frame.getWidth();
        float fFrameHeight = _frame.getHeight();

        /*
        float fActorWidth = max((float)getSize().x, fFrameWidth);
        float fActorHeight = max((float)getSize().y, fFrameHeight);
        */

        float fActorWidth = getSize().x;
        float fActorHeight = getSize().y;

        if (_guideX[1] == 0.0f)
            _guideX[1] = fFrameWidth;
        if (_guideY[1] == 0.0f)
            _guideY[1] = fFrameHeight;

        RectF srcFrameRect = _frame.getSrcRect();

        _guidesX[0] = srcFrameRect.getLeft(); // these guides contains floats from 0.0 to 1.0, compared to original guides which contain floats in px
        _guidesX[1] = lerp(srcFrameRect.getLeft(), srcFrameRect.getRight(), _guideX[0] / fFrameWidth); // lerp is needed here cuz the frame might be in an atlas
        _guidesX[2] = lerp(srcFrameRect.getLeft(), srcFrameRect.getRight(), _guideX[1] / fFrameWidth);
        _guidesX[3] = srcFrameRect.getRight();

        _guidesY[0] = srcFrameRect.getTop();
        _guidesY[1] = lerp(srcFrameRect.getTop(), srcFrameRect.getBottom(), _guideY[0] / fFrameHeight);
        _guidesY[2] = lerp(srcFrameRect.getTop(), srcFrameRect.getBottom(), _guideY[1] / fFrameHeight);
        _guidesY[3] = srcFrameRect.getBottom();

        // filling X axis
        _pointsX.push_back(0.0f);
        _pointsX.push_back(_guideX[0]);

        if (_horzMode == STRETCHING)
        {
            _pointsX.push_back(fActorWidth - (fFrameWidth - _guideX[1]));
            _pointsX.push_back(fActorWidth);
        }
        else if (_horzMode == TILING || _horzMode == TILING_FULL)
        {
            float curX = _guideX[0];
            float rightB = fActorWidth - (fFrameWidth - _guideX[1]); // right bound (in px)
            float centerPart = _guideX[1] - _guideX[0]; // length of the center piece (in px)

            // now we add a new center piece every time until we reach right bound
            while (1)
            {
                curX += centerPart;

                if (curX <= rightB)
                {
                    _pointsX.push_back(curX);
                }
                else
                {
                    if (_horzMode == TILING_FULL)
                    {
                        _pointsX.push_back(rightB);
                        _pointsX.push_back(fActorWidth);
                    }
                    else
                        _pointsX.push_back(curX - centerPart + (fFrameWidth - _guideX[1]));
                    break;
                }
            }
        }

        // filling Y axis
        _pointsY.push_back(0.0f);
        _pointsY.push_back(_guideY[0]);

        if (_vertMode == STRETCHING)
        {
            _pointsY.push_back(fActorHeight - (fFrameHeight - _guideY[1]));
            _pointsY.push_back(fActorHeight);
        }
        else if (_vertMode == TILING || _vertMode == TILING_FULL)
        {
            float curY = _guideY[0];
            float bottomB = fActorHeight - (fFrameHeight - _guideY[1]); // bottom bound (in px)
            float centerPart = _guideY[1] - _guideY[0]; // length of the center piece (in px)

            // now we add a new center piece every time until we reach right bound
            while (1)
            {
                curY += centerPart;

                if (curY <= bottomB)
                {
                    _pointsY.push_back(curY);
                }
                else
                {
                    if (_vertMode == TILING_FULL)
                    {
                        _pointsY.push_back(bottomB);
                        _pointsY.push_back(fActorHeight);
                    }
                    else
                        _pointsY.push_back(curY - centerPart + (fFrameHeight - _guideY[1]));
                    break;
                }
            }
        }

        _prepared = true;
    }

    void Box9Sprite::sizeChanged(const Vector2&)
    {
        _prepared = false;
    }

    void Box9Sprite::doRender(const RenderState& rs)
    {
        if (!_prepared)
            prepare();


        _mat->apply();

        STDRenderer* renderer = STDRenderer::getCurrent();

        if (_mat->_base)
        {
            if (_guidesX.size() >= 2 || _guidesY.size() >= 2)
            {
                renderer->setTransform(rs.transform);

                QColor color = rs.getFinalColor(getColor());

                // number of vertical blocks
                int vc = (int)_pointsX.size() - 1;
                // number of horizontal blocks
                int hc = (int)_pointsY.size() - 1;

                int xgi = 0; // x guide index
                int ygi = 0;
                for (int yc = 0; yc < hc; yc++)
                {
                    for (int xc = 0; xc < vc; xc++)
                    {
                        if (xc == 0) // select correct index for _guides% arrays
                        {
                            xgi = 0;
                        }
                        else if (xc == (int)_pointsX.size() - 2)
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
                        else if (yc == (int)_pointsY.size() - 2)
                        {
                            ygi = 2;
                        }
                        else
                        {
                            ygi = 1;
                        }
                        RectF srcRect(_guidesX[xgi], _guidesY[ygi], _guidesX[xgi + 1] - _guidesX[xgi], _guidesY[ygi + 1] - _guidesY[ygi]);
                        RectF destRect(_pointsX[xc], _pointsY[yc], _pointsX[xc + 1] - _pointsX[xc], _pointsY[yc + 1] - _pointsY[yc]);

                        renderer->addQuad(color, srcRect, destRect);

                    }
                }
            }
        }
    }
}
