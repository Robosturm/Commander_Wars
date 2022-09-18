#include "objects/base/EventTextEdit.h"

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
    else
    {
        return QTextEdit::event(event);
    }
}

bool EventTextEdit::getSingleLine() const
{
    return m_singleLine;
}
void EventTextEdit::setSingleLine(bool newSingleLine)
{
    m_singleLine = newSingleLine;
}
