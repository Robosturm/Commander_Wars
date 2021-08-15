#include "3rd_party/oxygine-framework/oxygine/VisualStyle.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{
    VisualStyle::VisualStyle()
        : m_color(Qt::white),
          m_blend(blend_premultiplied_alpha)
    {

    }

    VStyleActor::VStyleActor()
    {
        Material mat;
        mat.m_blend = blend_premultiplied_alpha;
        mat.m_base = STDRenderer::white;
        m_flags |= flag_actorHasBounds;
        m_mat = MaterialCache::mc().cache(mat);
    }

    void VisualStyle::setColor(const QColor& color)
    {
        m_color = color;
    }

    void VisualStyle::setBlendMode(blend_mode mode)
    {
        m_blend = mode;
    }

    const QColor& VStyleActor::getColor() const
    {
        return m_vstyle.getColor();
    }

    const QColor& VStyleActor::getAddColor() const
    {
        return m_mat->m_addColor;
    }

    void VStyleActor::setColor(const QColor& color)
    {
        m_vstyle.setColor(color);
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
        if (GameWindow::getWindow()->isWorker())
        {
            QMutexLocker lock(&m_Locked);
            changeAddColor(color);
        }
        else
        {
            changeAddColor(color);
        }
    }

    void VStyleActor::changeAddColor(const QColor& color)
    {
        m_mat = m_mat->clone();
        m_mat->m_addColor = color;
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
        QMutexLocker lock(&m_Locked);
        m_vstyle.setBlendMode(mode);
        m_mat = m_mat->clone();
        m_mat->m_blend = mode;
        m_mat = MaterialCache::mc().cache(*m_mat.get());
        matChanged();
    }

    void VStyleActor::setMaterial(spMaterial mat)
    {
        m_mat = mat;
        matChanged();
    }

    QColor VStyleActor::getDisableColor() const
    {
        return m_disableColor;
    }

    void VStyleActor::setDisableColor(const QColor &value)
    {
        m_disableColor = value;
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
            setAddColor(m_disableColor);
        }
    }

}
