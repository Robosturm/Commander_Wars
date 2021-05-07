#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    spMaterialX Material::current;
    spMaterialX Material::null;

    bool STDMaterial::cmp(const STDMaterial& a, const STDMaterial& b)
    {
        if (a.m_base != b.m_base ||
            a.m_table != b.m_table ||
            a.m_alpha != b.m_alpha ||
            a.m_blend != b.m_blend ||
            a.m_flags != b.m_flags ||
            a.m_uberShader != b.m_uberShader ||
            a.m_addColor != b.m_addColor)
        {
            return false;
        }
        return true;
    }

    void STDMaterial::init()
    {
        m_addColor = QColor(0, 0, 0, 0);
        m_blend = blend_premultiplied_alpha;
        m_flags = 0;
        m_uberShader = &STDRenderer::uberShader;
    }

    void STDMaterial::rehash(size_t& hash) const
    {
        hash_combine(hash, m_base.get());
        hash_combine(hash, m_alpha.get());
        hash_combine(hash, m_table.get());
        hash_combine(hash, (int)m_blend);
        hash_combine(hash, m_flags);
        hash_combine(hash, m_uberShader);
        hash_combine(hash, qRgba(m_addColor));
    }

    void STDMaterial::xapply()
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
            Vector4 vec = Vector4(m_addColor.redF(), m_addColor.greenF(), m_addColor.blueF(), m_addColor.alphaF());
            r->getDriver()->setUniform("add_color", vec);
        }
        float brightnessColor = GameWindow::getWindow()->getBrightness();
        float gamma = GameWindow::getWindow()->getGamma();
        Vector4 brightness = Vector4(brightnessColor, brightnessColor, brightnessColor, 0);
        r->getDriver()->setUniform("brightness_color", brightness);
        r->getDriver()->setUniform("gamma", gamma);

        rsCache().setTexture(UberShaderProgram::SAMPLER_TABLE, m_table);
        rsCache().setTexture(UberShaderProgram::SAMPLER_BASE, m_base);
        rsCache().setTexture(UberShaderProgram::SAMPLER_ALPHA, m_alpha);
        rsCache().setBlendMode(m_blend);
    }

    void STDMaterial::xflush()
    {
        STDRenderer::getCurrent()->flush();
    }


    void STDMaterial::render(const AffineTransform& tr, const QColor& c, const RectF& src, const RectF& dest)
    {
        STDRenderer::getCurrent()->setTransform(tr);
        STDRenderer::getCurrent()->addQuad(c, src, dest);
    }

    void STDMaterial::render(const QColor& c, const RectF& src, const RectF& dest)
    {
        STDRenderer::getCurrent()->addQuad(c, src, dest);
    }


    spSTDMaterial STDMaterial::cloneDefaultShader() const
    {
        STDMaterial mat(*this);
        mat.m_uberShader = &STDRenderer::uberShader;
        return MaterialCache::mc().cache(mat);
    }

    Material::Material(const Material& other)
    {
        m_hash = other.m_hash;
        m_compare = other.m_compare;
    }

    Material::Material(compare cmp)
        : m_hash(0),
          m_compare(cmp)
    {

    }

    Material::Material() : m_hash(0)
    {

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

    oxygine::Material& Material::operator=(const Material& r)
    {
        m_compare = r.m_compare;
        m_hash = r.m_hash;
        return *this;
    }
}
