#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/math/ScalarMath.h"

namespace oxygine
{
    spMaterial Material::current{nullptr};
    spMaterial Material::null{nullptr};

    bool Material::compare(const Material* matA, const Material* matB)
    {
        if (matA->m_base       != matB->m_base ||
            matA->m_table      != matB->m_table ||
            matA->m_blend      != matB->m_blend ||
            matA->m_fracShader      != matB->m_fracShader ||
            matA->m_addColor   != matB->m_addColor)
        {
            return false;
        }
        return true;
    }

    void Material::init()
    {
        m_addColor = QColor(0, 0, 0, 0);
        m_blend = VideoDriver::blend_alpha;
        m_fracShader = UberShaderProgram::ColorMode::COLOR_DEFAULT;
    }

    void Material::rehash(size_t& hash) const
    {
        hash_combine(hash, m_base.get());
        hash_combine(hash, m_table.get());
        hash_combine(hash, static_cast<qint32>(m_blend));
        hash_combine(hash, m_fracShader);
        hash_combine(hash, qRgba(m_addColor));
    }

    void Material::setMatrixMode(bool set)
    {
        if (set)
        {
            m_fracShader = UberShaderProgram::COLOR_MATRIX;
        }
        else
        {
            m_fracShader = UberShaderProgram::COLOR_DEFAULT;
        }
    }

    void Material::xapply()
    {
        STDRenderer* r = STDRenderer::getCurrent();
        if (m_table.get() != nullptr && m_fracShader == UberShaderProgram::ColorMode::COLOR_DEFAULT)
        {
            r->setFracShader(UberShaderProgram::ColorMode::COLOR_TABLE);
        }
        else
        {
            r->setFracShader(m_fracShader);
        }
        VideoDriver::Uniform4f vec = VideoDriver::Uniform4f(m_addColor.redF(), m_addColor.greenF(), m_addColor.blueF(), m_addColor.alphaF());
        r->getDriver()->setUniform("add_color", vec);
        float brightnessColor = GameWindow::getWindow()->getBrightness();
        float gamma = GameWindow::getWindow()->getGamma();
        VideoDriver::Uniform4f brightness = VideoDriver::Uniform4f(brightnessColor, brightnessColor, brightnessColor, 0);
        r->getDriver()->setUniform("brightness_color", brightness);
        r->getDriver()->setUniform("gamma", gamma);

        rsCache().setTexture(UberShaderProgram::SAMPLER_TABLE, m_table);
        rsCache().setTexture(UberShaderProgram::SAMPLER_BASE, m_base);
        rsCache().setBlendMode(m_blend);
    }

    void Material::xflush()
    {
        STDRenderer::getCurrent()->flush();
    }

    void Material::render(const QTransform& tr, const QColor& c, const QRectF& src, QRect& dest)
    {
        STDRenderer::getCurrent()->setTransform(tr);
        STDRenderer::getCurrent()->addQuad(c, src, dest);
    }

    void Material::apply()
    {
        if (current != this)
        {
            current->flush();
            xapply();
            current = spMaterial(this);
        }
    }

    void Material::flush()
    {
        xflush();
    }
}
