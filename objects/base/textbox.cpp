#include "objects/base/textbox.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/interpreter.h"

#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"

Textbox::Textbox(qint32 width, qint32 heigth)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Textbox");
#endif
    Interpreter::setCppOwnerShip(this);

    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    m_Textbox = MemoryManagement::create<oxygine::Box9Sprite>();
    m_Textbox->setResAnim(pAnim);
    m_Textfield = MemoryManagement::create<oxygine::TextField>();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = (heigth > 0);
    style.elideText = Qt::TextElideMode::ElideNone;
    m_Textfield->setStyle(style);
    m_Textfield->setHtmlText("");


    oxygine::spClipRectActor pClipActor = MemoryManagement::create<oxygine::ClipRectActor>();

    if (heigth < 0)
    {
        heigth = 40;
    }
    pClipActor->addChild(m_Textfield);
    m_Textbox->addChild(pClipActor);
    m_Textbox->setSize(width, heigth);
    setSize(width, heigth);
    m_Textfield->setWidth(0);
    m_Textfield->setHeight(m_Textbox->getScaledHeight());
    pClipActor->setSize(m_Textbox->getScaledWidth() - 20, m_Textbox->getScaledHeight());
    pClipActor->setX(10);
    pClipActor->setY(5);

    addChild(m_Textbox);
#ifdef GRAPHICSUPPORT
    addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event* pEvent)
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            qint32 x = pTouchEvent->localPosition.x() - m_Textfield->getX();
            m_focusPosition = getClickedLinePosition(x, getCurrentText(), m_Textfield->getStyle().font->font);
        }
        pEvent->stopPropagation();
        if (FocusableObject::getFocusedObject() == this)
        {
            setCursorPosition(m_focusPosition);
        }
        else
        {
            emit sigFocused();
        }
    });
#endif
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
#ifdef GRAPHICSUPPORT
    // no need to calculate more than we need if we're invisible
    const QString currentText = getCurrentText();
    QString drawText = getDrawText(currentText);
    if (m_lastDrawText != drawText)
    {
        m_Textfield->setHtmlText(drawText);
        m_lastDrawText = drawText;
        if(m_focused)
        {
            qint32 curmsgpos = getCursorPosition();
            if (drawText.size() > 0)
            {
                // calc text field position based on curmsgpos
                qint32 xPos = 0;
                constexpr qint32 charSpaceCount = 2;
                QFontMetrics metrics(m_Textfield->getFont()->font);
                const auto averageCharSize = metrics.averageCharWidth();
                qint32 boxSize = (m_Textbox->getScaledWidth() - 40 -  averageCharSize);
                QRect rect = metrics.boundingRect(currentText.mid(0, curmsgpos));
                xPos = boxSize -rect.width() - averageCharSize * charSpaceCount;
                if (xPos > 0)
                {
                    xPos = 0;
                }
                else if (m_Textfield->getTextRect().width() - averageCharSize * charSpaceCount < boxSize)
                {
                    xPos = m_Textbox->getScaledWidth() - m_Textfield->getTextRect().width() - averageCharSize * charSpaceCount;
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
    }
#endif
    oxygine::Actor::update(us);
}

void Textbox::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_Textbox->setEnabled(value);
    m_Textfield->setEnabled(value);
}
