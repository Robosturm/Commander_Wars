#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/TextBuilder.h"
#include "resource_management/fontmanager.h"

#include <QMutexLocker>

namespace oxygine
{
#ifndef GRAPHICSUPPORT
        QColor TextField::m_dummyColor;
        TextStyle TextField::m_dummyTextStyle = TextStyle(oxygine::Font());
        QRect TextField::m_dummyRect;
        QString TextField::m_dummyText{""};
#endif
    TextField::TextField()
#ifdef GRAPHICSUPPORT
        : m_style(FontManager::getMainFont24())
#endif
    {
    }

    bool TextField::isOn(const QPoint& localPosition, float)
    {
#ifdef GRAPHICSUPPORT
        return getTextRect().contains(localPosition);
#else
        return false;
#endif
    }

    void TextField::setMultiline(bool multiline)
    {
#ifdef GRAPHICSUPPORT
        m_style.multiline = multiline;
        rebuildText();
#endif
    }

    void TextField::setStyleColor(const QColor& color)
    {
#ifdef GRAPHICSUPPORT
        m_style.color = color;
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

    const Font & TextField::getFont() const
    {
#ifdef GRAPHICSUPPORT
        return m_style.font;
#else
        return m_dummyTextStyle.font;
#endif
    }

    void TextField::setFont(Font & font)
    {
#ifdef GRAPHICSUPPORT
        m_style.font = font;
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

    void TextField::setStyle(const TextStyle& st)
    {
#ifdef GRAPHICSUPPORT
        TextStyle::HorizontalAlign halign = m_style.hAlign;
        m_style = st;

        if (st.hAlign == TextStyle::HALIGN_DEFAULT)
        {
            m_style.hAlign = halign;
        }
        rebuildText();
#endif
    }

    void TextField::sizeChanged(const QSize&)
    {
        rebuildText();
    }

    void TextField::setText(const QString & str, bool lock)
    {
#ifdef GRAPHICSUPPORT
        m_htmlText = false;
        if (m_text != str)
        {
            m_text = str;
            rebuildText(lock);
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

    const QRect& TextField::getTextRect() const
    {
#ifdef GRAPHICSUPPORT
        return m_textRect;
#else
        return m_dummyRect;
#endif
    }

    bool TextField::getBounds(QRect& r) const
    {
#ifdef GRAPHICSUPPORT
        r = getTextRect();
#endif
        return true;
    }

    text::Node* TextField::getRootNode()
    {
#ifdef GRAPHICSUPPORT
        return m_root.get();
#else
        return nullptr;
#endif
    }

    void TextField::rebuildText(bool lock)
    {
#ifdef GRAPHICSUPPORT
        if (lock)
        {
            m_Locked.lock();
        }
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
        rd.align(*m_root.get());
        m_textRect = rd.getBounds();
        if (lock)
        {
            m_Locked.unlock();
        }
#endif
    }

    void TextField::doRender(RenderState const& rs)
    {
        RenderDelegate::instance->doRender(this, rs);
    }
}
