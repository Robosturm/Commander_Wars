#include "spinbox.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

SpinBox::SpinBox(qint32 width, qint32 min, qint32 max, Mode mode)
    : m_MinValue(min),
      m_MaxValue(max),
      m_Mode(mode)
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
    m_Textfield->setText("");


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

    oxygine::spBox9Sprite spinBox = new oxygine::Box9Sprite();
    spinBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    spinBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    spinBox->setResAnim(pAnim);
    spinBox->setSize(width - m_Textbox->getWidth(), 40);
    spinBox->setX(m_Textbox->getWidth());

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
        float value = getCurrentValue();
        value += m_spinDirection;
        setCurrentValue(value);
        checkInput();
        toggle.start();
        emit sigValueChanged(getCurrentValue());
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_spinDirection = 0;
    });
    spinBox->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(9, spinBox->getHeight() - m_pArrowDown->getHeight() - 8);

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
        float value = getCurrentValue();
        value += m_spinDirection;
        setCurrentValue(value);
        checkInput();
        toggle.start();
        emit sigValueChanged(getCurrentValue());
    });
    m_pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_spinDirection = 0;
    });
    spinBox->addChild(m_pArrowUp);
    m_pArrowUp->setPosition(9, 8);

    this->addChild(spinBox);


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
            float value = checkInput();
            emit sigValueChanged(value);
        }
        else
        {
            m_focused = false;
        }

    });
    toggle.start();

    connect(pApp, &Mainapp::sigKeyDown, this, &SpinBox::SpinBox::KeyInput, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigText, this, &SpinBox::TextInput, Qt::QueuedConnection);
}


void SpinBox::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_pArrowUp->setEnabled(value);
    m_Textbox->setEnabled(value);
    m_focused = false;
}

void SpinBox::setCurrentValue(float value)
{
    m_Text = QString::number(static_cast<double>(value));
    checkInput();
}

void SpinBox::update(const oxygine::UpdateState& us)
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
        m_Textfield->setText(drawText.toStdString().c_str());

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
        if (m_spinDirection != 0.0f)
        {
            if (toggle.elapsed() > BLINKFREQG)
            {
                float value = getCurrentValue();
                value += m_spinDirection;
                setValue(value);
                toggle.start();
            }
            checkInput();
        }
    }
    oxygine::Actor::update(us);
}

float SpinBox::getCurrentValue()
{
    float value = 0;
    if (m_Text == "∞")
    {
        value = m_InfinityValue;
    }
    else
    {
        bool ok = false;
        value = m_Text.toFloat(&ok);
        if (!ok)
        {
            value = m_InfinityValue;
        }
    }
    return value;
}

float SpinBox::checkInput()
{

    bool ok = false;
    float value = m_Text.toFloat(&ok);
    if (m_Text == "∞")
    {
        value = m_InfinityValue;
        ok = true;
    }
    if (!m_focused)
    {
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
    }
    return value;
}

void SpinBox::setValue(float value)
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
            {
                m_Text = QString::number(static_cast<double>(value));
                break;
            }
        }
    }
    m_Textfield->setText(m_Text.toStdString().c_str());
}

float SpinBox::getInfinityValue() const
{
    return m_InfinityValue;
}

void SpinBox::setInfinityValue(float InfinityValue)
{
    m_InfinityValue = InfinityValue;
}

float SpinBox::getSpinSpeed() const
{
    return m_SpinSpeed;
}

void SpinBox::setSpinSpeed(float SpinSpeed)
{
    m_SpinSpeed = SpinSpeed;
}

void SpinBox::TextInput(SDL_Event event)
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

void SpinBox::KeyInput(SDL_Event event)
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
                float value = checkInput();
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
