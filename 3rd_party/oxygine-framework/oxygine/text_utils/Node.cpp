#include "Node.h"
#include "../AnimationFrame.h"
#include "../Font.h"
#include "../MaterialCache.h"
#include "../RenderState.h"
#include "../STDRenderDelegate.h"
#include "../res/ResFont.h"

namespace oxygine
{
    namespace text
    {
        Node::Node(): _firstChild(0), _lastChild(0), _nextSibling(0)
        {

        }

        Node::~Node()
        {
            Node* child = _firstChild;
            while (child)
            {
                Node* next = child->_nextSibling;
                delete child;

                child = next;
            }
        }

        void Node::appendNode(Node* tn)
        {
            if (!_firstChild)
                _firstChild = tn;
            else
                _lastChild->_nextSibling = tn;

            _lastChild = tn;
        }



        void Node::resize(Aligner& rd)
        {
            xresize(rd);

            resizeChildren(rd);
        }

        void Node::resizeChildren(Aligner& rd)
        {
            Node* child = _firstChild;
            while (child)
            {
                child->resize(rd);
                child = child->_nextSibling;
            }
        }

        void Node::finalPass(Aligner& rd)
        {
            xfinalPass(rd);

            Node* child = _firstChild;
            while (child)
            {
                child->finalPass(rd);
                child = child->_nextSibling;
            }
        }

        void Node::drawChildren(DrawContext& dc)
        {
            Node* child = _firstChild;
            while (child)
            {
                child->draw(dc);
                child = child->_nextSibling;
            }
        }

        Symbol* Node::getSymbol(int& pos)
        {
            Node* node = _firstChild;
            while (node)
            {
                int num = 0;
                Symbol* res = node->getSymbol(pos);
                if (res)
                    return res;
                node = node->_nextSibling;
            }
            return 0;
        }

        void Node::draw(DrawContext& dc)
        {
            drawChildren(dc);
        }



        void Node::updateMaterial(const STDMaterial& mat)
        {
            Node* node = _firstChild;
            while (node)
            {
                node->updateMaterial(mat);
                node = node->_nextSibling;
            }
            xupdateMaterial(mat);
        }


        TextNode::TextNode(QString v)
        {
            auto data = v.toUtf8();
            for (qint32 i = 0; i < data.size(); i++)
            {
                Symbol s;
                s.code = data[i];
                _data.push_back(s);
            }
        }

        Symbol* TextNode::getSymbol(int& pos)
        {
            if ((int)_data.size() > pos)
                return &_data[pos];
            pos -= _data.size();
            return Node::getSymbol(pos);
        }

        void TextNode::draw(DrawContext& dc)
        {
            size_t size = _data.size();
            for (size_t i = 0; i < size; ++i)
            {
                const Symbol& s = _data[i];
                if (!s.mat)
                    continue;

                s.mat->apply();
                s.mat->render(dc.color, s.gl.src, s.destRect);
            }

            drawChildren(dc);
        }

        void TextNode::xupdateMaterial(const STDMaterial& mat)
        {
            for (size_t i = 0; i < _data.size(); ++i)
            {
                size_t size = _data.size();
                for (size_t i = 0; i < size; ++i)
                {
                    Symbol& s = _data[i];

                    spSTDMaterial m = mat.clone();
                    m->_base = s.mat->_base;

                    s.mat = mc().cache(*m.get());
                }
            }
        }

        int _defMissing = '?';
        void TextNode::setDefaultMissingSymbol(int v)
        {
            _defMissing = v;
        }

        void TextNode::xresize(Aligner& rd)
        {
            if (!_data.empty())
            {
                int i = 0;
                const Font* font = rd._font;

                const size_t opt = rd.options;

                while (i != (int)_data.size())
                {
                    Symbol& s = _data[i];
                    if (s.code == '\n')
                        rd.nextLine();
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

                        if (rd.mat->_base == gl->texture)
                            s.mat = rd.mat;
                        else
                        {
                            spSTDMaterial mat = rd.mat->clone();
                            mat->_base = gl->texture;

                            s.mat = mc().cache(*mat.get());
                            rd.mat = s.mat;
                        }
                    }
                    ++i;
                    if (i < 0)
                        i = 0;
                }
            }
        }

        float mlt(int x, float sf)
        {
            //return (x + 0.01f) / sf;
            return x / sf;
        }

        void TextNode::xfinalPass(Aligner& rd)
        {
            float scaleFactor = rd.getScale();

            int offsetY = rd.bounds.pos.y;

            for (size_t i = 0; i < _data.size(); ++i)
            {
                Symbol& s = _data[i];
                s.y += offsetY;

                if (s.gl.texture)
                    s.destRect = RectF(mlt(s.x, scaleFactor), mlt(s.y, scaleFactor), mlt(s.gl.sw, scaleFactor), mlt(s.gl.sh, scaleFactor));
                else
                    s.destRect = RectF(0, 0, 0, 0);
            }
        }

        void DivNode::resize(Aligner& rd)
        {
            if (options == -1)
            {
                resizeChildren(rd);
                return;
            }

            size_t prevOpt = rd.options;
            rd.options = options;
            resizeChildren(rd);
            rd.options = prevOpt;
        }

        void DivNode::draw(DrawContext& dc)
        {
            QColor prev = dc.color;

            dc.color = color * dc.primary;
            drawChildren(dc);
            dc.color = prev;
        }

        DivNode::DivNode(QDomElement& reader)
        {
            options = std::numeric_limits<quint32>::max();
            if (reader.hasAttribute("c"))
            {
                QString colorText(reader.attributeNode("c").value());
                if (!colorText.startsWith("#"))
                {
                    colorText.push_front("#");
                }
                color = QColor(colorText);
            }
            else if (reader.hasAttribute("color"))
            {
                QString colorText(reader.attributeNode("color").value());
                if (!colorText.startsWith("#"))
                {
                    colorText.push_front("#");
                }
                color = QColor(colorText);
            }
            else if (reader.hasAttribute("opt"))
            {
                options = reader.attributeNode("opt").value().toUInt();
            }
        }
    }
}
