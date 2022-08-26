#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h"

#include <QMutexLocker>

namespace oxygine
{
#ifndef GRAPHICSUPPORT
        TextStyle TextField::m_dummyTextStyle;
        Rect TextField::m_dummyRect;
        QString TextField::m_dummyText;
#endif

    bool TextField::isOn(const Vector2& localPosition, float)
    {
#ifdef GRAPHICSUPPORT
        Rect r = getTextRect();
        r.expand(Point(m_extendedIsOn, m_extendedIsOn), Point(m_extendedIsOn, m_extendedIsOn));
        return r.pointIn(Point((int)localPosition.x, (int)localPosition.y));
#else
        return false;
#endif
    }

    void TextField::setVAlign(TextStyle::VerticalAlign align)
    {
#ifdef GRAPHICSUPPORT
        m_style.vAlign = align;
        rebuildText();
#endif
    }

    void TextField::setMultiline(bool multiline)
    {
#ifdef GRAPHICSUPPORT
        m_style.multiline = multiline;
        rebuildText();
#endif
    }

    void TextField::setBreakLongWords(bool val)
    {
#ifdef GRAPHICSUPPORT
        m_style.breakLongWords = val;
        rebuildText();
#endif
    }

    void TextField::setLinesOffset(qint32 offset)
    {
#ifdef GRAPHICSUPPORT
        m_style.linesOffset = offset;
        rebuildText();
#endif
    }

    void TextField::setBaselineScale(float s)
    {
#ifdef GRAPHICSUPPORT
        m_style.baselineScale = s;
        rebuildText();
#endif
    }

    void TextField::setKerning(qint32 kerning)
    {
#ifdef GRAPHICSUPPORT
        m_style.kerning = kerning;
        rebuildText();
#endif
    }

    void TextField::setFontSize(qint32 size)
    {
#ifdef GRAPHICSUPPORT
        m_style.fontSize = size;
        rebuildText();
#endif
    }

    void TextField::setStyleColor(const QColor& color)
    {
#ifdef GRAPHICSUPPORT
        m_style.color = color;
        rebuildText();
#endif
    }

    const ResFont* TextField::getFont() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.font;
#else
        return nullptr;
#endif
    }

    void TextField::setFont(const ResFont* font)
    {
#ifdef GRAPHICSUPPORT
        m_style.font = font;
        if (!m_style.font)
        {
            m_style.font = nullptr;
        }
        rebuildText();
#endif
    }

    void TextField::setHAlign(TextStyle::HorizontalAlign align)
    {
#ifdef GRAPHICSUPPORT
        m_style.hAlign = align;
        rebuildText();
#endif
    }

    void TextField::setAlign(TextStyle::VerticalAlign vAlign, TextStyle::HorizontalAlign hAlign)
    {
#ifdef GRAPHICSUPPORT
        m_style.vAlign = vAlign;
        m_style.hAlign = hAlign;
        rebuildText();
#endif
    }

    void TextField::setStyle(const TextStyle& st)
    {
#ifdef GRAPHICSUPPORT
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
            m_style.font = nullptr;
        }
        rebuildText();
#endif
    }

    void TextField::sizeChanged(const Vector2&)
    {
        rebuildText();
    }

    void TextField::matChanged()
    {
#ifdef GRAPHICSUPPORT
        if (!m_root)
        {
            return;
        }
        m_root->updateMaterial(*m_mat.get());
#endif
    }

    void TextField::setText(const QString & str)
    {
#ifdef GRAPHICSUPPORT
        m_htmlText = false;
        if (m_text != str)
        {
            m_text = str;
            rebuildText();
        }
#endif
    }

    const QString & TextField::getText() const
    {
#ifdef GRAPHICSUPPORT
         return m_text;
#else
        return m_dummyText;
#endif
    }

    void TextField::setHtmlText(const QString & str)
    {
#ifdef GRAPHICSUPPORT
        m_htmlText = true;
        if (m_text != str)
        {
            m_text = str;
            rebuildText();
        }
#endif
    }

    qint32 TextField::getFontSize() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.fontSize;
#else
        return 0;
#endif
    }

    qint32 TextField::getLinesOffset() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.linesOffset;
#else
        return 0;
#endif
    }

    TextStyle::VerticalAlign TextField::getVAlign() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.vAlign;
#else
        return TextStyle::VerticalAlign::VALIGN_DEFAULT;
#endif
    }

    TextStyle::HorizontalAlign TextField::getHAlign() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.hAlign;
#else
        return TextStyle::HorizontalAlign::HALIGN_DEFAULT;
#endif
    }

    bool TextField::getMultiline() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.multiline;
#else
        return false;
#endif
    }

    bool TextField::getBreakLongWords() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.breakLongWords;
#else
        return false;
#endif
    }

    qint32 TextField::getKerning() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.kerning;
#else
        return 0;
#endif
    }

    const QColor& TextField::getStyleColor() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.color;
#else
        return m_dummyColor;
#endif
    }

    float TextField::getBaselineScale() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.baselineScale;
#else
        return 0;
#endif
    }

    text::Symbol* TextField::getSymbolAt(qint32 pos) const
    {
#ifdef GRAPHICSUPPORT
        return const_cast<TextField*>(this)->getRootNode()->getSymbol(pos);
#else
        return nullptr;
#endif
    }

    const Rect& TextField::getTextRect() const
    {
#ifdef GRAPHICSUPPORT
        const_cast<TextField*>(this)->getRootNode();
        return m_textRect;
#else
        return m_dummyRect;
#endif
    }

    bool TextField::getBounds(RectF& r) const
    {
#ifdef GRAPHICSUPPORT
        r = getTextRect().cast<RectF>();
#endif
        return true;
    }

    text::Node* TextField::getRootNode()
    {
#ifdef GRAPHICSUPPORT
        if (!m_style.font)
        {
            return m_root.get();
        }
        return m_root.get();
#else
        return nullptr;
#endif
    }

    void TextField::rebuildText()
    {
#ifdef GRAPHICSUPPORT
        QMutexLocker lock(&m_Locked);
        float scale = 1.0f;
        if (m_style.font != nullptr)
        {
            const Font* font = m_style.font->getClosestFont(scale, m_style.fontSize, scale);
            if (font)
            {
                m_rtscale = scale;
                m_root = nullptr;
                if (m_htmlText)
                {
                    text::TextBuilder b;
                    m_root = b.parse(m_text);
                }
                else
                {
                    m_root = text::spTextNode::create(m_text);
                }
                text::Aligner rd(m_style, m_mat, font, scale, getSize());
                rd.begin();
                m_root->resize(rd);
                rd.end();
                m_root->finalPass(rd);
                rd.setBounds((rd.getBounds().cast<RectF>() / rd.getScale()).cast<Rect>());
                m_textRect = rd.getBounds();
            }
        }
#endif
    }

    void TextField::doRender(RenderState const& rs)
    {
        RenderDelegate::instance->doRender(this, rs);
    }
}
