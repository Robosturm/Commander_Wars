#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGLES20.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h"

namespace oxygine
{
    static ResFont* _defaultFont = nullptr;
    void TextField::setDefaultFont(ResFont* f)
    {
        _defaultFont = f;
    }

    ResFont* TextField::getDefaultFont()
    {
        return _defaultFont;
    }

    TextField::TextField():
        _root(nullptr),
        _textRect(0, 0, 0, 0),
        _rtscale(1.0f)
    {
        m_style.font = _defaultFont;
        setText("");
    }

    TextField::~TextField()
    {
    }

    bool TextField::isOn(const Vector2& localPosition, float)
    {
        Rect r = getTextRect();
        r.expand(Point(_extendedIsOn, _extendedIsOn), Point(_extendedIsOn, _extendedIsOn));
        return r.pointIn(Point((int)localPosition.x, (int)localPosition.y));
    }

    void TextField::setVAlign(TextStyle::VerticalAlign align)
    {
        m_style.vAlign = align;
        rebuildText();
    }

    void TextField::setMultiline(bool multiline)
    {
        m_style.multiline = multiline;
        rebuildText();
    }

    void TextField::setBreakLongWords(bool val)
    {
        m_style.breakLongWords = val;
        rebuildText();
    }

    void TextField::setLinesOffset(qint32 offset)
    {
        m_style.linesOffset = offset;
        rebuildText();
    }

    void TextField::setBaselineScale(float s)
    {
        m_style.baselineScale = s;
        rebuildText();
    }

    void TextField::setKerning(qint32 kerning)
    {
        m_style.kerning = kerning;
        rebuildText();
    }

    void TextField::setFontSize(qint32 size)
    {
        m_style.fontSize = size;
        rebuildText();
    }

    void TextField::setStyleColor(const QColor& color)
    {
        m_style.color = color;
        rebuildText();
    }

    void TextField::setOptions(size_t opt)
    {
        m_style.options = opt;
        rebuildText();
    }

    const ResFont* TextField::getFont() const
    {
        return m_style.font;
    }

    void TextField::setFont(const ResFont* font)
    {
        m_style.font = font;
        if (!m_style.font)
        {
            m_style.font = _defaultFont;
        }
        rebuildText();
    }

    void TextField::setHAlign(TextStyle::HorizontalAlign align)
    {
        m_style.hAlign = align;
        rebuildText();
    }

    void TextField::setAlign(TextStyle::VerticalAlign vAlign, TextStyle::HorizontalAlign hAlign)
    {
        m_style.vAlign = vAlign;
        m_style.hAlign = hAlign;
        rebuildText();
    }

    void TextField::setStyle(const TextStyle& st)
    {
        TextStyle::HorizontalAlign halign = m_style.hAlign;
        TextStyle::VerticalAlign valign = m_style.vAlign;
        qint32 size = m_style.fontSize;
        m_style = st;

        if (st.hAlign == TextStyle::HALIGN_DEFAULT)
        {
            m_style.hAlign = halign;
        }
        if (st.vAlign == TextStyle::VALIGN_DEFAULT)
        {
            m_style.vAlign = valign;
        }

        if (st.fontSize == 0)
        {
            m_style.fontSize = size;
        }

        if (!m_style.font)
        {
            m_style.font = _defaultFont;
        }
        rebuildText();
    }

    void TextField::sizeChanged(const Vector2&)
    {
        rebuildText();
    }

    void TextField::matChanged()
    {
        if (!_root)
        {
            return;
        }
        _root->updateMaterial(*m_mat.get());
    }

    void TextField::setText(QString str)
    {
        m_htmlText = false;
        if (m_text != str)
        {
            m_text = str;
            rebuildText();
        }
    }

    QString TextField::getText() const
    {
         QString ret = m_text;
         return ret;
    }

    void TextField::setHtmlText(QString str)
    {
        m_htmlText = true;
        if (m_text != str)
        {
            m_text = str;
            rebuildText();
        }
    }

    qint32 TextField::getFontSize() const
    {
        return m_style.fontSize;
    }

    qint32 TextField::getLinesOffset() const
    {
        return m_style.linesOffset;
    }

    TextStyle::VerticalAlign    TextField::getVAlign() const
    {
        return m_style.vAlign;
    }

    TextStyle::HorizontalAlign  TextField::getHAlign() const
    {
        return m_style.hAlign;
    }

    bool TextField::getMultiline() const
    {
        return m_style.multiline;
    }

    bool TextField::getBreakLongWords() const
    {
        return m_style.breakLongWords;
    }

    qint32 TextField::getKerning() const
    {
        return m_style.kerning;
    }

    const QColor& TextField::getStyleColor() const
    {
        return m_style.color;
    }

    float TextField::getBaselineScale() const
    {
        return m_style.baselineScale;
    }

    size_t TextField::getOptions() const
    {
        return m_style.options;
    }

    text::Symbol* TextField::getSymbolAt(qint32 pos) const
    {
        return const_cast<TextField*>(this)->getRootNode()->getSymbol(pos);
    }

    const Rect& TextField::getTextRect() const
    {
        const_cast<TextField*>(this)->getRootNode();
        return _textRect;
    }

    bool TextField::getBounds(RectF& r) const
    {
        r = getTextRect().cast<RectF>();
        return true;
    }


    text::Node* TextField::getRootNode()
    {
        if (!m_style.font)
        {
            return _root.get();
        }
        return _root.get();
    }

    void TextField::rebuildText()
    {
        float scale = 1.0f;
        if (m_style.font != nullptr)
        {
            const Font* font = m_style.font->getClosestFont(scale, m_style.fontSize, scale);
            if (font)
            {
                m_Locked.lock();
                _rtscale = scale;
                //_realFontSize = fontSize;
                _root = nullptr;
                if (m_htmlText)
                {
                    text::TextBuilder b;
                    _root = b.parse(m_text);
                }
                else
                {
                    _root = new text::TextNode(m_text);
                }
                text::Aligner rd(m_style, m_mat, font, scale, getSize());
                rd.begin();
                _root->resize(rd);
                rd.end();
                _root->finalPass(rd);
                rd.bounds = (rd.bounds.cast<RectF>() / rd.getScale()).cast<Rect>();
                _textRect = rd.bounds;
                m_Locked.unlock();
            }
        }
    }

    void TextField::doRender(RenderState const& rs)
    {
        m_rdelegate->doRender(this, rs);
    }
}
