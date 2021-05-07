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
            Symbol()
                : x(0),
                  y(0),
                  code(0)
            {
            }

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


            const TextStyle& getStyle() const {return m_style;}
            float getScale() const;

            void begin();
            void end();
            qint32 putSymbol(Symbol& s);
            void nextLine();

        public:
            TextStyle m_style;
            Rect m_bounds;
            qint32 m_width;
            qint32 m_height;
            size_t m_options;
            spSTDMaterial m_mat;
            const Font* m_font;

        private:
            typedef QVector<Symbol*> line;
            qint32 getLineWidth()const;
            qint32 getLineSkip()const;
            void _alignLine(line& ln);
            void _nextLine(line& ln);
            qint32 _alignX(qint32 rx);
            qint32 _alignY(qint32 ry);

        private:
            float m_scale;
            qint32 m_x;
            qint32 m_y;
            line m_line;
            qint32 m_lineWidth;
            qint32 m_lineSkip;
        };
    }
}
