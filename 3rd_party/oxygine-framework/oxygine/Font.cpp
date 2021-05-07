#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"

namespace oxygine
{
    quint32 qHash(const oxygine::glyph& k, quint32 seed) noexcept
    {
        return ::qHash(k.ch + k.opt, seed);
    }

    Font::Font()
        : m_ignoreOptions(true),
          m_scale(1.0f),
          m_sdf(false),
          m_size(0),
          m_baselineDistance(0)
    {
    }

    Font::~Font()
    {
    }

    void Font::init(qint32 realSize, qint32 baselineDistance, int, bool sdf)
    {
        m_sdf = sdf;
        m_size = realSize;
        m_baselineDistance = baselineDistance;
    }

    void Font::addGlyph(const glyph& gl)
    {
        m_glyphs.insert(gl);
    }

    void Font::clear()
    {
        m_glyphs.clear();
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
        g.opt = m_ignoreOptions ? 0 : opt;
        glyphs::const_iterator it = m_glyphs.find(g);
        if (it != m_glyphs.end())
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
            fn->m_glyphs.insert(gl);
            g = findGlyph(code, opt);
            Q_ASSERT(g);
        }

        return g;
    }

    qint32 Font::getBaselineDistance() const
    {
        return m_baselineDistance;
    }

    qint32 Font::getSize() const
    {
        return m_size;
    }

    float Font::getScale() const
    {
        return m_scale;
    }
}
