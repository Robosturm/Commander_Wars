#include "objects/base/spinbox.h"
#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "qguiapplication.h"
#include "qclipboard.h"

SpinBox::SpinBox(qint32 width, qint32 min, qint32 max, Mode mode)
    : m_MinValue(min),
      m_MaxValue(max),
      m_Mode(mode)
{
    setObjectName("SpinBox");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    m_Textbox = oxygine::spBox9Sprite::create();
    m_Textbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setResAnim(pAnim);
    m_Textfield = oxygine::spTextField::create();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_Textfield->setStyle(style);
    m_Textfield->setHtmlText("");


    oxygine::spClipRectActor pClipActor = oxygine::spClipRectActor::create();

    m_Textfield->attachTo(pClipActor);
    m_Textbox->addChild(pClipActor);
    m_Textbox->setSize(width - 35, 40);
    this->setSize(width, 40);
    m_Textfield->setWidth(m_Textbox->getWidth() - 20);
    m_Textfield->setHeight(m_Textbox->getHeight());
    pClipActor->setSize(m_Textfield->getSize());
    pClipActor->setX(10);
    pClipActor->setY(5);
    this->addChild(m_Textbox);

    m_pSpinBox = oxygine::spBox9Sprite::create();
    m_pSpinBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpinBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpinBox->setResAnim(pAnim);
    m_pSpinBox->setSize(width - m_Textbox->getWidth(), 40);
    m_pSpinBox->setX(m_Textbox->getWidth());

    m_pArrowDown = oxygine::spButton::create();
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowDown->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        if (m_spinDirection == 0)
        {
            m_spinDirection = -1 * m_SpinSpeed;
            qreal value = getCurrentValue();
            value += m_spinDirection;
            setCurrentValue(value);
            m_toggle.start();
            emit sigValueChanged(getCurrentValue());
        }
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
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
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        if (m_spinDirection == 0)
        {
            m_spinDirection = 1 * m_SpinSpeed;
            qreal value = getCurrentValue();
            value += m_spinDirection;
            setCurrentValue(value);
            m_toggle.start();
            emit sigValueChanged(getCurrentValue());
        }
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_spinDirection = 0;
    });
    m_pSpinBox->addChild(m_pArrowUp);
    m_pArrowUp->setPosition(9, 8);

    this->addChild(m_pSpinBox);


    this->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFocused();
    });
    m_toggle.start();

    connect(pApp, &Mainapp::sigKeyDown, this, &SpinBox::SpinBox::KeyInput, Qt::QueuedConnection);
}

void SpinBox::focused()
{
    m_curmsgpos = m_Text.size();
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->show();
    }
}

QString SpinBox::getUnit() const
{
    return m_unit;
}

void SpinBox::setUnit(const QString &unit)
{
    m_unit = unit;
}

void SpinBox::focusedLost()
{
    qreal value = checkInput();
    m_Textfield->setX(0);
    emit sigValueChanged(value);
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->hide();
    }
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
    if(m_focused)
    {
        // create output text
        QString drawText = m_Text;
        if (m_toggle.elapsed() < BLINKFREQG)
        {
            drawText.insert(m_curmsgpos,"|");
        }
        else
        {
            drawText.insert(m_curmsgpos," ");
        }
        if (m_toggle.elapsed() > BLINKFREQG * 2)
        {
            m_toggle.start();
        }
        m_Textfield->setHtmlText(drawText + m_unit);

        if (m_Text.size() > 0)
        {
            // calc text field position based on curmsgpos
            qint32 xPos = 0;
            qint32 fontWidth = m_Textfield->getTextRect().getWidth() / m_Text.size();
            qint32 boxSize = (m_Textbox->getWidth() - 5 - fontWidth);
            xPos = -fontWidth * m_curmsgpos + boxSize / 2;
            if (xPos > 0)
            {
                xPos = 0;
            }
            else if ((m_Text.size() - m_curmsgpos + 1) * fontWidth < boxSize)
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
            if (m_toggle.elapsed() > BLINKFREQG)
            {
                qreal value = getCurrentValue();
                value += m_spinDirection;
                setValue(value);
                m_toggle.start();
            }
            checkInput();
        }
        QString drawText = m_Text;
        m_Textfield->setHtmlText(drawText + m_unit);
    }
    oxygine::Actor::update(us);
}

