#include "objects/base/label.h"

#include "coreengine/interpreter.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

Label::Label(qint32 width, bool showBox)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Label");
#endif
    Interpreter::setCppOwnerShip(this);

    qint32 xOffset = 0;
    qint32 yOffset = 0;
    if (showBox)
    {
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("dropdownmenu");
        oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
        pBox->setResAnim(pAnim);
        pBox->setSize(width, 40);
        addChild(pBox);
        xOffset = 5;
        yOffset = 5;
    }
    m_clipRect = oxygine::spClipRectActor::create();
    m_clipRect->setWidth(width);
    m_clipRect->setHeight(28);
    m_clipRect->setPosition(xOffset, yOffset);
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

void Label::setWidth(qint32 width)
{    
    m_clipRect->setWidth(width);
    m_textField->setWidth(width);
    oxygine::Sprite::setWidth(width);    
}

void Label::setHeight(qint32 height)
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
    qint32 height = getTextRect().height();
    m_clipRect->setHeight(height);
    oxygine::Sprite::setHeight(height);
    setTooltipText(str);
#endif
}

void Label::setStyle(const oxygine::TextStyle& st)
{    
#ifdef GRAPHICSUPPORT
    m_textField->setStyle(st);
    m_clipRect->setPosition(m_clipRect->getPosition() + QPoint(0, -m_clipOffset));
    m_clipOffset = st.font->offsetY;
    m_clipRect->setPosition(m_clipRect->getPosition() + QPoint(0, m_clipOffset));
    qint32 height = getTextRect().height();
    m_clipRect->setHeight(height);
    oxygine::Sprite::setHeight(height);
#endif
}

oxygine::TextStyle Label::getStyle()
{
    return m_textField->getStyle();
}
