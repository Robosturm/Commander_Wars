#include <QClipboard>
#ifdef GRAPHICSUPPORT
#include <QApplication>
#endif

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/base/focusableobject.h"

#include "coreengine/console.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

FocusableObject* FocusableObject::m_focusedObject = nullptr;
bool FocusableObject::m_registeredAtStage = false;

FocusableObject::FocusableObject()
{
#ifdef GRAPHICSUPPORT
    setObjectName("FocusableObject");
#endif
    Interpreter::setCppOwnerShip(this);

    connect(this, &FocusableObject::sigFocused, this, &FocusableObject::focusedInternal);
    connect(this, &FocusableObject::sigFocusedLost, this, &FocusableObject::focusedLost);
    connect(this, &FocusableObject::sigLooseFocusInternal, this, &FocusableObject::looseFocusInternal);
    if (!m_registeredAtStage)
    {
        m_registeredAtStage = true;
        oxygine::Stage::getStage()->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event* event)
        {
            if (m_focusedObject != nullptr &&
                !m_focusedObject->isDescendant(oxygine::safeSpCast<Actor>(event->target)))
            {
                looseFocus();
            }
        });
    }
}

bool FocusableObject::handleEvent(QEvent *event)
{
    auto* pObj = FocusableObject::getFocusedObject();
    bool handled = false;
    if (pObj != nullptr)
    {
        switch (event->type())
        {
            case QEvent::InputMethodQuery:
            case QEvent::InputMethod:
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
            case QEvent::Shortcut:
            case QEvent::ShortcutOverride:
            {
                handled = pObj->doHandleEvent(event);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return handled;
}

void FocusableObject::handleInputMethodQuery(Qt::InputMethodQuery query, QVariant arg)
{
    if (FocusableObject::getFocusedObject() != nullptr)
    {
        FocusableObject::getFocusedObject()->inputMethodQuery(query, arg);
    }
}


FocusableObject::~FocusableObject()
{
    if (m_focusedObject == this)
    {
        m_focusedObject = nullptr;
    }
}

void FocusableObject::looseFocus()
{
    if (m_focusedObject != nullptr)
    {
        CONSOLE_PRINT("Focus lost for object", Console::eDEBUG);
        m_focusedObject->m_focused = false;
        emit m_focusedObject->sigFocusedLost();
        m_focusedObject = nullptr;
#ifdef GRAPHICSUPPORT
        auto virtualKeyboard = QApplication::inputMethod();
        if (virtualKeyboard != nullptr)
        {
            virtualKeyboard->hide();
        }
#endif
    }
}

void FocusableObject::looseFocusInternal()
{
    if (m_focusedObject != nullptr)
    {
        CONSOLE_PRINT("Loosing focus forced by object", Console::eDEBUG);
        m_focusedObject->m_focused = false;
        m_focusedObject = nullptr;
#ifdef GRAPHICSUPPORT
        auto virtualKeyboard = QApplication::inputMethod();
        if (virtualKeyboard != nullptr)
        {
            virtualKeyboard->hide();
        }
#endif
    }
}

void FocusableObject::focusedInternal()
{
    if (m_focusedObject != this && !m_subComponent)
    {
        CONSOLE_PRINT("Focused object changed", Console::eDEBUG);
        looseFocus();
        m_focusedObject = this;
        m_focusedObject->m_focused = true;
        m_focusedObject->focused();
    }
}

bool FocusableObject::getSubComponent() const
{
    return m_subComponent;
}

void FocusableObject::setSubComponent(bool subComponent)
{
    m_subComponent = subComponent;
}

bool FocusableObject::getFocused() const
{
    return m_focused;
}
