#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    spMaterial Material::current;
    spMaterial Material::null;

    Material::Material(const Material& other)
        : m_hash(other.m_hash),
          m_base(other.m_base),
          m_table(other.m_table),
          m_alpha(other.m_alpha),
          m_blend(other.m_blend),
          m_uberShader(other.m_uberShader),
          m_addColor(other.m_addColor),
          m_flags(other.m_flags)
    {
    }

    bool Material::compare(const Material* matA, const Material* matB)
    {
        if (matA->m_base       != matB->m_base ||
            matA->m_table      != matB->m_table ||
            matA->m_alpha      != matB->m_alpha ||
            matA->m_blend      != matB->m_blend ||
            matA->m_flags      != matB->m_flags ||
            matA->m_uberShader != matB->m_uberShader ||
            matA->m_addColor   != matB->m_addColor)
        {
            return false;
        }
        return true;
    }

    void Material::init()
    {
        m_addColor = QColor(0, 0, 0, 0);
        m_blend = VideoDriver::blend_premultiplied_alpha;
        m_flags = 0;
        m_uberShader = &STDRenderer::uberShader;
    }

    void Material::rehash(size_t& hash) const
    {
        hash_combine(hash, m_base.get());
        hash_combine(hash, m_alpha.get());
        hash_combine(hash, m_table.get());
        hash_combine(hash, static_cast<qint32>(m_blend));
        hash_combine(hash, m_flags);
        hash_combine(hash, m_uberShader);
        hash_combine(hash, qRgba(m_addColor));
    }

    void Material::xapply()
    {
        STDRenderer* r = STDRenderer::getCurrent();
        r->setUberShaderProgram(m_uberShader);
        qint32 tempFlags = 0;
        if (m_table.get() != nullptr)
        {
            tempFlags |= UberShaderProgram::COLOR_TABLE;
        }
        if (m_addColor.rgba())
        {
            tempFlags |= UberShaderProgram::ADD_COLOR;
        }
        r->setShaderFlags(m_flags | tempFlags);
        if (m_addColor.rgba())
        {
            VideoDriver::Uniform4f vec = VideoDriver::Uniform4f(m_addColor.redF(), m_addColor.greenF(), m_addColor.blueF(), m_addColor.alphaF());
            r->getDriver()->setUniform("add_color", vec);
        }
        float brightnessColor = GameWindow::getWindow()->getBrightness();
        float gamma = GameWindow::getWindow()->getGamma();
        VideoDriver::Uniform4f brightness = VideoDriver::Uniform4f(brightnessColor, brightnessColor, brightnessColor, 0);
        r->getDriver()->setUniform("brightness_color", brightness);
        r->getDriver()->setUniform("gamma", gamma);

        rsCache().setTexture(UberShaderProgram::SAMPLER_TABLE, m_table);
        rsCache().setTexture(UberShaderProgram::SAMPLER_BASE, m_base);
        rsCache().setTexture(UberShaderProgram::SAMPLER_ALPHA, m_alpha);
        rsCache().setBlendMode(m_blend);
    }

    void Material::xflush()
    {
        STDRenderer::getCurrent()->flush();
    }

    void Material::render(const AffineTransform& tr, const QColor& c, const RectF& src, const RectF& dest)
    {
        STDRenderer::getCurrent()->setTransform(tr);
        STDRenderer::getCurrent()->addQuad(c, src, dest);
    }

    void Material::render(const QColor& c, const RectF& src, const RectF& dest)
    {
        STDRenderer::getCurrent()->addQuad(c, src, dest);
    }

    void Material::apply()
    {
        if (current != this)
        {
            current->flush();
            xapply();
            current = this;
        }
    }

    void Material::flush()
    {
        xflush();
    }
}
