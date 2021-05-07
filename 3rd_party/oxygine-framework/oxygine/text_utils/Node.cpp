#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"

namespace oxygine
{
    namespace text
    {
        Node::Node(): m_firstChild(0), m_lastChild(0), m_nextSibling(0)
        {
        }

        Node::~Node()
        {
        }

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

        void Node::updateMaterial(const STDMaterial& mat)
        {
            spNode node = m_firstChild;
            while (node)
            {
                node->updateMaterial(mat);
                node = node->m_nextSibling;
            }
            xupdateMaterial(mat);
        }

        TextNode::TextNode(QString v)
        {
            for (qint32 i = 0; i < v.size(); i++)
            {
                Symbol s;
                s.code = v[i].unicode();
                _data.push_back(s);
            }
        }

        Symbol* TextNode::getSymbol(int& pos)
        {
            if (_data.size() > pos)
            {
                return &_data[pos];
            }
            pos -= _data.size();
            return Node::getSymbol(pos);
        }

        void TextNode::draw(DrawContext& dc)
        {
            qint32 size = _data.size();
            for (qint32 i = 0; i < size; ++i)
            {
                const Symbol& s = _data[i];
                if (!s.mat)
                {
                    continue;
                }

                s.mat->apply();
                s.mat->render(dc.m_color, s.gl.src, s.destRect);
            }

            drawChildren(dc);
        }

        void TextNode::xupdateMaterial(const STDMaterial& mat)
        {
            for (qint32 i = 0; i < _data.size(); ++i)
            {
                size_t size = _data.size();
                for (size_t i = 0; i < size; ++i)
                {
                    Symbol& s = _data[i];

                    spSTDMaterial m = dynamic_pointer_cast<STDMaterial>(mat.clone());
                    m->m_base = s.mat->m_base;

                    s.mat = MaterialCache::mc().cache(*m.get());
                }
            }
        }

        qint32 _defMissing = '?';
        void TextNode::setDefaultMissingSymbol(qint32 v)
        {
            _defMissing = v;
        }

        void TextNode::xresize(Aligner& rd)
        {
            if (!_data.empty())
            {
                qint32 i = 0;
                const Font* font = rd.m_font;

                const size_t opt = rd.m_options;

                while (i != (int)_data.size())
                {
                    Symbol& s = _data[i];
                    if (s.code == '\n')
                    {
                        rd.nextLine();
                    }
                    else
                    {
                        const glyph* gl = font->getGlyph(s.code, opt);
                        if (gl)
                        {
                            s.gl = *gl;
                            i += rd.putSymbol(s);
                        }
                        else
                        {
                            gl = font->getGlyph(_defMissing, opt);
                            if (gl)//even 'missing' symbol  could be missing
                            {
                                s.gl = *gl;
                                i += rd.putSymbol(s);
                            }
                        }

                        if (gl != nullptr)
                        {
                            if (rd.m_mat->m_base.get() == gl->texture.get())
                            {
                                s.mat = rd.m_mat;
                            }
                            else
                            {
                                spSTDMaterial mat = dynamic_pointer_cast<STDMaterial>(rd.m_mat->clone());
                                mat->m_base = gl->texture;

                                s.mat = dynamic_pointer_cast<STDMaterial>(MaterialCache::mc().cache(*mat.get()));
                                rd.m_mat = s.mat;
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

            qint32 offsetY = rd.m_bounds.pos.y;

            for (qint32 i = 0; i < _data.size(); ++i)
            {
                Symbol& s = _data[i];
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

            size_t prevOpt = rd.m_options;
            rd.m_options = m_options;
            resizeChildren(rd);
            rd.m_options = prevOpt;
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
