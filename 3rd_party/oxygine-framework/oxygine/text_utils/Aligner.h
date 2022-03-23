#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

#include <QFontMetrics>

namespace oxygine
{
    namespace text
    {
        class Node;
        class Aligner
        {
        public:
            explicit Aligner(const TextStyle& style, const Vector2& size);
            virtual ~Aligner() = default;
            const TextStyle& getStyle() const
            {
                return m_style;
            }
            void align(text::Node & node);
            void nextLine(qint32 lastLineX, qint32 lastLineWidth);
            void nodeEnd(qint32 lastLineWidth);
            /**
             * @brief getXAlignment calculates the x-alignement for the given line width
             * @param lineWidth
             * @return
             */
            inline qint32 getXAlignment(qint32 lineWidth)
            {
                qint32 tx = 0;
                switch (m_style.hAlign)
                {
                    case TextStyle::HALIGN_LEFT:
                    case TextStyle::HALIGN_DEFAULT:
                        tx = 0;
                        break;
                    case TextStyle::HALIGN_MIDDLE:
                        tx = m_width / 2 - lineWidth / 2;
                        break;
                    case TextStyle::HALIGN_RIGHT:
                        tx = m_width - lineWidth;
                        break;
                }
                return tx;
            }
            /**
             * @brief getY current Y advance position
             * @return
             */
            inline qint32 getY() const
            {
                return m_y;
            }

            inline const Rect &getBounds() const
            {
                return m_bounds;
            }
            /**
             * @brief getWidth the available width of the text rect
             * @return
             */
            inline qint32 getWidth() const
            {
                return m_width;
            }

            inline const QFontMetrics &getMetrics() const
            {
                return m_metrics;
            }

            qint32 getX() const;
            void setX(qint32 newX);
            void addLineNode(Node* node);
        private:
            void begin();
            void end();
            void updateX();
        private:
            TextStyle m_style;
            Rect m_bounds;
            qint32 m_width;
            qint32 m_height;
            qint32 m_x;
            qint32 m_y;
            qint32 m_lineSkip;
            QFontMetrics m_metrics;
            std::vector<Node*> m_lineNodes;
        };
    }
}
