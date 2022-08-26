#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "texture.h"

namespace oxygine
{
    quint32 qHash(const oxygine::glyph& k, quint32 seed) noexcept
    {
        return ::qHash(k.ch, seed);
    }

    void Font::init(qint32 realSize, qint32 baselineDistance)
    {
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

    const glyph* Font::findGlyph(qint32 code) const
    {
        glyph g;
        g.ch = code;
        glyphs::const_iterator it = m_glyphs.find(g);
        if (it != m_glyphs.end())
        {
            return &(*it);
        }

        return 0;
    }

    const glyph* Font::getGlyph(qint32 code) const
    {
        const glyph* g = findGlyph(code);
        if (g)
        {
            return g;
        }

        glyph gl;
        Font* fn = const_cast<Font*>(this);
        if (fn->loadGlyph(code, gl))
        {
            fn->m_glyphs.insert(gl);
            g = findGlyph(code);
            handleErrorPolicy(ep_show_warning, "unable to find glyph Font::getGlyph");
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
