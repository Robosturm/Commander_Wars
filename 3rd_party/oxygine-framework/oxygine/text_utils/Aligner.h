#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"
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
            spMaterial mat;
        };

        class Aligner
        {
        public:
            using line = QVector<Symbol*>;
            explicit Aligner(const TextStyle& style, spMaterial mat, const Font* font, float gscale, const Vector2& size);
            virtual ~Aligner() = default;
            const TextStyle& getStyle() const
            {
                return m_style;
            }
            float getScale() const;
            void begin();
            void end();
            qint32 putSymbol(Symbol& s);
            void nextLine();
            const Rect &getBounds() const
            {
                return m_bounds;
            }
            void setBounds(const Rect &newBounds)
            {
                m_bounds = newBounds;
            }
            const Font *getFont() const
            {
                return m_font;
            }
            spMaterial getMat() const
            {
                return m_mat;
            }
            void setMat(spMaterial newMat)
            {
                m_mat = newMat;
            }
        private:
            qint32 getLineWidth()const;
            qint32 getLineSkip()const;
            void _alignLine(line& ln);
            void _nextLine(line& ln);
            qint32 _alignX(qint32 rx);
            qint32 _alignY(qint32 ry);

        private:
            TextStyle m_style;
            Rect m_bounds;
            qint32 m_width;
            qint32 m_height;
            spMaterial m_mat;
            const Font* m_font;
            float m_scale;
            qint32 m_x;
            qint32 m_y;
            line m_line;
            qint32 m_lineWidth;
            qint32 m_lineSkip;
        };
    }
}
