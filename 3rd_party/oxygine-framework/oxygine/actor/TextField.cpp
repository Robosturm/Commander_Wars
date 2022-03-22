#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h"

#include <QMutexLocker>

namespace oxygine
{
    TextField::TextField():
        m_root(nullptr),
        m_textRect(0, 0, 0, 0)
    {
        setText("");
    }

    bool TextField::isOn(const Vector2& localPosition, float)
    {
        Rect r = getTextRect();
        r.expand(Point(m_extendedIsOn, m_extendedIsOn), Point(m_extendedIsOn, m_extendedIsOn));
        return r.pointIn(Point((int)localPosition.x, (int)localPosition.y));
    }

    void TextField::setMultiline(bool multiline)
    {
        m_style.multiline = multiline;
        rebuildText();
    }

    void TextField::setStyleColor(const QColor& color)
    {
        m_style.color = color;
        rebuildText();
    }

    const QFont & TextField::getFont() const
    {
        return m_style.font;
    }

    void TextField::setFont(QFont & font)
    {
        m_style.font = font;
        rebuildText();
    }

    void TextField::setHAlign(TextStyle::HorizontalAlign align)
    {
        m_style.hAlign = align;
        rebuildText();
    }

    void TextField::setStyle(const TextStyle& st)
    {
        TextStyle::HorizontalAlign halign = m_style.hAlign;
        m_style = st;

        if (st.hAlign == TextStyle::HALIGN_DEFAULT)
        {
            m_style.hAlign = halign;
        }
        rebuildText();
    }

    void TextField::sizeChanged(const Vector2&)
    {
        rebuildText();
    }

    void TextField::setText(const QString & str)
    {
        m_htmlText = false;
        if (m_text != str)
        {
            m_text = str;
            rebuildText();
        }
    }

    const QString & TextField::getText() const
    {
         return m_text;
    }

    void TextField::setHtmlText(const QString & str)
    {
        m_htmlText = true;
        if (m_text != str)
        {
            m_text = str;
            rebuildText();
        }
    }

    TextStyle::HorizontalAlign TextField::getHAlign() const
    {
        return m_style.hAlign;
    }

    bool TextField::getMultiline() const
    {
        return m_style.multiline;
    }

    const QColor& TextField::getStyleColor() const
    {
        return m_style.color;
    }

    const Rect& TextField::getTextRect() const
    {
        const_cast<TextField*>(this)->getRootNode();
        return m_textRect;
    }

    bool TextField::getBounds(RectF& r) const
    {
        r = getTextRect().cast<RectF>();
        return true;
    }

    text::Node* TextField::getRootNode()
    {
        return m_root.get();
    }

    void TextField::rebuildText()
    {
        QMutexLocker lock(&m_Locked);
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
        text::Aligner rd(m_style, getSize());
        rd.begin();
        m_root->resize(rd);
        rd.end();
        m_textRect = rd.getBounds().cast<Rect>();
    }

    void TextField::doRender(RenderState const& rs)
    {
        m_rdelegate->doRender(this, rs);
    }
}
