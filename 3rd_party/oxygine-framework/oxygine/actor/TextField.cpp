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
        _style.font = _defaultFont;
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
        _style.vAlign = align;
        rebuildText();
    }

    void TextField::setMultiline(bool multiline)
    {
        _style.multiline = multiline;
        rebuildText();
    }

    void TextField::setBreakLongWords(bool val)
    {
        _style.breakLongWords = val;
        rebuildText();
    }

    void TextField::setLinesOffset(int offset)
    {
        _style.linesOffset = offset;
        rebuildText();
    }

    void TextField::setBaselineScale(float s)
    {
        _style.baselineScale = s;
        rebuildText();
    }

    void TextField::setKerning(int kerning)
    {
        _style.kerning = kerning;
        rebuildText();
    }

    void TextField::setFontSize(int size)
    {
        _style.fontSize = size;
        rebuildText();
    }

    void TextField::setStyleColor(const QColor& color)
    {
        _style.color = color;
        rebuildText();
    }

    void TextField::setOptions(size_t opt)
    {
        _style.options = opt;
        rebuildText();
    }

    const ResFont* TextField::getFont() const
    {
        return _style.font;
    }

    void TextField::setFont(const ResFont* font)
    {
        _style.font = font;
        if (!_style.font)
        {
            _style.font = _defaultFont;
        }
        rebuildText();
    }

    void TextField::setHAlign(TextStyle::HorizontalAlign align)
    {
        _style.hAlign = align;
        rebuildText();
    }

    void TextField::setAlign(TextStyle::VerticalAlign vAlign, TextStyle::HorizontalAlign hAlign)
    {
        _style.vAlign = vAlign;
        _style.hAlign = hAlign;
        rebuildText();
    }

    void TextField::setStyle(const TextStyle& st)
    {
        TextStyle::HorizontalAlign halign = _style.hAlign;
        TextStyle::VerticalAlign valign = _style.vAlign;
        int size = _style.fontSize;
        _style = st;

        if (st.hAlign == TextStyle::HALIGN_DEFAULT)
        {
            _style.hAlign = halign;
        }
        if (st.vAlign == TextStyle::VALIGN_DEFAULT)
        {
            _style.vAlign = valign;
        }

        if (st.fontSize == 0)
        {
            _style.fontSize = size;
        }

        if (!_style.font)
        {
            _style.font = _defaultFont;
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
        _root->updateMaterial(*_mat.get());
    }

    void TextField::setText(QString str)
    {
        m_htmlText = false;
        if (_text != str)
        {
            _text = str;
            rebuildText();
        }
    }

    QString TextField::getText() const
    {
         QString ret = _text;
         return ret;
    }

    void TextField::setHtmlText(QString str)
    {
        m_htmlText = true;
        if (_text != str)
        {
            _text = str;
            rebuildText();
        }
    }

    int TextField::getFontSize() const
    {
        return _style.fontSize;
    }

    int TextField::getLinesOffset() const
    {
        return _style.linesOffset;
    }

    TextStyle::VerticalAlign    TextField::getVAlign() const
    {
        return _style.vAlign;
    }

    TextStyle::HorizontalAlign  TextField::getHAlign() const
    {
        return _style.hAlign;
    }

    bool TextField::getMultiline() const
    {
        return _style.multiline;
    }

    bool TextField::getBreakLongWords() const
    {
        return _style.breakLongWords;
    }

    int TextField::getKerning() const
    {
        return _style.kerning;
    }

    const QColor& TextField::getStyleColor() const
    {
        return _style.color;
    }

    float TextField::getBaselineScale() const
    {
        return _style.baselineScale;
    }

    size_t TextField::getOptions() const
    {
        return _style.options;
    }

    text::Symbol* TextField::getSymbolAt(int pos) const
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
        if (!_style.font)
        {
            return _root.get();
        }
        return _root.get();
    }

    void TextField::rebuildText()
    {
        float scale = 1.0f;
        if (_style.font != nullptr)
        {
            const Font* font = _style.font->getClosestFont(scale, _style.fontSize, scale);
            if (font)
            {
                m_Locked.lock();
                _rtscale = scale;
                //_realFontSize = fontSize;
                _root = nullptr;
                if (m_htmlText)
                {
                    text::TextBuilder b;
                    _root = b.parse(_text);
                }
                else
                {
                    _root = new text::TextNode(_text);
                }
                text::Aligner rd(_style, _mat, font, scale, getSize());
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
        _rdelegate->doRender(this, rs);
    }
}
