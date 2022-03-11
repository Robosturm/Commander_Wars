#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"

namespace oxygine
{
    namespace text
    {        
        qint32 TextNode::m_defMissingGlyph = '?';

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

        void Node::finalPass(Aligner& rd)
        {
            xfinalPass(rd);
            spNode child = m_firstChild;
            while (child)
            {
                child->finalPass(rd);
                child = child->m_nextSibling;
            }
        }

        void Node::drawChildren(DrawContext& dc)
        {
            spNode child = m_firstChild;
            while (child)
            {
                child->draw(dc);
                child = child->m_nextSibling;
            }
        }

        Symbol* Node::getSymbol(int& pos)
        {
            spNode node = m_firstChild;
            while (node)
            {
                Symbol* res = node->getSymbol(pos);
                if (res)
                    return res;
                node = node->m_nextSibling;
            }
            return 0;
        }

        void Node::draw(DrawContext& dc)
        {
            drawChildren(dc);
        }

        void Node::updateMaterial(const Material& mat)
        {
            spNode node = m_firstChild;
            while (node)
            {
                node->updateMaterial(mat);
                node = node->m_nextSibling;
            }
            xupdateMaterial(mat);
        }

        TextNode::TextNode(const QString & v)
        {
            for (auto & c : v)
            {
                Symbol s;
                s.code = c.unicode();
                m_data.push_back(s);
            }
        }

        Symbol* TextNode::getSymbol(int& pos)
        {
            if (m_data.size() > pos)
            {
                return &m_data[pos];
            }
            pos -= m_data.size();
            return Node::getSymbol(pos);
        }

        void TextNode::draw(DrawContext& dc)
        {
            for (const auto & s : qAsConst(m_data))
            {
                if (!s.mat)
                {
                    continue;
                }

                s.mat->apply();
                s.mat->render(dc.m_color, s.gl.src, s.destRect);
            }

            drawChildren(dc);
        }

        void TextNode::xupdateMaterial(const Material& mat)
        {
            for (auto & s : m_data)
            {
                spMaterial m = mat.clone();
                m->m_base = s.mat->m_base;
                s.mat = MaterialCache::mc().cache(*m.get());
            }
        }

        void TextNode::xresize(Aligner& rd)
        {
            if (!m_data.empty())
            {
                qint32 i = 0;
                const Font* font = rd.getFont();
                while (i != m_data.size())
                {
                    Symbol& s = m_data[i];
                    if (s.code == '\n')
                    {
                        rd.nextLine();
                    }
                    else
                    {
                        const glyph* gl = font->getGlyph(s.code);
                        if (gl)
                        {
                            s.gl = *gl;
                            i += rd.putSymbol(s);
                        }
                        else
                        {
                            gl = font->getGlyph(m_defMissingGlyph);
                            if (gl)//even 'missing' symbol  could be missing
                            {
                                s.gl = *gl;
                                i += rd.putSymbol(s);
                            }
                        }

                        if (gl != nullptr)
                        {
                            if (rd.getMat()->m_base.get() == gl->texture.get())
                            {
                                s.mat = rd.getMat();
                            }
                            else
                            {
                                spMaterial mat = dynamic_pointer_cast<Material>(rd.getMat()->clone());
                                mat->m_base = gl->texture;
                                s.mat = MaterialCache::mc().cache(*mat.get());
                                rd.setMat(s.mat);
                            }
                        }
                    }
                    ++i;
                    if (i < 0)
                    {
                        i = 0;
                    }
                }
            }
        }

        float mlt(qint32 x, float sf)
        {
            return x / sf;
        }

        void TextNode::xfinalPass(Aligner& rd)
        {
            float scaleFactor = rd.getScale();

            qint32 offsetY = rd.getBounds().pos.y;

            for (auto & s : m_data)
            {
                s.y += offsetY;
                if (s.gl.texture)
                {
                    s.destRect = RectF(mlt(s.x, scaleFactor), mlt(s.y, scaleFactor), mlt(s.gl.sw, scaleFactor), mlt(s.gl.sh, scaleFactor));
                }
                else
                {
                    s.destRect = RectF(0, 0, 0, 0);
                }
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

        void DivNode::draw(DrawContext& dc)
        {
            QColor prev = dc.m_color;

            dc.m_color = m_color * dc.m_primary;
            drawChildren(dc);
            dc.m_color = prev;
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
