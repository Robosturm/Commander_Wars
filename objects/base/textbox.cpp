#include "textbox.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "qguiapplication.h"
#include "qclipboard.h"

Textbox::Textbox(qint32 width, qint32 heigth)
{
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
    style.multiline = (heigth > 0);
    m_Textfield->setStyle(style);
    m_Textfield->setHtmlText("");


    oxygine::spClipRectActor pClipActor = new oxygine::ClipRectActor();

    if (heigth < 0)
    {
        heigth = 40;
    }
    m_Textfield->attachTo(pClipActor);
    m_Textbox->addChild(pClipActor);
    m_Textbox->setSize(width, heigth);
    this->setSize(width, heigth);
    m_Textfield->setWidth(m_Textbox->getWidth() - 20);
    m_Textfield->setHeight(m_Textbox->getHeight());
    pClipActor->setSize(m_Textfield->getSize());
    pClipActor->setX(10);
    pClipActor->setY(5);

    this->addChild(m_Textbox);
    this->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event* event)
    {
        event->stopPropagation();
        emit sigFocused();
    });
    toggle.start();

    Mainapp* pMainapp = Mainapp::getInstance();
    connect(pMainapp, &Mainapp::sigKeyDown, this, &Textbox::KeyInput, Qt::QueuedConnection);
}

void Textbox::focusedLost()
{
    emit sigTextChanged(m_Text);
    Tooltip::enableTooltip();
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->hide();
    }
}

void Textbox::focused()
{
    Tooltip::disableTooltip();
    curmsgpos = m_Text.size();
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->show();
    }
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
        m_Textfield->setHtmlText(m_Text);
    }
    m_Textfield->setWidth( m_Textfield->getTextRect().getWidth());
    oxygine::Actor::update(us);
}

void Textbox::KeyInput(oxygine::KeyEvent event)
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
                    curmsgpos += text.size();
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
                case Qt::Key_At:
                {
                    QString msg = "@";
                    m_Text.insert(curmsgpos, msg);
                    curmsgpos += msg.size();
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
                    Tooltip::setEnabled(true);
                    emit sigTextChanged(m_Text);
                    emit sigEnterPressed(m_Text);
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
                    curmsgpos += msg.size();
                }
            }
        }
        
    }
}

void Textbox::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_Textbox->setEnabled(value);
    m_Textfield->setEnabled(value);
}
