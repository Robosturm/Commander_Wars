#include "objects/base/textbox.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

Textbox::Textbox(qint32 width, qint32 heigth)
{
    setObjectName("Textbox");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());

    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    m_Textbox = oxygine::spBox9Sprite::create();
    m_Textbox->setResAnim(pAnim);
    m_Textfield = oxygine::spTextField::create();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = (heigth > 0);
    m_Textfield->setStyle(style);
    m_Textfield->setHtmlText("");


    oxygine::spClipRectActor pClipActor = oxygine::spClipRectActor::create();

    if (heigth < 0)
    {
        heigth = 40;
    }
    pClipActor->addChild(m_Textfield);
    m_Textbox->addChild(pClipActor);
    m_Textbox->setSize(width, heigth);
    setSize(width, heigth);
    m_Textfield->setWidth(m_Textbox->getWidth() - 20);
    m_Textfield->setHeight(m_Textbox->getHeight());
    pClipActor->setSize(m_Textfield->getSize());
    pClipActor->setX(10);
    pClipActor->setY(5);

    addChild(m_Textbox);
    addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event* event)
    {
        event->stopPropagation();
        emit sigFocused();
    });
}

bool Textbox::onEditFinished()
{
    emit sigTextChanged(getCurrentText());
    emit sigEnterPressed(getCurrentText());
    return true;
}

void Textbox::focusedLost()
{
    emit sigTextChanged(getCurrentText());
    TextInput::focusedLost();
}

void Textbox::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    QString drawText = getDrawText(getCurrentText());
    if(m_focused)
    {
        qint32 curmsgpos = getCursorPosition();
        m_Textfield->setHtmlText(drawText);
        if (drawText.size() > 0)
        {
            // calc text field position based on curmsgpos
            qint32 xPos = 0;
            qint32 fontWidth = m_Textfield->getTextRect().getWidth() / drawText.size();
            qint32 boxSize = (m_Textbox->getWidth() - 40 - fontWidth);
            xPos = -fontWidth * curmsgpos + boxSize / 2;
            if (xPos > 0)
            {
                xPos = 0;
            }
            else if ((drawText.size() - curmsgpos + 3) * fontWidth < boxSize)
            {
                xPos = m_Textbox->getWidth() - m_Textfield->getTextRect().getWidth() - fontWidth * 3;
                if (xPos > 0)
                {
                    xPos = 0;
                }
            }
            else
            {
                // all fine
            }
            m_Textfield->setX(xPos);
        }
    }
    else
    {
        m_Textfield->setHtmlText(drawText);
    }
    qint32 width = m_Textfield->getTextRect().getWidth();
    if (width != m_Textfield->getWidth())
    {
        m_Textfield->setWidth(m_Textfield->getTextRect().getWidth());
    }
    oxygine::Actor::update(us);
}

void Textbox::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_Textbox->setEnabled(value);
    m_Textfield->setEnabled(value);
}
