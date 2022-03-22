#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{
    namespace text
    {
        class Aligner
        {
        public:
            explicit Aligner(const TextStyle& style, const Vector2& size);
            virtual ~Aligner() = default;
            const TextStyle& getStyle() const
            {
                return m_style;
            }
            float getScale() const;
            void begin();
            void end();
            void nextLine();
            const Rect &getBounds() const
            {
                return m_bounds;
            }

        private:
            qint32 getLineWidth() const;
            qint32 getLineSkip() const;
            // void _alignLine(line& ln);
            // void _nextLine(line& ln);
            qint32 _alignX(qint32 rx);

        private:
            TextStyle m_style;
            Rect m_bounds;
            qint32 m_width;
            qint32 m_height;
            qint32 m_x;
            qint32 m_y;
            // line m_line;
            qint32 m_lineWidth;
            qint32 m_lineSkip;
        };
    }
}
