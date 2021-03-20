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
        if (a._base != b._base ||
            a._table != b._table ||
            a._alpha != b._alpha ||
            a._blend != b._blend ||
            a._flags != b._flags ||
            a._uberShader != b._uberShader ||
            a._addColor != b._addColor)
        {
            return false;
        }
        return true;
    }

    void STDMaterial::init()
    {
        _addColor = QColor(0, 0, 0, 0);
        _blend = blend_premultiplied_alpha;
        _flags = 0;
        _uberShader = &STDRenderer::uberShader;
    }

    void STDMaterial::rehash(size_t& hash) const
    {
        hash_combine(hash, _base.get());
        hash_combine(hash, _alpha.get());
        hash_combine(hash, _table.get());
        hash_combine(hash, (int)_blend);
        hash_combine(hash, _flags);
        hash_combine(hash, _uberShader);
        hash_combine(hash, qRgba(_addColor));
    }

    void STDMaterial::xapply()
    {
        STDRenderer* r = STDRenderer::getCurrent();
        r->setUberShaderProgram(_uberShader);
        int tempFlags = 0;
        if (_table.get() != nullptr)
        {
            tempFlags |= UberShaderProgram::COLOR_TABLE;
        }
        if (_addColor.rgba())
        {
            tempFlags |= UberShaderProgram::ADD_COLOR;
        }
        r->setShaderFlags(_flags | tempFlags);
        if (_addColor.rgba())
        {
            Vector4 vec = Vector4(_addColor.redF(), _addColor.greenF(), _addColor.blueF(), _addColor.alphaF());
            r->getDriver()->setUniform("add_color", vec);
        }
        float brightnessColor = GameWindow::getWindow()->getBrightness();
        float gamma = GameWindow::getWindow()->getGamma();
        Vector4 brightness = Vector4(brightnessColor, brightnessColor, brightnessColor, 0);
        r->getDriver()->setUniform("brightness_color", brightness);
        r->getDriver()->setUniform("gamma", gamma);

        rsCache().setTexture(UberShaderProgram::SAMPLER_TABLE, _table);
        rsCache().setTexture(UberShaderProgram::SAMPLER_BASE, _base);
        rsCache().setTexture(UberShaderProgram::SAMPLER_ALPHA, _alpha);
        rsCache().setBlendMode(_blend);
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
        mat._uberShader = &STDRenderer::uberShader;
        return MaterialCache::mc().cache(mat);
    }

    Material::Material(const Material& other)
    {
        _hash = other._hash;
        _compare = other._compare;
    }

    Material::Material(compare cmp) : _hash(0), _compare(cmp)
    {

    }

    Material::Material() : _hash(0)
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
        _compare = r._compare;
        _hash = r._hash;
        return *this;
    }
}
