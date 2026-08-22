#ifdef GRAPHICSUPPORT
#include <QApplication>
#endif

#include "objects/base/selectkey.h"
#include "objects/base/label.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"

SelectKey::SelectKey(Qt::Key code)
{
#ifdef GRAPHICSUPPORT
    setObjectName("SelectKey");
#endif
    Interpreter::setCppOwnerShip(this);
    setSize(180, 40);
    m_Button = ObjectManager::createButton("", getWidth());
    oxygine::Actor* pActor = m_Button.get();
    m_Button->addEventListener(oxygine::TouchEvent::CLICK, [this, pActor](oxygine::Event * )->void
    {
        spLabel pText = oxygine::safeSpCast<Label>(pActor->getFirstChild());
        pText->setHtmlText(tr("Press Key"));
        pText->setX(pActor->getScaledWidth() / 2 - pText->getTextRect().width() / 2);
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
    connect(Mainapp::getInstance(), &Mainapp::sigKeyDown, this, &SelectKey::keyInput, Qt::QueuedConnection);
}

void SelectKey::focusedLost()
{
    setKeycode(m_currentCode);
#ifdef GRAPHICSUPPORT
    auto virtualKeyboard = QApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->hide();
    }
#endif
}

void SelectKey::focused()
{
#ifdef GRAPHICSUPPORT
    auto virtualKeyboard = QApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->show();
    }
#endif
}

void SelectKey::keyInput(oxygine::KeyEvent event)
{
    if (m_active && !event.getContinousPress())
    {
        restartTooltiptimer();
        Qt::Key cur = event.getKey();
        setKeycode(cur);
    }
}

void SelectKey::setKeycode(Qt::Key code)
{
    Mainapp::getInstance()->pauseRendering();
    QString codeText = GlobalUtils::getKeycodeText(code);
    if (codeText != tr("Unknown"))
    {
        m_currentCode = code;
        spLabel pText = oxygine::safeSpCast<Label>(m_Button->getFirstChild());
        pText->setHtmlText(codeText);
        pText->setX(m_Button->getScaledWidth() / 2 - pText->getTextRect().width() / 2);
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
    Mainapp::getInstance()->continueRendering();
}
