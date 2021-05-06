#include "3rd_party/oxygine-framework/oxygine/VisualStyle.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    VisualStyle::VisualStyle()
        : _color(Qt::white),
          _blend(blend_premultiplied_alpha)
    {

    }

    VStyleActor::VStyleActor()
    {
        STDMaterial mat;
        mat._blend = blend_premultiplied_alpha;
        mat._base = STDRenderer::white;
        m_flags |= flag_actorHasBounds;
        m_mat = dynamic_pointer_cast<STDMaterial>(MaterialCache::mc().cache(mat));
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
        return m_mat->_addColor;
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

        m_mat = dynamic_pointer_cast<STDMaterial>(m_mat->clone());
        m_mat->_addColor = color;
        m_mat = MaterialCache::mc().cache(*m_mat.get());
        matChanged();
    }

    void VStyleActor::setAddColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a /*= 0*/)
    {
        setAddColor(QColor(r, g, b, a));
    }

    void VStyleActor::setBlendMode(blend_mode mode)
    {
        if (getBlendMode() == mode)
        {
            return;
        }
        _vstyle.setBlendMode(mode);

        m_mat = dynamic_pointer_cast<STDMaterial>(m_mat->clone());
        m_mat->_blend = mode;
        m_mat = MaterialCache::mc().cache(*m_mat.get());
        matChanged();
    }

    void VStyleActor::setMaterial(spSTDMaterial mat)
    {
        m_mat = mat;
        matChanged();
    }

    void VStyleActor::resetMaterial()
    {
        setMaterial(m_mat->cloneDefaultShader());
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
