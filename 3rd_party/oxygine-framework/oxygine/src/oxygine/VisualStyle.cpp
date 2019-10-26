#include "VisualStyle.h"
#include "MaterialCache.h"
#include "RenderState.h"
#include "actor/Actor.h"
#include "utils/stringUtils.h"
#include <sstream>

namespace oxygine
{
    VisualStyle::VisualStyle(): _color(Color::White), _blend(blend_premultiplied_alpha)
    {

    }

    VStyleActor::VStyleActor()
    {
        STDMaterial mat;
        mat._blend = blend_premultiplied_alpha;
        mat._base = STDRenderer::white;
        _flags |= flag_actorHasBounds;
        _mat = mc().cache(mat);
    }

    void VStyleActor::copyFrom(const VStyleActor& src, cloneOptions opt)
    {
        inherited::copyFrom(src, opt);
        _vstyle = src._vstyle;
    }

    void VisualStyle::setColor(const Color& color)
    {
        _color = color;
    }

    void VisualStyle::setBlendMode(blend_mode mode)
    {
        _blend = mode;
    }

    const Color& VStyleActor::getColor() const
    {
        return _vstyle.getColor();
    }

    const Color& VStyleActor::getAddColor() const
    {
        return _mat->_addColor;
    }

    void VStyleActor::setColor(const Color& color)
    {
        _vstyle.setColor(color);
    }

    void VStyleActor::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        setColor(Color(r, g, b, a));
    }

    void VStyleActor::setAddColor(const Color& color)
    {
        if (getAddColor() == color)
            return;

        _mat = _mat->clone();
        _mat->_addColor = color;
        _mat = mc().cache(*_mat.get());
        matChanged();
    }

    void VStyleActor::setAddColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a /*= 0*/)
    {
        setAddColor(Color(r, g, b, a));
    }

    void VStyleActor::setBlendMode(blend_mode mode)
    {
        if (getBlendMode() == mode)
            return;

        _vstyle.setBlendMode(mode);

        _mat = _mat->clone();
        _mat->_blend = mode;
        _mat = mc().cache(*_mat.get());
        matChanged();
    }

    void VStyleActor::setMaterial(spSTDMaterial mat)
    {
        //if (_mat == mat)
        //    return;
        _mat = mat;
        matChanged();
    }

    void VStyleActor::resetMaterial()
    {
        setMaterial(_mat->cloneDefaultShader());
    }

    Color VStyleActor::getDisableColor() const
    {
        return disableColor;
    }

    void VStyleActor::setDisableColor(const Color &value)
    {
        disableColor = value;
    }

    void VStyleActor::setEnabled(bool value)
    {
        Actor::setEnabled(value);
        if (value)
        {
            setAddColor(0, 0, 0, 0);
        }
        else
        {
            setAddColor(disableColor);
        }
    }

}
