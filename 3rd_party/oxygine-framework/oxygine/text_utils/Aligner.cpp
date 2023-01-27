#include "3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

namespace oxygine
{
    namespace text
    {
        Aligner::Aligner(const TextStyle& style, const QSize& size)
#ifdef GRAPHICSUPPORT
            : m_style(style),
              m_bounds(0, 0, 0, 0),
              m_width(size.width()),
              m_height(size.height()),
              m_x(0),
              m_y(0),
              m_metrics(style.font.font)
#endif
        {            
#ifdef GRAPHICSUPPORT
            m_lineSkip = m_metrics.height();
            m_lineNodes.reserve(50);
#endif
        }

        void Aligner::align(text::Node & node)
        {            
#ifdef GRAPHICSUPPORT
            begin();
            node.resize(*this);
            end();
#endif
        }

        void Aligner::begin()
        {
#ifdef GRAPHICSUPPORT
            m_x = 0;
            m_y = m_metrics.ascent();
            m_bounds = QRect(getXAlignment(0), 0, 0, 0);
#endif
        }

        void Aligner::nextLine(qint32 lastLineX, qint32 lastLineWidth)
        {            
#ifdef GRAPHICSUPPORT
            m_y += m_lineSkip;
            updateX();
            m_bounds.moveLeft(std::min(lastLineX, m_bounds.x()));
            m_bounds.setWidth(std::max(lastLineWidth, m_bounds.width()));
            m_x = 0;
#endif
        }

        void Aligner::addLineNode(Node* node)
        {
#ifdef GRAPHICSUPPORT
            m_lineNodes.push_back(node);
#endif
        }

        void Aligner::updateX()
        {
#ifdef GRAPHICSUPPORT
            if (m_lineNodes.size() > 0)
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
#endif
        }

        void Aligner::nodeEnd(qint32 lastLineWidth)
        {
#ifdef GRAPHICSUPPORT
            m_x += lastLineWidth;
            m_bounds.setWidth(std::max(lastLineWidth, m_bounds.width()));
#endif
        }

        void Aligner::end()
        {
#ifdef GRAPHICSUPPORT
            qint32 ry = m_y + m_metrics.descent();
            if (m_style.multiline)
            {
                m_y += m_lineSkip;
            }
            updateX();
            m_bounds.setHeight(ry);
#endif
        }

        qint32 Aligner::getX() const
        {
#ifdef GRAPHICSUPPORT
            return m_x;
#else
            return 0;
#endif
        }

        void Aligner::setX(qint32 newX)
        {
#ifdef GRAPHICSUPPORT
            m_x = newX;
#endif
        }

    }
}
