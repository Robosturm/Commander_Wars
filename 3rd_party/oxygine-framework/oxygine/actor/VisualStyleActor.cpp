#include "3rd_party/oxygine-framework/oxygine/actor/VisualStyleActor.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{
#ifndef GRAPHICSUPPORT
        QColor VStyleActor::m_dummyColor;
#endif

    VStyleActor::VStyleActor()
    {
#ifdef GRAPHICSUPPORT
        Material mat;
        mat.m_blend = VideoDriver::blend_alpha;
        mat.m_base = STDRenderer::white;
        m_flags |= flag_actorHasBounds;
        m_mat = MaterialCache::mc().cache(mat);
#endif
    }

    void VStyleActor::setColor(const QColor& color)
    {
#ifdef GRAPHICSUPPORT
        m_color = color;
#endif
    }

    void VStyleActor::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        setColor(QColor(r, g, b, a));
    }

    void VStyleActor::setAddColor(const QColor& color)
    {
#ifdef GRAPHICSUPPORT
        if (getAddColor() == color)
        {
            return;
        }
        if (!GameWindow::getWindow()->isMainThread())
        {
            emit MemoryManagement::getInstance().sigSetAddColor(getSharedPtr<VStyleActor>(), color);
        }
        else
        {
            changeAddColor(color);
        }
#endif
    }

    void VStyleActor::changeAddColor(const QColor& color)
    {
#ifdef GRAPHICSUPPORT
        m_mat = m_mat->clone();
        m_mat->m_addColor = color;
        m_mat = MaterialCache::mc().cache(*m_mat.get());
        matChanged();
#endif
    }

    void VStyleActor::setAddColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a /*= 0*/)
    {
        setAddColor(QColor(r, g, b, a));
    }

    void VStyleActor::setMaterial(spMaterial & mat)
    {
#ifdef GRAPHICSUPPORT
        m_mat = mat;
        matChanged();
#endif
    }

    void VStyleActor::setDisableColor(const QColor &value)
    {
#ifdef GRAPHICSUPPORT
        m_disableColor = value;
#endif
    }

    void VStyleActor::setEnabled(bool value)
    {
#ifdef GRAPHICSUPPORT
        Actor::setEnabled(value);
        if (value)
        {
            setAddColor(0, 0, 0, 0);
        }
        else
        {
            setAddColor(m_disableColor);
        }
#endif
    }
}
