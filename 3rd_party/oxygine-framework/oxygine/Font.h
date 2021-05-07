#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include <QSet>

namespace oxygine
{
    struct glyph;
    inline quint32 qHash(const oxygine::glyph& k, quint32 seed = 0) noexcept;

    struct glyph
    {
        RectF src;

        qint32 ch{0};
        glyphOptions opt{0};

        short sw{0};
        short sh{0};

        short offset_x{0};
        short offset_y{0};

        short advance_x{0};
        short advance_y{0};

        spNativeTexture texture;

        bool operator == (const glyph& r) const {return ch == r.ch && opt == r.opt;}
    };

    class Font;
    typedef oxygine::intrusive_ptr<Font> spFont;

    class Font : public Object
    {
    public:
        Font();
        ~Font();

        void init(qint32 size, qint32 baselineDistance, qint32 lineHeight, bool sdf = false);

        void addGlyph(const glyph& g);
        void sortGlyphs() {}

        void clear();

        void setScale(float scale) { m_scale = scale; }
        void setBaselineDistance(qint32 d) { m_baselineDistance = d; }

        const glyph*    getGlyph(qint32 code, const glyphOptions& opt) const;
        qint32             getBaselineDistance() const;
        qint32             getSize() const;
        float           getScale() const;

    protected:
        const glyph* findGlyph(qint32 code, const glyphOptions& opt) const;
        virtual bool loadGlyph(int, glyph&, const glyphOptions&) { return false; }
        typedef QSet<glyph> glyphs;

    protected:
        glyphs m_glyphs;
        bool m_ignoreOptions{false};

        float m_scale{0.0f};
        bool m_sdf{false};

        qint32 m_size{0};
        qint32 m_baselineDistance{0};
    };
}
