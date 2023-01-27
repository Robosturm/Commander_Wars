#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"

namespace oxygine
{
    namespace text
    {
        void Node::appendNode(spNode tn)
        {
            if (!m_firstChild)
            {
                m_firstChild = tn;
            }
            else
            {
                m_lastChild->m_nextSibling = tn;
            }

            m_lastChild = tn;
        }

        void Node::resize(Aligner& rd)
        {
            rd.addLineNode(this);
            xresize(rd);
            resizeChildren(rd);
        }

        void Node::resizeChildren(Aligner& rd)
        {
            spNode child = m_firstChild;
            while (child)
            {
                child->resize(rd);
                child = child->m_nextSibling;
            }
        }

        void Node::drawChildren(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter)
        {
            spNode child = m_firstChild;
            while (child)
            {
                child->draw(rs, style, drawColor, painter);
                child = child->m_nextSibling;
            }
        }

        void Node::draw(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter)
        {
            drawChildren(rs, style, drawColor, painter);
        }

        TextNode::TextNode(const QString & v)
            : m_text(v)
        {
        }

        void TextNode::draw(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter)
        {
#ifdef GRAPHICSUPPORT
            painter.setTransform(rs.transform);

//            painter.setPen(QPen(drawColor, -1, Qt::SolidLine, style.font.borderCapStyle, style.font.borderJoin));
//            painter.setBrush(drawColor);
//            painter.drawPath(m_path);
            painter.fillPath(m_path, drawColor);
            drawChildren(rs, style, drawColor, painter);
#endif
        }

        void TextNode::xresize(Aligner& rd)
        {
#ifdef GRAPHICSUPPORT
            if (!m_text.isEmpty())
            {
                QStringList lines = m_text.split("\n");
                m_lines.clear();
                m_offsets.clear();
                m_lines.reserve(50);
                m_offsets.reserve(50);
                auto & metrics = rd.getMetrics();
                bool checkWidth = (rd.getWidth() > 0);
                for (auto & line : lines)
                {
                    auto * currentLine = addNewLine(rd);
                    QStringList words = line.split(' ');
                    for (auto & word : words)
                    {
                        if (checkWidth && metrics.horizontalAdvance(*currentLine + word) > rd.getWidth() - rd.getX())
                        {
                            if (rd.getStyle().multiline)
                            {
                                currentLine = addNewLine(rd);
                                *currentLine = metrics.elidedText(word + ' ', rd.getStyle().elideText, rd.getWidth() - rd.getX());
                            }
                            else
                            {
                                *currentLine = metrics.elidedText(*currentLine + word + ' ', rd.getStyle().elideText, rd.getWidth() - rd.getX());
                                break;
                            }
                        }
                        else
                        {
                            *currentLine += word + ' ';
                        }
                    }
                }
                qint32 index = m_lines.size() - 1;
                QString line = m_lines[index];
                qint32 width = metrics.horizontalAdvance(line);
                m_offsets[index].setX(rd.getXAlignment(width));
                rd.nodeEnd(width);
                m_path.clear();
                for (qint32 i = 0; i < m_lines.size(); ++i)
                {
                    qint32 x = static_cast<qint32>(m_offsets[i].x() + rd.getStyle().font.offsetX);
                    qint32 y = static_cast<qint32>(m_offsets[i].y() + rd.getStyle().font.offsetY);
                    m_path.addText(x, y, rd.getStyle().font.font, m_lines[i]);
                }
            }
#endif
        }

#ifdef GRAPHICSUPPORT
        QString * TextNode::addNewLine(Aligner& rd)
        {
            if (m_lines.size() != 0)
            {
                qint32 index = m_lines.size() - 1;
                qint32 width = rd.getMetrics().horizontalAdvance(m_lines[index]);
                qint32 posX = rd.getXAlignment(width);
                m_offsets[index].setX(posX);
                rd.nextLine(posX, width);
                rd.addLineNode(this);
            }
            m_lines.push_back(QString());
            m_offsets.push_back(QPoint(rd.getX(), rd.getY()));
            return &(m_lines[m_lines.size() - 1]);
        }
#endif

        qint32 TextNode::getWidth(Aligner& rd)
        {
#ifdef GRAPHICSUPPORT
            qint32 width = 0;
            if (m_lines.size() > 0)
            {
                qint32 index = m_lines.size() - 1;
                width = rd.getMetrics().boundingRect(m_lines[index]).width();
            }
            return width;
#else
            return 0;
#endif
        }

        void TextNode::setX(qint32 x)
        {
            if (m_lines.size() > 0)
            {
                qint32 index = m_lines.size() - 1;
                m_offsets[index].setX(x);
            }
        }

        void DivNode::resize(Aligner& rd)
        {
            if (m_options == 0xff)
            {
                resizeChildren(rd);
                return;
            }
            resizeChildren(rd);
        }

        void DivNode::draw(const RenderState& rs, const TextStyle & style, const QColor &, QPainter & painter)
        {
            drawChildren(rs, style, m_color, painter);
        }

        DivNode::DivNode(QDomElement& reader)
        {
            m_options = std::numeric_limits<quint32>::max();
            if (reader.hasAttribute("c"))
            {
                QString colorText(reader.attributeNode("c").value());
                if (!colorText.startsWith("#"))
                {
                    colorText.push_front("#");
                }
                m_color = QColor(colorText);
            }
            else if (reader.hasAttribute("color"))
            {
                QString colorText(reader.attributeNode("color").value());
                if (!colorText.startsWith("#"))
                {
                    colorText.push_front("#");
                }
                m_color = QColor(colorText);
            }
            else if (reader.hasAttribute("opt"))
            {
                m_options = reader.attributeNode("opt").value().toUInt();
            }
        }
    }
}
