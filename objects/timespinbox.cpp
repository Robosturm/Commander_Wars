#include "timespinbox.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

TimeSpinBox::TimeSpinBox(qint32 width)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("textbox");
    m_Textbox = new oxygine::Box9Sprite();
    m_Textbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_Textbox->setResAnim(pAnim);
    m_Textfield = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
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
    this->addChild(m_Textbox);

    m_pSpinBox = new oxygine::Box9Sprite();
    m_pSpinBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpinBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pSpinBox->setResAnim(pAnim);
    m_pSpinBox->setSize(width - m_Textbox->getWidth(), 40);
    m_pSpinBox->setX(m_Textbox->getWidth());

    m_pArrowDown = new oxygine::Button();
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowDown->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
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
    m_pArrowUp->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_pArrowUp->setFlippedY(true);
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_pArrowUp->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(16, 16, 16, 0)), 300);
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowUp->addTween(oxygine::Sprite::TweenAddColor(oxygine::Color(0, 0, 0, 0)), 300);
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
        m_focused = !m_focused;
        if (m_focused)
        {
            curmsgpos = m_Text.size();
        }
    });
    this->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        // not the best solution
        // but for the start the easiest one :)
        if (m_focused)
        {
            m_focused = false;
            qint32 value = checkInput();
            emit sigValueChanged(value);
        }
        else
        {
            m_focused = false;
        }

    });
    toggle.start();

    connect(pApp, &Mainapp::sigKeyDown, this, &TimeSpinBox::KeyInput, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigText, this, &TimeSpinBox::TextInput, Qt::QueuedConnection);

    setCurrentValue(0);
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
        m_Textfield->setHtmlText(drawText.toStdString().c_str());

        if (m_Text.size() > 0)
        {
            // calc text field position based on curmsgpos
            qint32 xPos = 0;
            qint32 fontWidth = m_Textfield->getTextRect().getWidth() / m_Text.size();
            qint32 boxSize = (m_Textbox->getWidth() - 40 - fontWidth);
            xPos = -fontWidth * curmsgpos + boxSize / 2;
            if (xPos > 0)
            {
                xPos = 0;
            }
            else if ((m_Text.size() - curmsgpos + 3) * fontWidth < boxSize)
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
        m_Textfield->setHtmlText(drawText.toStdString().c_str());
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
    m_Textfield->setHtmlText(m_Text.toStdString().c_str());
}

qint32 TimeSpinBox::getSpinSpeed() const
{
    return m_SpinSpeed;
}

void TimeSpinBox::setSpinSpeed(qint32 SpinSpeed)
{
    m_SpinSpeed = SpinSpeed;
}

void TimeSpinBox::TextInput(SDL_Event event)
{
    if (m_focused)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        // for the start we don't check for upper or lower key input
        QString msg = QString(event.text.text);
        m_Text.insert(curmsgpos, msg);
        checkInput();
        curmsgpos = m_Text.size();
        pApp->continueThread();
    }
}

void TimeSpinBox::KeyInput(SDL_Event event)
{
    // for debugging
    SDL_Keycode cur = event.key.keysym.sym;
    if (m_focused)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        if ((event.key.keysym.mod & KMOD_CTRL) > 0)
        {
            switch(cur)
            {
            case SDLK_v:
            {
                QString text = SDL_GetClipboardText();
                m_Text = m_Text.insert(curmsgpos, text);
                checkInput();
                curmsgpos = text.size();
                break;
            }
            case SDLK_c:
            {
                SDL_SetClipboardText(m_Text.toStdString().c_str());
                break;
            }
            case SDLK_x:
            {
                SDL_SetClipboardText(m_Text.toStdString().c_str());
                m_Text = "0";
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
            case SDLK_HOME:
            {
                curmsgpos = 0;
                break;
            }
            case SDLK_LEFT:
            {
                curmsgpos--;
                if(curmsgpos < 0)
                {
                    curmsgpos = 0;
                }
                break;
            }
            case SDLK_RIGHT:
            {
                curmsgpos++;
                if(curmsgpos > m_Text.size())
                {
                    curmsgpos = m_Text.size();
                }
                break;
            }
            case SDLK_KP_ENTER:
            case SDLK_RETURN:
            {
                m_focused = false;
                qint32 value = checkInput();
                emit sigValueChanged(value);
                break;
            }
            case SDLK_BACKSPACE:
            {
                if(curmsgpos > 0){
                    m_Text.remove(curmsgpos - 1,1);
                    curmsgpos--;
                }
                break;
            }
            case SDLK_DELETE:
            {
                if (curmsgpos < m_Text.size())
                {
                    m_Text.remove(curmsgpos, 1);
                }
                break;
            }
            case SDLK_END:
            {
                curmsgpos = m_Text.size();
                break;
            }
            }
        }
        pApp->continueThread();
    }
}
