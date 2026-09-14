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
        else if (event->type() == QEvent::InputMethod)
        {
            QInputMethodEvent* inputEvent = static_cast<QInputMethodEvent*>(event);
            inputMethodEvent(inputEvent);
            return true;
        }
    }
    return QTextEdit::event(event);
}

void EventTextEdit::inputMethodEvent(QInputMethodEvent *event)
{
    const QString commitString = event->commitString();
    const qint32 replacementLength = event->replacementLength();
    CONSOLE_PRINT("Handling input method event: commitString='" + commitString + "' replacementLength=" + QString::number(replacementLength), GameConsole::eDEBUG);
    if (commitString.isEmpty() && replacementLength == 0)
    {
        QTextEdit::inputMethodEvent(event);
        return;
    }

    auto cursor = textCursor();
    const qint32 documentLength = document()->characterCount() - 1;
    const qint32 replacementStart = qBound(0, cursor.position() + event->replacementStart(), documentLength);
    const qint32 replacementEnd = qBound(replacementStart, replacementStart + replacementLength, documentLength);
    cursor.setPosition(replacementStart);
    cursor.setPosition(replacementEnd, QTextCursor::KeepAnchor);
    cursor.insertText(commitString);
    setTextCursor(cursor);
    QGuiApplication::inputMethod()->update(Qt::ImQueryAll);
    event->accept();
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
