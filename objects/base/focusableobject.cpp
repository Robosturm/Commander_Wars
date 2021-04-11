#include "objects/base/focusableobject.h"
#include "coreengine/console.h"

FocusableObject* FocusableObject::m_focusedObject = nullptr;
bool FocusableObject::m_registeredAtStage = false;

FocusableObject::FocusableObject()
    : QObject()
{
    setObjectName("FocusableObject");
    connect(this, &FocusableObject::sigFocused, this, &FocusableObject::focusedInternal);
    connect(this, &FocusableObject::sigFocusedLost, this, &FocusableObject::focusedLost);
    if (!m_registeredAtStage)
    {
        m_registeredAtStage = true;
        oxygine::getStage()->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event* event)
        {
            if (m_focusedObject != nullptr &&
                !m_focusedObject->isDescendant(static_cast<Actor*>(event->target.get())))
            {
                looseFocus();
            }
        });
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
        Console::print("Focus lost for object", Console::eDEBUG);
        m_focusedObject->m_focused = false;
        emit m_focusedObject->sigFocusedLost();
        m_focusedObject = nullptr;
    }
}

void FocusableObject::looseFocusInternal()
{
    if (m_focusedObject != nullptr)
    {
        Console::print("Loosing focus forced by object", Console::eDEBUG);
        m_focusedObject->m_focused = false;
        m_focusedObject = nullptr;
    }
}

void FocusableObject::focusedInternal()
{
    if (m_focusedObject != this && !m_subComponent)
    {
        Console::print("Focused object changed", Console::eDEBUG);
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
