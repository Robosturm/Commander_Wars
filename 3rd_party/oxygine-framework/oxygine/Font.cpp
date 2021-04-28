#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"

namespace oxygine
{
    quint32 qHash(const oxygine::glyph& k, quint32 seed) noexcept
    {
        return ::qHash(k.ch + k.opt, seed);
    }

    Font::Font()
        : _ignoreOptions(true),
          _scale(1.0f),
          _sdf(false),
          _size(0),
          _baselineDistance(0)
    {
    }

    Font::~Font()
    {
    }

    void Font::init(QString name, qint32 realSize, qint32 baselineDistance, int, bool sdf)
    {
        setName(name);
        _sdf = sdf;
        _size = realSize;
        _baselineDistance = baselineDistance;
    }

    void Font::addGlyph(const glyph& gl)
    {
        _glyphs.insert(gl);
    }

    void Font::clear()
    {
        _glyphs.clear();
    }

    bool glyphFindPred(const glyph& g, qint32 code)
    {
        return g.ch < code;
    }

    bool glyphsComparePred(const glyph& ob1, const glyph& ob2)
    {
        return ob1.ch < ob2.ch;
    }

    const glyph* Font::findGlyph(qint32 code, const glyphOptions& opt) const
    {
        glyph g;
        g.ch = code;
        g.opt = _ignoreOptions ? 0 : opt;
        glyphs::const_iterator it = _glyphs.find(g);
        if (it != _glyphs.end())
        {
            return &(*it);
        }

        return 0;
    }

    const glyph* Font::getGlyph(qint32 code, const glyphOptions& opt) const
    {
        const glyph* g = findGlyph(code, opt);
        if (g)
        {
            return g;
        }

        glyph gl;
        Font* fn = const_cast<Font*>(this);
        if (fn->loadGlyph(code, gl, opt))
        {
            fn->_glyphs.insert(gl);
            g = findGlyph(code, opt);
            Q_ASSERT(g);
        }

        return g;
    }

    qint32 Font::getBaselineDistance() const
    {
        return _baselineDistance;
    }

    qint32 Font::getSize() const
    {
        return _size;
    }

    float Font::getScale() const
    {
        return _scale;
    }
}
