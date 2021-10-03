#include "objects/base/focusableobject.h"
#include "coreengine/console.h"
#include "coreengine/mainapp.h"

#include <QClipboard>
#include <QGuiApplication>

FocusableObject* FocusableObject::m_focusedObject = nullptr;
bool FocusableObject::m_registeredAtStage = false;

FocusableObject::FocusableObject()
    : QObject()
{
    setObjectName("FocusableObject");
    connect(this, &FocusableObject::sigFocused, this, &FocusableObject::focusedInternal);
    connect(this, &FocusableObject::sigFocusedLost, this, &FocusableObject::focusedLost);
    connect(Mainapp::getInstance(), &Mainapp::sigFocusedObjectEvent, this, &FocusableObject::doHandleEvent, Qt::QueuedConnection);
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
    bool handled = false;
    if (FocusableObject::getFocusedObject() != nullptr)
    {
        switch (event->type())
        {
            case QEvent::InputMethodQuery:
            case QEvent::InputMethod:
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
            {
                std::shared_ptr<QEvent> ev(event->clone());
                emit Mainapp::getInstance()->sigFocusedObjectEvent(ev);
                handled = true;
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
        auto virtualKeyboard = QGuiApplication::inputMethod();
        if (virtualKeyboard != nullptr)
        {
            virtualKeyboard->hide();
        }
    }
}

void FocusableObject::looseFocusInternal()
{
    if (m_focusedObject != nullptr)
    {
        CONSOLE_PRINT("Loosing focus forced by object", Console::eDEBUG);
        m_focusedObject->m_focused = false;
        m_focusedObject = nullptr;
        auto virtualKeyboard = QGuiApplication::inputMethod();
        if (virtualKeyboard != nullptr)
        {
            virtualKeyboard->hide();
        }
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
