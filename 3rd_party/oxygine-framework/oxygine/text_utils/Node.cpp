#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
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

        QPoint Node::getRelativPos() const
        {
            return m_relativPos;
        }

        void Node::setRelativPos(QPoint newRelativPos)
        {
            m_relativPos = newRelativPos;
        }

        TextNode::TextNode(const QString & v)
        {
            m_splitData.push_back(v);
            m_yPos.push_back(0);
        }

        void TextNode::draw(const RenderState& rs, const TextStyle & style, const QColor & drawColor, QPainter & painter)
        {
            QPainterPath path;
            for (qint32 i = 0; i < m_splitData.size(); ++i)
            {
                path.addText(rs.transform.x + m_relativPos.x() + style.borderWidth, rs.transform.y + m_relativPos.y() + style.borderWidth + m_yPos[0], style.font, m_splitData[i]);
            }            
            painter.strokePath(path, QPen(Qt::black, style.borderWidth));
            painter.fillPath(path, QBrush(drawColor));
            drawChildren(rs, style, drawColor, painter);
        }

        // todo move to
        // QFontMetrics metrics(style.font);
        // QRect drawnRect;
        // drawnRect = metrics.boundingRect(m_data);
        // qint32 ascent = metrics.ascent();

        void TextNode::xresize(Aligner& rd)
        {
            // if (!m_data.empty())
            // {
            //     qint32 i = 0;
            //     const Font* font = rd.getFont();
            //     while (i != m_data.size())
            //     {
            //         Symbol& s = m_data[i];
            //         if (s.code == '\n')
            //         {
            //             rd.nextLine();
            //         }
            //         else
            //         {
            //             const glyph* gl = font->getGlyph(s.code);
            //             if (gl)
            //             {
            //                 s.gl = *gl;
            //                 i += rd.putSymbol(s);
            //             }
            //             else
            //             {
            //                 gl = font->getGlyph(m_defMissingGlyph);
            //                 if (gl)//even 'missing' symbol  could be missing
            //                 {
            //                     s.gl = *gl;
            //                     i += rd.putSymbol(s);
            //                 }
            //             }

            //             if (gl != nullptr)
            //             {
            //                 if (rd.getMat()->m_base.get() == gl->texture.get())
            //                 {
            //                     s.mat = rd.getMat();
            //                 }
            //                 else
            //                 {
            //                     spMaterial mat = dynamic_pointer_cast<Material>(rd.getMat()->clone());
            //                     mat->m_base = gl->texture;
            //                     s.mat = MaterialCache::mc().cache(*mat.get());
            //                     rd.setMat(s.mat);
            //                 }
            //             }
            //         }
            //         ++i;
            //         if (i < 0)
            //         {
            //             i = 0;
            //         }
            //     }
            // }
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
