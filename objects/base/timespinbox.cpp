#include "objects/base/timespinbox.h"
#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include <QTime>

TimeSpinBox::TimeSpinBox(qint32 width)
{
    setObjectName("TimeSpinBox");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());

    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    m_Textbox = oxygine::spBox9Sprite::create();
    m_Textbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setResAnim(pAnim);
    m_Textfield = oxygine::spTextField::create();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_Textfield->setStyle(style);
    m_Textfield->setHtmlText("");


    oxygine::spClipRectActor pClipActor = oxygine::spClipRectActor::create();

    pClipActor->addChild(m_Textfield);
    m_Textbox->addChild(pClipActor);
    m_Textbox->setSize(width - 35, 40);
    setSize(width, 40);
    m_Textfield->setWidth(m_Textbox->getWidth() - 20);
    m_Textfield->setHeight(m_Textbox->getHeight());
    pClipActor->setSize(m_Textfield->getSize());
    pClipActor->setX(10);
    pClipActor->setY(5);
    addChild(m_Textbox);

    m_pSpinBox = oxygine::spBox9Sprite::create();
    m_pSpinBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpinBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpinBox->setResAnim(pAnim);
    m_pSpinBox->setSize(width - m_Textbox->getWidth(), 40);
    m_pSpinBox->setX(m_Textbox->getWidth());

    m_pArrowDown = oxygine::spButton::create();
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowDown->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    auto* pArrowDown = m_pArrowDown.get();
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event*)
    {
        if (m_spinDirection == 0)
        {
            m_spinDirection = -1 * m_SpinSpeed;
            qint32 value = getCurrentValue();
            value += m_spinDirection;
            setCurrentValue(value);
            m_spinTimer.start();
            emit sigValueChanged(getCurrentValue());
        }
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event*)
    {
        m_spinDirection = 0;
    });
    m_pSpinBox->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(9, m_pSpinBox->getHeight() - m_pArrowDown->getHeight() - 8);

    m_pArrowUp = oxygine::spButton::create();
    // pButton->setPosition(200, 200);
    m_pArrowUp->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowUp->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pArrowUp->setFlippedY(true);
    auto* pArrowUp = m_pArrowUp.get();
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event*)
    {
        if (m_spinDirection == 0)
        {
            m_spinDirection = 1 * m_SpinSpeed;
            qint32 value = getCurrentValue();
            value += m_spinDirection;
            setCurrentValue(value);
            m_spinTimer.start();
            emit sigValueChanged(getCurrentValue());
        }
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event*)
    {
        m_spinDirection = 0;
    });
    m_pSpinBox->addChild(m_pArrowUp);
    m_pArrowUp->setPosition(9, 8);

    addChild(m_pSpinBox);


    addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        emit sigFocused();
    });
    m_spinTimer.start();
    setCurrentValue(0);
}

void TimeSpinBox::focusedLost()
{
    qint32 value = checkInput();
    m_Textfield->setX(0);
    emit sigValueChanged(value);
    TextInput::focusedLost();
}

void TimeSpinBox::setEnabled(bool value)
{    
    oxygine::Actor::setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_pArrowUp->setEnabled(value);
    m_Textbox->setEnabled(value);
    m_pSpinBox->setEnabled(value);
    m_focused = false;
}

void TimeSpinBox::setCurrentValue(qint32 value)
{
    setValue(value);
    checkInput();
}

void TimeSpinBox::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    QString drawText = getDrawText(getCurrentText());
    if(m_focused)
    {
        // create output text
        qint32 curmsgpos = getCursorPosition();
        m_Textfield->setHtmlText(drawText);

        if (drawText.size() > 0)
        {
            // calc text field position based on curmsgpos
            qint32 xPos = 0;
            qint32 fontWidth = m_Textfield->getTextRect().getWidth() / drawText.size();
            qint32 boxSize = (m_Textbox->getWidth() - 5 - fontWidth);
            xPos = -fontWidth * curmsgpos + boxSize / 2;
            if (xPos > 0)
            {
                xPos = 0;
            }
            else if ((drawText.size() - curmsgpos + 1) * fontWidth < boxSize)
            {
                xPos = m_Textbox->getWidth() - m_Textfield->getTextRect().getWidth() - fontWidth * 1;
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
        if (m_spinDirection != 0.0)
        {
            if (m_spinTimer.elapsed() > BLINKFREQG)
            {
                qint32 value = getCurrentValue();
                value += m_spinDirection;
                setValue(value);
                m_spinTimer.start();
            }
            checkInput();
        }
        m_Textfield->setHtmlText(drawText);
    }
    oxygine::Actor::update(us);
}

qint32 TimeSpinBox::getCurrentValue()
{
    qint32 value = 0;
    value = QTime::fromString(getCurrentText(), "hh:mm:ss").msecsSinceStartOfDay();
    return value;
}

qint32 TimeSpinBox::checkInput()
{
    qint32 value = QTime::fromString(getCurrentText(), "hh:mm:ss").msecsSinceStartOfDay();
    setValue(value);
    return value;
}

void TimeSpinBox::setValue(qint32 value)
{    
    QString text = QTime::fromMSecsSinceStartOfDay(value).toString("hh:mm:ss");
    setCurrentText(text);
    m_Textfield->setHtmlText(text);
}

qint32 TimeSpinBox::getSpinSpeed() const
{
    return m_SpinSpeed;
}

void TimeSpinBox::setSpinSpeed(qint32 SpinSpeed)
{
    m_SpinSpeed = SpinSpeed;
}

bool TimeSpinBox::onEditFinished()
{
    qint32 value = checkInput();
    emit sigValueChanged(value);
    return true;
}
