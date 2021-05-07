#include "label.h"

#include "coreengine/mainapp.h"
#include "resource_management/fontmanager.h"

Label::Label(qint32 width)
{
    setObjectName("Label");
    m_clipRect = oxygine::spClipRectActor::create();
    m_clipRect->setWidth(width);
    m_clipRect->setHeight(28);
    m_textField = oxygine::spTextField::create();
    m_textField->setWidth(width);
    Label::setWidth(width);
    m_clipRect->addChild(m_textField);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
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
    m_textField->setHtmlText(str);
    setTooltipText(str);    
}

void Label::setHtmlText(QString str)
{    
    m_textField->setHtmlText(str);
    m_clipRect->setHeight(getTextRect().getHeight() * 1.4f);
    setTooltipText(str);    
}

void Label::setStyle(const oxygine::TextStyle& st)
{    
    m_textField->setStyle(st);
    m_clipRect->setHeight(getTextRect().getHeight() * 1.4f);    
}
