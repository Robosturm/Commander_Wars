#include "3rd_party/oxygine-framework/oxygine/VisualStyle.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    VisualStyle::VisualStyle(): _color(Qt::white), _blend(blend_premultiplied_alpha)
    {

    }

    VStyleActor::VStyleActor()
    {
        STDMaterial mat;
        mat._blend = blend_premultiplied_alpha;
        mat._base = STDRenderer::white;
        _flags |= flag_actorHasBounds;
        _mat = MaterialCache::mc().cache(mat);
    }

    void VisualStyle::setColor(const QColor& color)
    {
        _color = color;
    }

    void VisualStyle::setBlendMode(blend_mode mode)
    {
        _blend = mode;
    }

    const QColor& VStyleActor::getColor() const
    {
        return _vstyle.getColor();
    }

    const QColor& VStyleActor::getAddColor() const
    {
        return _mat->_addColor;
    }

    void VStyleActor::setColor(const QColor& color)
    {
        _vstyle.setColor(color);
    }

    void VStyleActor::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        setColor(QColor(r, g, b, a));
    }

    void VStyleActor::setAddColor(const QColor& color)
    {
        if (getAddColor() == color)
        {
            return;
        }

        _mat = _mat->clone();
        _mat->_addColor = color;
        _mat = MaterialCache::mc().cache(*_mat.get());
        matChanged();
    }

    void VStyleActor::setAddColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a /*= 0*/)
    {
        setAddColor(QColor(r, g, b, a));
    }

    void VStyleActor::setBlendMode(blend_mode mode)
    {
        if (getBlendMode() == mode)
            return;

        _vstyle.setBlendMode(mode);

        _mat = _mat->clone();
        _mat->_blend = mode;
        _mat = MaterialCache::mc().cache(*_mat.get());
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

    QColor VStyleActor::getDisableColor() const
    {
        return disableColor;
    }

    void VStyleActor::setDisableColor(const QColor &value)
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
