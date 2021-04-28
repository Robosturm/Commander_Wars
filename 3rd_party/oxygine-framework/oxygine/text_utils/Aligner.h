#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{
    struct glyph;
    class Font;


    namespace text
    {
        struct Symbol
        {
            Symbol(): x(0), y(0), code(0) {}

            short x, y;
            qint32 code;
            glyph gl;
            RectF destRect;
            spSTDMaterial mat;
        };

        class Aligner
        {
        public:
            Aligner(const TextStyle& style, spSTDMaterial mat, const Font* font, float gscale, const Vector2& size);
            ~Aligner();


            const TextStyle& getStyle() const {return style;}
            float getScale() const;

            void begin();
            void end();

            qint32 putSymbol(Symbol& s);
            void nextLine();


            TextStyle style;
            Rect bounds;
            qint32 width;
            qint32 height;
            size_t options;

            spSTDMaterial mat;

            const Font* _font;

        private:
            qint32 getLineWidth()const;
            qint32 getLineSkip()const;


            typedef QVector<Symbol*> line;

            void _alignLine(line& ln);
            qint32 _alignX(qint32 rx);
            qint32 _alignY(qint32 ry);
            void _nextLine(line& ln);

            float _scale;
            qint32 _x, _y;
            line _line;
            qint32 _lineWidth;
            qint32 _lineSkip;
        };
    }
}
