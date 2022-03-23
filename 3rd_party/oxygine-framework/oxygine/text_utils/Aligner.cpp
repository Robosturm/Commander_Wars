#include "3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

namespace oxygine
{
    namespace text
    {
        Aligner::Aligner(const TextStyle& style, const Vector2& size)
            : m_style(style),
              m_bounds(0, 0, 0, 0),
              m_width(size.x),
              m_height(size.y),
              m_x(0),
              m_y(0),
              m_metrics(style.font)
        {
            m_lineSkip = m_metrics.height() + style.borderWidth * 2;
            m_lineNodes.reserve(50);
        }

        void Aligner::align(text::Node & node)
        {
            begin();
            node.resize(*this);
            end();
        }

        void Aligner::begin()
        {
            m_x = 0;
            m_y = m_metrics.ascent();
            m_bounds = Rect(getXAlignment(0), 0, 0, 0);
        }

        void Aligner::nextLine(qint32 lastLineX, qint32 lastLineWidth)
        {
            m_y += m_lineSkip;
            updateX();
            m_bounds.setX(std::min(lastLineX, m_bounds.getX()));
            m_bounds.setWidth(std::max(lastLineWidth, m_bounds.getWidth()));
            m_x = 0;
        }

        void Aligner::addLineNode(Node* node)
        {
            m_lineNodes.push_back(node);
        }

        void Aligner::updateX()
        {
            if (m_lineNodes.size() > 1)
            {
                qint32 totalWidth = 0;
                std::vector<qint32> widths;
                widths.reserve(m_lineNodes.size());
                for (auto & node : m_lineNodes)
                {
                    qint32 width = node->getWidth(*this);
                    widths.push_back(width);
                    totalWidth += width;
                }
                qint32 startX = getXAlignment(totalWidth);
                for (qint32 i = 0; i < m_lineNodes.size(); ++i)
                {
                    m_lineNodes[i]->setX(startX);
                    startX += widths[i];
                }
            }
            m_lineNodes.clear();
        }

        void Aligner::nodeEnd(qint32 lastLineWidth)
        {
            m_x += lastLineWidth;
            m_bounds.setWidth(std::max(lastLineWidth, m_bounds.getWidth()));
        }

        void Aligner::end()
        {
            qint32 ry = m_y;
            if (m_style.multiline)
            {
                m_y += m_lineSkip;
            }
            updateX();
            m_bounds.setHeight(ry);
        }

        qint32 Aligner::getX() const
        {
            return m_x;
        }

        void Aligner::setX(qint32 newX)
        {
            m_x = newX;
        }

    }
}
