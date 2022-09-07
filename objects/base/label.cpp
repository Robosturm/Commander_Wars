#include "label.h"

#include "coreengine/mainapp.h"
#include "resource_management/fontmanager.h"

Label::Label(qint32 width)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Label");
#endif
    m_clipRect = oxygine::spClipRectActor::create();
    m_clipRect->setWidth(width);
    m_clipRect->setHeight(28);
    m_textField = oxygine::spTextField::create();
    m_textField->setWidth(width);
    Label::setWidth(width);
    m_clipRect->addChild(m_textField);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    setStyle(style);
    addChild(m_clipRect);
}

void Label::setWidth(float width)
{    
    m_clipRect->setWidth(width);
    m_textField->setWidth(width);
    oxygine::Sprite::setWidth(width);    
}

void Label::setHeight(float height)
{
    m_clipRect->setHeight(height);
    m_textField->setHeight(height);
    oxygine::Sprite::setHeight(height);
}

void Label::setText(QString str)
{    
    m_textField->setText(str);
}

void Label::setHtmlText(QString str)
{
#ifdef GRAPHICSUPPORT
    m_textField->setHtmlText(str);
    qint32 height = getTextRect().getHeight();
    qint32 fontHeight = m_textField->getStyle().font.borderWidth;
    if (fontHeight < 0)
    {
        height += qAbs(fontHeight) * 2;
    }
    m_clipRect->setHeight(height);
    oxygine::Sprite::setHeight(height);
    setTooltipText(str);    
#endif
}

void Label::setStyle(const oxygine::TextStyle& st)
{    
#ifdef GRAPHICSUPPORT
    m_textField->setStyle(st);
    qint32 height = getTextRect().getHeight();
    qint32 fontHeight = m_textField->getStyle().font.borderWidth;
    if (fontHeight < 0)
    {
        height += qAbs(fontHeight) * 2;
    }
    m_clipRect->setHeight(height);
    oxygine::Sprite::setHeight(height);
#endif
}

oxygine::TextStyle Label::getStyle()
{
    return m_textField->getStyle();
}
