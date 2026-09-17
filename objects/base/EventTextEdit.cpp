#include "objects/base/EventTextEdit.h"
#include "coreengine/interpreter.h"

#include <QGuiApplication>
#include <QInputMethod>

EventTextEdit::EventTextEdit()
{
#ifdef GRAPHICSUPPORT
    setObjectName("EventTextEdit");
#endif
    Interpreter::setCppOwnerShip(this);
}

bool EventTextEdit::event(QEvent *event)
{
    QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
    if (keyEvent != nullptr &&
        keyEvent->key() == Qt::Key_Return &&
        (keyEvent->modifiers() != Qt::ShiftModifier ||
         m_singleLine))
    {
        emit returnPressed();
        return true;
    }
    else if (!m_editableKeys)
    {
        if (keyEvent != nullptr)
        {
            // swallow key presses that would modify the text,
            // keep forwarding cursor movement, selection and copy keys
            if (isEditingKeyPress(keyEvent))
            {
                return true;
            }
            return QTextEdit::event(event);
        }
    }
    return QTextEdit::event(event);
}

void EventTextEdit::inputMethodEvent(QInputMethodEvent *event)
{
    auto currentText = toPlainText();
    QTextEdit::inputMethodEvent(event);
    if (currentText != toPlainText() && m_editableKeys)
    {
        setPlainText(currentText);
    }
}

bool EventTextEdit::isEditingKeyPress(QKeyEvent *keyEvent) const
{
    bool editing = false;
    if (keyEvent->type() == QEvent::KeyPress)
    {
        const QString text = keyEvent->text();
        if (keyEvent->matches(QKeySequence::Copy))
        {
            editing = false;
        }
        else if (keyEvent->matches(QKeySequence::Paste) ||
                 keyEvent->matches(QKeySequence::Cut) ||
                 keyEvent->matches(QKeySequence::Undo) ||
                 keyEvent->matches(QKeySequence::Redo))
        {
            editing = true;
        }
        else if (keyEvent->key() == Qt::Key_Backspace ||
                 keyEvent->key() == Qt::Key_Delete)
        {
            editing = true;
        }
        else if (!text.isEmpty() &&
                 (text[0].isPrint() ||
                  text[0] == QLatin1Char('\r') ||
                  text[0] == QLatin1Char('\n') ||
                  text[0] == QLatin1Char('\t')))
        {
            editing = true;
        }
    }
    return editing;
}

bool EventTextEdit::getSingleLine() const
{
    return m_singleLine;
}
void EventTextEdit::setSingleLine(bool newSingleLine)
{
    m_singleLine = newSingleLine;
}

void EventTextEdit::setEditableKeys(bool editableKeys)
{
    m_editableKeys = editableKeys;
}

bool EventTextEdit::getEditableKeys() const
{
    return m_editableKeys;
}
