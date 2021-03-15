#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include <QSet>

namespace oxygine
{
    struct glyph;
    inline uint qHash(const oxygine::glyph& k, uint seed = 0) noexcept;

    struct glyph
    {
        RectF src;

        int ch;
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

        void init(QString name, int size, int baselineDistance, int lineHeight, bool sdf = false);

        void addGlyph(const glyph& g);
        void sortGlyphs() {}

        void clear();

        void setScale(float scale) { _scale = scale; }
        void setBaselineDistance(int d) { _baselineDistance = d; }

        const glyph*    getGlyph(int code, const glyphOptions& opt) const;
        int             getBaselineDistance() const;
        int             getSize() const;
        float           getScale() const;

    protected:
        const glyph* findGlyph(int code, const glyphOptions& opt) const;

        virtual bool loadGlyph(int, glyph&, const glyphOptions&) { return false; }

        typedef QSet<glyph> glyphs;

        glyphs _glyphs;
        bool _ignoreOptions;

        float _scale;
        bool _sdf;

        int _size;
        int _baselineDistance;
    };
}
