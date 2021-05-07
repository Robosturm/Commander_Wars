#include "selectkey.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/label.h"

#include <qguiapplication.h>

SelectKey::SelectKey(Qt::Key code)
{
    setObjectName("SelectKey");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    m_Button = ObjectManager::createButton("");
    m_Button->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        Label* pText = dynamic_cast<Label*>(m_Button->getFirstChild()->getFirstChild().get());
        pText->setHtmlText(tr("Press Key"));
        pText->setX(m_Button->getWidth() / 2 - pText->getTextRect().getWidth() / 2);
        if (pText->getX() < 5)
        {
            pText->setX(5);
        }
        pText->setTooltipText("");
        m_active = true;
        emit sigFocused();
    });
    addChild(m_Button);
    setKeycode(code);
    connect(pApp, &Mainapp::sigKeyDown, this, &SelectKey::keyInput, Qt::QueuedConnection);
}

void SelectKey::focusedLost()
{
    setKeycode(m_currentCode);
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->hide();
    }
}

void SelectKey::focused()
{
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->show();
    }
}

void SelectKey::keyInput(oxygine::KeyEvent event)
{
    if (m_active)
    {
        restartTooltiptimer();
        Qt::Key cur = event.getKey();
        setKeycode(cur);
    }
}

QString SelectKey::getKeycodeText(Qt::Key code)
{
    QString codeText = tr("Unknown");
    const oxygine::Font* pFont = FontManager::getMainFont24()->getFont();
    if (code == Qt::Key_Space)
    {
        codeText = tr("Space");
    }
    else if (code == Qt::Key_F2)
    {
        codeText = tr("F2");
    }
    else if (code == Qt::Key_F3)
    {
        codeText = tr("F3");
    }
    else if (code == Qt::Key_F4)
    {
        codeText = tr("F4");
    }
    else if (code == Qt::Key_F5)
    {
        codeText = tr("F5");
    }
    else if (code == Qt::Key_F6)
    {
        codeText = tr("F6");
    }
    else if (code == Qt::Key_F7)
    {
        codeText = tr("F7");
    }
    else if (code == Qt::Key_F8)
    {
        codeText = tr("F8");
    }
    else if (code == Qt::Key_F9)
    {
        codeText = tr("F9");
    }
    else if (code == Qt::Key_F10)
    {
        codeText = tr("F10");
    }
    else if (code == Qt::Key_F11)
    {
        codeText = tr("F11");
    }
    else if (code == Qt::Key_F12)
    {
        codeText = tr("F12");
    }
    else if (code == Qt::Key_Up)
    {
        codeText = tr("Up");
    }
    else if (code == Qt::Key_Down)
    {
        codeText = tr("Down");
    }
    else if (code == Qt::Key_Left)
    {
        codeText = tr("Left");
    }
    else if (code == Qt::Key_Right)
    {
        codeText = tr("Right");
    }
    else if (code == Qt::Key_Return)
    {
        codeText = tr("Return");
    }
    else if (code == Qt::Key_Enter)
    {
        codeText = tr("Enter");
    }
    else if (code == Qt::Key_Backspace)
    {
        codeText = tr("Backspace");
    }
    else if (code == Qt::Key_Tab)
    {
        codeText = tr("Tab");
    }
    else if (code == Qt::Key_Control)
    {
        codeText = tr("Ctrl");
    }
    else if (code == Qt::Key_Alt)
    {
        codeText = tr("Alt");
    }
    else if (code == Qt::Key_AltGr)
    {
        codeText = tr("AltGr");
    }
    else if (code == Qt::Key_Plus)
    {
        codeText = tr("+");
    }
    else if (code == Qt::Key_Minus)
    {
        codeText = tr("-");
    }
    else if (code == Qt::Key_Asterisk)
    {
        codeText = tr("*");
    }
    else if (code == Qt::Key_Slash)
    {
        codeText = tr("/");
    }
    else if (code == 0)
    {
        codeText = tr("None");
    }
    else if (code <= 255)
    {
        const oxygine::glyph* pGlyph = pFont->getGlyph(code, oxygine::glyphOptions());
        if (pGlyph != nullptr)
        {
            codeText = static_cast<char>(code);
        }
    }
    return codeText;
}

void SelectKey::setKeycode(Qt::Key code)
{
    
    QString codeText = getKeycodeText(code);
    if (codeText != tr("Unknown"))
    {
        m_currentCode = code;
        Label* pText = dynamic_cast<Label*>(m_Button->getFirstChild()->getFirstChild().get());
        pText->setHtmlText((tr("Key ") + codeText));
        pText->setX(m_Button->getWidth() / 2 - pText->getTextRect().getWidth() / 2);
        if (pText->getX() < 5)
        {
            pText->setX(5);
        }
        pText->setTooltipText("");
        m_active = false;
        emit sigKeyChanged(m_currentCode);
    }
    else
    {
        setKeycode(m_currentCode);
    }
    looseFocusInternal();
}
