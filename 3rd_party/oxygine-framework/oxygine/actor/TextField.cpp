#include "TextField.h"
#include "../Font.h"
#include "../res/ResFont.h"
#include "../RenderDelegate.h"
#include "../RenderState.h"
#include "../core/gl/VideoDriverGLES20.h"
#include "../res/Resources.h"
#include "../text_utils/Node.h"
#include "../text_utils/TextBuilder.h"
#include <sstream>

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
        _flags |= flag_rebuild;
        _style.font = _defaultFont;
    }

    TextField::~TextField()
    {
        delete _root;
        _root = nullptr;
    }

    void TextField::copyFrom(const TextField& src, cloneOptions opt)
    {
        VStyleActor::copyFrom(src, opt);
        _text = src._text;
        _style = src._style;
        _root = nullptr;
        _rtscale = 1.0f;

        _flags |= flag_rebuild;
        _textRect = src._textRect;
    }

    bool TextField::isOn(const Vector2& localPosition, float localScale)
    {
        Rect r = getTextRect(localScale);
        r.expand(Point(_extendedIsOn, _extendedIsOn), Point(_extendedIsOn, _extendedIsOn));
        return r.pointIn(Point((int)localPosition.x, (int)localPosition.y));
    }

    void TextField::needRebuild()
    {
        _flags |= flag_rebuild;
    }

    void TextField::setVAlign(TextStyle::VerticalAlign align)
    {
        _style.vAlign = align;
        needRebuild();
    }

    void TextField::setMultiline(bool multiline)
    {
        _style.multiline = multiline;
        needRebuild();
    }

    void TextField::setBreakLongWords(bool val)
    {
        _style.breakLongWords = val;
        needRebuild();
    }

    void TextField::setLinesOffset(int offset)
    {
        _style.linesOffset = offset;
        needRebuild();
    }

    void TextField::setBaselineScale(float s)
    {
        _style.baselineScale = s;
        needRebuild();
    }

    void TextField::setKerning(int kerning)
    {
        _style.kerning = kerning;
        needRebuild();
    }

    void TextField::setFontSize(int size)
    {
        _style.fontSize = size;
        needRebuild();
    }

    void TextField::setStyleColor(const QColor& color)
    {
        _style.color = color;
        needRebuild();
    }

    void TextField::setOptions(size_t opt)
    {
        _style.options = opt;
        needRebuild();
    }

    const ResFont* TextField::getFont() const
    {
        return _style.font;
    }

    void TextField::setFont(const ResFont* font)
    {
        _style.font = font;
        if (!_style.font)
            _style.font = _defaultFont;
        needRebuild();
    }

    void TextField::setHAlign(TextStyle::HorizontalAlign align)
    {
        _style.hAlign = align;
        needRebuild();
    }

    void TextField::setAlign(TextStyle::VerticalAlign vAlign, TextStyle::HorizontalAlign hAlign)
    {
        _style.vAlign = vAlign;
        _style.hAlign = hAlign;
        needRebuild();
    }

    void TextField::setStyle(const TextStyle& st)
    {
        TextStyle::HorizontalAlign halign = _style.hAlign;
        TextStyle::VerticalAlign valign = _style.vAlign;
        int size = _style.fontSize;
        _style = st;

        if (st.hAlign == TextStyle::HALIGN_DEFAULT)
            _style.hAlign = halign;

        if (st.vAlign == TextStyle::VALIGN_DEFAULT)
            _style.vAlign = valign;

        if (st.fontSize == 0)
            _style.fontSize = size;

        if (!_style.font)
            _style.font = _defaultFont;

        needRebuild();
    }

    void TextField::sizeChanged(const Vector2& size)
    {
        needRebuild();
    }

    void TextField::matChanged()
    {
        qWarning("not optimal");
        if (_flags & flag_rebuild)
            return;

        if (!_root)
            return;

        _root->updateMaterial(*_mat.get());
    }

    void TextField::setText(QString str)
    {
        _flags &= ~flag_html;
        if (_text != str)
        {
            _text = str;
            needRebuild();
        }
    }

    void TextField::setHtmlText(QString str)
    {
        _flags |= flag_html;
        if (_text != str)
        {
            _text = str;
            needRebuild();
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
        return const_cast<TextField*>(this)->getRootNode(_rtscale)->getSymbol(pos);
    }

    const Rect& TextField::getTextRect(float localScale) const
    {
        const_cast<TextField*>(this)->getRootNode(localScale);
        return _textRect;
    }

    bool TextField::getBounds(RectF& r) const
    {
        r = getTextRect().cast<RectF>();
        return true;
    }


    text::Node* TextField::getRootNode(float globalScale)
    {
        if (!_style.font)
            return _root;


        globalScale = qAbs(globalScale);

        float scale = 1.0f;
        const Font* font = _style.font->getClosestFont(globalScale, _style.fontSize, scale);

        if ((_flags & flag_rebuild || _rtscale != scale) && font)
        {
            _rtscale = scale;
            //_realFontSize = fontSize;
            delete _root;

            _flags &= ~flag_rebuild;

            if (_flags & flag_html)
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

            Event ev(EVENT_REBUILD);
            dispatchEvent(&ev);
        }

        return _root;
    }

    void TextField::doRender(RenderState const& rs)
    {
        _rdelegate->doRender(this, rs);
    }
}
