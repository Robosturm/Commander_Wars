#include "objects/base/spinbox.h"

#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"

SpinBox::SpinBox(qint32 width, qreal min, qreal max, Mode mode)
    : m_MinValue(min),
      m_MaxValue(max),
      m_Mode(mode)
{
#ifdef GRAPHICSUPPORT
    setObjectName("SpinBox");
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
    style.multiline = false;
    m_Textfield->setStyle(style);
    m_Textfield->setHtmlText("");


    oxygine::spClipRectActor pClipActor = MemoryManagement::create<oxygine::ClipRectActor>();

    pClipActor->addChild(m_Textfield);
    m_Textbox->addChild(pClipActor);
    m_Textbox->setSize(width - 35, 40);
    setSize(width, 40);
    m_Textfield->setWidth(m_Textbox->getScaledWidth() - 20);
    m_Textfield->setHeight(m_Textbox->getScaledHeight());
    pClipActor->setSize(m_Textfield->getScaledSize());
    pClipActor->setX(10);
    pClipActor->setY(5);
    addChild(m_Textbox);

    m_pSpinBox = MemoryManagement::create<oxygine::Box9Sprite>();
    m_pSpinBox->setResAnim(pAnim);
    m_pSpinBox->setSize(width - m_Textbox->getScaledWidth(), 40);
    m_pSpinBox->setX(m_Textbox->getScaledWidth());

    m_pArrowDown = MemoryManagement::create<oxygine::Button>();
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowDown->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    auto* pArrowDown = m_pArrowDown.get();
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [pArrowDown](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [pArrowDown](oxygine::Event*)
    {
        pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event*)
    {
        if (m_spinDirection == 0)
        {
            m_spinDirection = -1 * m_SpinSpeed;
            qreal value = getCurrentValue();
            value += m_spinDirection;
            setCurrentValue(value);
            m_spinTimer.start();
            emit sigValueChanged(getCurrentValue());
        }
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event*)
    {
        m_spinDirection = 0;
    });
    m_pSpinBox->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(9, m_pSpinBox->getScaledHeight() - m_pArrowDown->getScaledHeight() - 8);

    m_pArrowUp = MemoryManagement::create<oxygine::Button>();
    // pButton->setPosition(200, 200);
    m_pArrowUp->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowUp->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pArrowUp->setFlippedY(true);
    auto* pArrowUp = m_pArrowUp.get();
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OVER, [pArrowUp](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OUTX, [pArrowUp](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event*)
    {
        if (m_spinDirection == 0)
        {
            m_spinDirection = 1 * m_SpinSpeed;
            qreal value = getCurrentValue();
            value += m_spinDirection;
            setCurrentValue(value);
            m_spinTimer.start();
            emit sigValueChanged(getCurrentValue());
        }
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event*)
    {
        m_spinDirection = 0;
    });
    m_pSpinBox->addChild(m_pArrowUp);
    m_pArrowUp->setPosition(9, 8);

    addChild(m_pSpinBox);


    addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFocused();
    });
    m_spinTimer.start();
}

QString SpinBox::getUnit() const
{
    return m_unit;
}

void SpinBox::setUnit(const QString unit)
{
    m_unit = unit;
}

void SpinBox::focusedLost()
{
    qreal value = checkInput();
    m_Textfield->setX(0);
    emit sigValueChanged(value);
    TextInput::focusedLost();
}

void SpinBox::setEnabled(bool value)
{    
    oxygine::Actor::setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_pArrowUp->setEnabled(value);
    m_Textbox->setEnabled(value);
    m_pSpinBox->setEnabled(value);
    m_focused = false;    
}

void SpinBox::setCurrentValue(qreal value)
{
    setValue(value);
    checkInput();
}

void SpinBox::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    QString drawText = getDrawText(getCurrentText() + m_unit);
    if(m_focused)
    {
        qint32 curmsgpos = getCursorPosition();        
        m_Textfield->setHtmlText(drawText + m_unit);

        if (drawText.size() > 0)
        {
            // calc text field position based on curmsgpos
            qint32 xPos = 0;
            qint32 fontWidth = m_Textfield->getTextRect().width() / drawText.size();
            qint32 boxSize = (m_Textbox->getScaledWidth() - 5 - fontWidth);
            xPos = -fontWidth * curmsgpos + boxSize / 2;
            if (xPos > 0)
            {
                xPos = 0;
            }
            else if ((drawText.size() - curmsgpos + 1) * fontWidth < boxSize)
            {
                xPos = m_Textbox->getScaledWidth() - m_Textfield->getTextRect().width() - fontWidth * 1;
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
                qreal value = getCurrentValue();
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

qreal SpinBox::getCurrentValue()
{
    qreal value = 0;
    const QString text = getCurrentText();
    if (text == "∞")
    {
        value = m_InfinityValue;
    }
    else
    {
        bool ok = false;
        value = text.toDouble(&ok);
        if (!ok)
        {
            value = m_InfinityValue;
        }
    }
    return value;
}

qreal SpinBox::checkInput()
{
    CONSOLE_PRINT("SpinBox::checkInput", GameConsole::eDEBUG);
    bool ok = false;
    const QString text = getCurrentText();
    qreal value = text.toDouble(&ok);
    if (text == "∞")
    {
        value = m_InfinityValue;
        ok = true;
    }
    if (!ok)
    {
        value = m_MinValue;
    }
    if (value < m_MinValue)
    {
        value = m_MinValue;
    }
    if (value > m_MaxValue)
    {
        value = m_MaxValue;
    }
    setValue(value);
    return value;
}

void SpinBox::setValue(qreal value)
{    
    QString text = getCurrentText();
    if (value <= m_InfinityValue)
    {
        text = "∞";
    }
    else
    {
        switch (m_Mode)
        {
            case Mode::Int:
            {
                text = QString::number(static_cast<qint32>(value));
                break;
            }
            case Mode::Float:
            case Mode::Real:
            {
                text = QString::number(value, 'f', 2);
                break;
            }
        }
    }
    setCurrentText(text);
    m_Textfield->setHtmlText(text + m_unit);
    
}

qreal SpinBox::getInfinityValue() const
{
    return m_InfinityValue;
}

void SpinBox::setInfinityValue(qreal InfinityValue)
{
    m_InfinityValue = InfinityValue;
}

qreal SpinBox::getSpinSpeed() const
{
    return m_SpinSpeed;
}

void SpinBox::setSpinSpeed(qreal SpinSpeed)
{
    m_SpinSpeed = SpinSpeed;
}

bool SpinBox::onEditFinished()
{
    qreal value = checkInput();
    emit sigValueChanged(value);
    return true;
}
