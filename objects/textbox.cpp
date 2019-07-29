#include "textbox.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

Textbox::Textbox(qint32 width)
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
    m_Textbox->setSize(width, 40);
    this->setSize(width, 40);
    m_Textfield->setWidth(m_Textbox->getWidth() - 20);
    m_Textfield->setHeight(m_Textbox->getHeight());
    pClipActor->setSize(m_Textfield->getSize());
    pClipActor->setX(10);



    this->addChild(m_Textbox);
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
            emit sigTextChanged(m_Text);
        }
        else
        {
            m_focused = false;
        }

    });
    toggle.start();

    Mainapp* pMainapp = Mainapp::getInstance();
    connect(pMainapp, &Mainapp::sigKeyDown, this, &Textbox::KeyInput, Qt::QueuedConnection);
    connect(pMainapp, &Mainapp::sigText, this, &Textbox::TextInput, Qt::QueuedConnection);
}

void Textbox::setCurrentText(QString text)
{
    m_Text = text;
}

void Textbox::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    if(m_focused)
    {
        // create output text
        QString drawText = m_Text;
        if (toggle.elapsed() < BLINKFREQG)
        {
            drawText.insert(curmsgpos,"|");
        }else{
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
        m_Textfield->setHtmlText(m_Text.toStdString().c_str());
    }
    oxygine::Actor::update(us);
}

void Textbox::TextInput(SDL_Event event)
{
    if (m_focused)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        // for the start we don't check for upper or lower key input
        QString msg = QString(event.text.text);
        m_Text.insert(curmsgpos, msg);
        curmsgpos += msg.size();
        pApp->continueThread();
    }
}

void Textbox::KeyInput(SDL_Event event)
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
                emit sigTextChanged(m_Text);
                emit sigEnterPressed(m_Text);
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