qreal SpinBox::getCurrentValue()
{
    qreal value = 0;
    if (m_Text == "∞")
    {
        value = m_InfinityValue;
    }
    else
    {
        bool ok = false;
        value = m_Text.toDouble(&ok);
        if (!ok)
        {
            value = m_InfinityValue;
        }
    }
    return value;
}

qreal SpinBox::checkInput()
{
    Console::print("SpinBox::checkInput", Console::eDEBUG);
    bool ok = false;
    qreal value = m_Text.toDouble(&ok);
    if (m_Text == "∞")
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
    
    if (value == m_InfinityValue)
    {
        m_Text = "∞";
    }
    else
    {
        switch (m_Mode)
        {
            case Mode::Int:
            {
                m_Text = QString::number(static_cast<qint32>(value));
                break;
            }
            case Mode::Float:
            case Mode::Real:
            {
                m_Text = QString::number(value, 'f', 2);
                break;
            }
        }
    }
    m_Textfield->setHtmlText(m_Text + m_unit);
    
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

void SpinBox::KeyInput(oxygine::KeyEvent event)
{
    // for debugging
    Qt::Key cur = event.getKey();
    if (m_focused)
    {
        restartTooltiptimer();
        if ((event.getModifiers() & Qt::KeyboardModifier::ControlModifier) > 0)
        {
            switch(cur)
            {
                case Qt::Key_V:
                {
                    QString text = QGuiApplication::clipboard()->text();
                    m_Text = m_Text.insert(m_curmsgpos, text);
                    m_curmsgpos = text.size();
                    break;
                }
                case Qt::Key_C:
                {
                    QGuiApplication::clipboard()->setText(m_Text);
                    break;
                }
                case Qt::Key_X:
                {
                    QGuiApplication::clipboard()->setText(m_Text);
                    m_Text = "";
                    m_curmsgpos = 0;
                    break;
                }
                default:
                {
                    // nothing
                    break;
                }
            }
        }
        else
        {
            //Handle Key Input for the console
            switch(cur)
            {
                case Qt::Key_Home:
                {
                    m_curmsgpos = 0;
                    break;
                }
                case Qt::Key_Left:
                {
                    m_curmsgpos--;
                    if(m_curmsgpos < 0)
                    {
                        m_curmsgpos = 0;
                    }
                    break;
                }
                case Qt::Key_Right:
                {
                    m_curmsgpos++;
                    if(m_curmsgpos > m_Text.size())
                    {
                        m_curmsgpos = m_Text.size();
                    }
                    break;
                }
                case Qt::Key_Enter:
                case Qt::Key_Return:
                {
                    looseFocusInternal();
                    qreal value = checkInput();
                    emit sigValueChanged(value);
                    break;
                }
                case Qt::Key_Backspace:
                {
                    if(m_curmsgpos > 0){
                        m_Text.remove(m_curmsgpos - 1,1);
                        m_curmsgpos--;
                    }
                    break;
                }
                case Qt::Key_Delete:
                {
                    if (m_curmsgpos < m_Text.size())
                    {
                        m_Text.remove(m_curmsgpos, 1);
                    }
                    break;
                }
                case Qt::Key_End:
                {
                    m_curmsgpos = m_Text.size();
                    break;
                }
                default:
                {
                    // for the start we don't check for upper or lower key input
                    QString msg = event.getText();
                    m_Text.insert(m_curmsgpos, msg);
                    bool ok = false;
                    msg.toFloat(&ok);
                    if (!ok)
                    {
                        checkInput();
                    }
                    m_curmsgpos = m_Text.size();
                }
            }
        }
        
    }
}
