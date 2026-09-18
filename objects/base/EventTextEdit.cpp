#include "objects/base/EventTextEdit.h"
#include "coreengine/interpreter.h"

#include <QGuiApplication>
#include <QInputMethod>

EventTextEdit::EventTextEdit(QWidget* parent)
 : QTextEdit(parent)
{
#ifdef GRAPHICSUPPORT
    setObjectName("EventTextEdit");
#endif
    setAttribute(Qt::WA_InputMethodEnabled, true);
    setFocusPolicy(Qt::StrongFocus);
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

bool EventTextEdit::handleTextInputEvent(QEvent *event)
{
    const QString currentText = toPlainText();
    bool handled = EventTextEdit::event(event);
    if (m_editableKeys &&
        currentText == toPlainText() &&
        event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        handled = handleForwardedEditingKey(keyEvent) || handled;
    }
    return handled;
}

void EventTextEdit::inputMethodEvent(QInputMethodEvent *event)
{
    auto currentText = toPlainText();
    QTextEdit::inputMethodEvent(event);
    if (currentText != toPlainText() && !m_editableKeys)
    {
        setPlainText(currentText);
    }
}

QVariant EventTextEdit::inputMethodQuery(Qt::InputMethodQuery query) const
{
    if (query == Qt::ImEnabled)
    {
        return m_editableKeys;
    }
    return QTextEdit::inputMethodQuery(query);
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

bool EventTextEdit::insertForwardedKeyText(QKeyEvent *keyEvent)
{
    if (keyEvent->matches(QKeySequence::Copy) ||
        keyEvent->matches(QKeySequence::Paste) ||
        keyEvent->matches(QKeySequence::Cut) ||
        keyEvent->matches(QKeySequence::Undo) ||
        keyEvent->matches(QKeySequence::Redo) ||
        keyEvent->modifiers().testFlag(Qt::ControlModifier) ||
        keyEvent->modifiers().testFlag(Qt::AltModifier) ||
        keyEvent->modifiers().testFlag(Qt::MetaModifier))
    {
        return false;
    }

    const QString text = keyEvent->text();
    if (text.isEmpty())
    {
        return false;
    }
    if (m_singleLine &&
        (text.contains(QLatin1Char('\r')) ||
         text.contains(QLatin1Char('\n'))))
    {
        emit returnPressed();
        return true;
    }
    insertPlainText(text);
    keyEvent->accept();
    return true;
}

bool EventTextEdit::handleForwardedEditingKey(QKeyEvent *keyEvent)
{
    if (keyEvent->matches(QKeySequence::Paste))
    {
        paste();
        keyEvent->accept();
        return true;
    }
    if (keyEvent->matches(QKeySequence::Cut))
    {
        cut();
        keyEvent->accept();
        return true;
    }
    if (keyEvent->matches(QKeySequence::Undo))
    {
        undo();
        keyEvent->accept();
        return true;
    }
    if (keyEvent->matches(QKeySequence::Redo))
    {
        redo();
        keyEvent->accept();
        return true;
    }
    if (keyEvent->key() == Qt::Key_Backspace)
    {
        textCursor().deletePreviousChar();
        keyEvent->accept();
        return true;
    }
    if (keyEvent->key() == Qt::Key_Delete)
    {
        textCursor().deleteChar();
        keyEvent->accept();
        return true;
    }
    return insertForwardedKeyText(keyEvent);
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
