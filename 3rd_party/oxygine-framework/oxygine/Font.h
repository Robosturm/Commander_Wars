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

        qint32 ch;
        glyphOptions opt;

        short sw;
        short sh;

        short offset_x;
        short offset_y;

        short advance_x;
        short advance_y;

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

        void init(QString name, qint32 size, qint32 baselineDistance, qint32 lineHeight, bool sdf = false);

        void addGlyph(const glyph& g);
        void sortGlyphs() {}

        void clear();

        void setScale(float scale) { _scale = scale; }
        void setBaselineDistance(qint32 d) { _baselineDistance = d; }

        const glyph*    getGlyph(qint32 code, const glyphOptions& opt) const;
        qint32             getBaselineDistance() const;
        qint32             getSize() const;
        float           getScale() const;

    protected:
        const glyph* findGlyph(qint32 code, const glyphOptions& opt) const;

        virtual bool loadGlyph(int, glyph&, const glyphOptions&) { return false; }

        typedef QSet<glyph> glyphs;

        glyphs _glyphs;
        bool _ignoreOptions;

        float _scale;
        bool _sdf;

        qint32 _size;
        qint32 _baselineDistance;
    };
}
