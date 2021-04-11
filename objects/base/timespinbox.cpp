#include "timespinbox.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "qguiapplication.h"
#include "qclipboard.h"

#include <QTime>

TimeSpinBox::TimeSpinBox(qint32 width)
{
    setObjectName("TimeSpinBox");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    m_Textbox = new oxygine::Box9Sprite();
    m_Textbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setResAnim(pAnim);
    m_Textfield = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_Textfield->setStyle(style);
    m_Textfield->setHtmlText("");


    oxygine::spClipRectActor pClipActor = new oxygine::ClipRectActor();

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

    m_pSpinBox = new oxygine::Box9Sprite();
    m_pSpinBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpinBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpinBox->setResAnim(pAnim);
    m_pSpinBox->setSize(width - m_Textbox->getWidth(), 40);
    m_pSpinBox->setX(m_Textbox->getWidth());

    m_pArrowDown = new oxygine::Button();
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
        m_spinDirection = -1 * m_SpinSpeed;
        qint32 value = getCurrentValue();
        value += m_spinDirection;
        setCurrentValue(value);
        toggle.start();
        emit sigValueChanged(getCurrentValue());
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_spinDirection = 0;
    });
    m_pSpinBox->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(9, m_pSpinBox->getHeight() - m_pArrowDown->getHeight() - 8);

    m_pArrowUp = new oxygine::Button();
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
        m_spinDirection = 1 * m_SpinSpeed;
        qint32 value = getCurrentValue();
        value += m_spinDirection;
        setCurrentValue(value);
        toggle.start();
        emit sigValueChanged(getCurrentValue());
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
    toggle.start();

    connect(pApp, &Mainapp::sigKeyDown, this, &TimeSpinBox::KeyInput, Qt::QueuedConnection);
    setCurrentValue(0);
}

void TimeSpinBox::focused()
{
    curmsgpos = m_Text.size();
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->show();
    }
}

void TimeSpinBox::focusedLost()
{
    qint32 value = checkInput();
    m_Textfield->setX(0);
    emit sigValueChanged(value);
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->hide();
    }
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
    if(m_focused)
    {
        // create output text
        QString drawText = m_Text;
        if (toggle.elapsed() < BLINKFREQG)
        {
            drawText.insert(curmsgpos,"|");
        }
        else
        {
            drawText.insert(curmsgpos," ");
        }
        if (toggle.elapsed() > BLINKFREQG * 2)
        {
            toggle.start();
        }
        m_Textfield->setHtmlText(drawText);

        if (m_Text.size() > 0)
        {
            // calc text field position based on curmsgpos
            qint32 xPos = 0;
            qint32 fontWidth = m_Textfield->getTextRect().getWidth() / m_Text.size();
            qint32 boxSize = (m_Textbox->getWidth() - 5 - fontWidth);
            xPos = -fontWidth * curmsgpos + boxSize / 2;
            if (xPos > 0)
            {
                xPos = 0;
            }
            else if ((m_Text.size() - curmsgpos + 1) * fontWidth < boxSize)
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
            if (toggle.elapsed() > BLINKFREQG)
            {
                qint32 value = getCurrentValue();
                value += m_spinDirection;
                setValue(value);
                toggle.start();
            }
            checkInput();
        }
        QString drawText = m_Text;
        m_Textfield->setHtmlText(drawText);
    }
    oxygine::Actor::update(us);
}

qint32 TimeSpinBox::getCurrentValue()
{
    qint32 value = 0;
    value = QTime::fromString(m_Text, "hh:mm:ss").msecsSinceStartOfDay();
    return value;
}

qint32 TimeSpinBox::checkInput()
{
    qint32 value = QTime::fromString(m_Text, "hh:mm:ss").msecsSinceStartOfDay();
    setValue(value);
    return value;
}

void TimeSpinBox::setValue(qint32 value)
{
    
    m_Text = QTime::fromMSecsSinceStartOfDay(value).toString("hh:mm:ss");
    m_Textfield->setHtmlText(m_Text);
    
}

qint32 TimeSpinBox::getSpinSpeed() const
{
    return m_SpinSpeed;
}

void TimeSpinBox::setSpinSpeed(qint32 SpinSpeed)
{
    m_SpinSpeed = SpinSpeed;
}

void TimeSpinBox::KeyInput(oxygine::KeyEvent event)
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
                    m_Text = m_Text.insert(curmsgpos, text);
                    curmsgpos = text.size();
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
                    curmsgpos = 0;
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
                    curmsgpos = 0;
                    break;
                }
                case Qt::Key_Left:
                {
                    curmsgpos--;
                    if(curmsgpos < 0)
                    {
                        curmsgpos = 0;
                    }
                    break;
                }
                case Qt::Key_Right:
                {
                    curmsgpos++;
                    if(curmsgpos > m_Text.size())
                    {
                        curmsgpos = m_Text.size();
                    }
                    break;
                }
                case Qt::Key_Enter:
                case Qt::Key_Return:
               {
                    looseFocusInternal();
                    qint32 value = checkInput();
                    emit sigValueChanged(value);
                    break;
                }
                case Qt::Key_Backspace:
                {
                    if(curmsgpos > 0){
                        m_Text.remove(curmsgpos - 1,1);
                        curmsgpos--;
                    }
                    break;
                }
                case Qt::Key_Delete:
                {
                    if (curmsgpos < m_Text.size())
                    {
                        m_Text.remove(curmsgpos, 1);
                    }
                    break;
                }
                case Qt::Key_End:
                {
                    curmsgpos = m_Text.size();
                    break;
                }
                default:
                {
                    // for the start we don't check for upper or lower key input
                    QString msg = event.getText();
                    m_Text.insert(curmsgpos, msg);
                    checkInput();
                    curmsgpos = m_Text.size();
                }
            }
        }
        
    }
}
