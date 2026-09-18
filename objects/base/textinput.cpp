#include "objects/base/textinput.h"

#include "coreengine/gameconsole.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

#include <QFontMetrics>
#include <QGuiApplication>
#include <QInputMethod>

TextInput::TextInput()
{
    Interpreter::setCppOwnerShip(this);
    connect(&m_lineEdit, &EventTextEdit::returnPressed, this, &TextInput::editFinished, Qt::QueuedConnection);
    connect(this, &TextInput::sigSetText, &m_lineEdit, &EventTextEdit::setPlainText, Qt::BlockingQueuedConnection);
    connect(this, &TextInput::sigSetReadonlyToEdit, &m_lineEdit, &EventTextEdit::setEditableKeys, Qt::BlockingQueuedConnection);
    m_toggle.start();
}

TextInput::~TextInput()
{
}

void TextInput::editFinished()
{
    if (getFocused())
    {
        if (onEditFinished())
        {
            looseFocusInternal();
            Tooltip::setEnabled(true);
        }
    }
}


void TextInput::inputMethodEvent(QInputMethodEvent* event)
{
    m_lineEdit.handleTextInputEvent(event);
}

void TextInput::inputMethodQueryEvent(QInputMethodQueryEvent* event)
{        
    m_lineEdit.event(event);
    if (event->queries() & Qt::ImEnabled)
    {
        event->setValue(Qt::ImEnabled, !m_readonly);
    }
}

QString TextInput::getCurrentText() const
{
    return m_lineEdit.toPlainText().trimmed();
}

void TextInput::setCurrentText(const QString text)
{
    m_lineEdit.setPlainText(text);
}

qint32 TextInput::getCursorPosition() const
{
    return m_lineEdit.textCursor().position();
}

void TextInput::setCursorPosition(qint32 position)
{
    auto cursor = m_lineEdit.textCursor();
    cursor.setPosition(position);
    m_lineEdit.setTextCursor(cursor);
}

bool TextInput::getReadonly() const
{
    return m_readonly;
}

QObject* TextInput::getInputMethodFocusObject()
{
    return &m_lineEdit;
}

void TextInput::setReadonly(bool readonly)
{
    m_readonly = readonly;
    m_lineEdit.setEditableKeys(!m_readonly);
}

bool TextInput::doHandleEvent(QEvent *event)
{
    bool ret = false;
    if (m_focused)
    {
        emit sigStartTooltip();
        switch (event->type())
        {
            case QEvent::KeyPress:
            case QEvent::InputMethodQuery:
            case QEvent::InputMethod:
            case QEvent::KeyRelease:
            case QEvent::Shortcut:
            case QEvent::ShortcutOverride:
            {
                ret = m_lineEdit.handleTextInputEvent(event);
                break;
            }
            default:
            {
                CONSOLE_PRINT("Ignoring event: " + QString::number(event->type()), GameConsole::eDEBUG);
                break;
            }
        }
    }
    return ret;
}

void TextInput::focused()
{
    Tooltip::disableTooltip();
    qint32 charCount = m_lineEdit.toPlainText().size();
    auto cursor = m_lineEdit.textCursor();
    if (m_focusPosition < 0 || charCount < m_focusPosition)
    {
        cursor.setPosition(charCount);
    }
    else
    {
        cursor.setPosition(m_focusPosition);
    }
    m_lineEdit.setTextCursor(cursor);
    Mainapp::getInstance()->focusObjectChanged(&m_lineEdit);
    QGuiApplication::inputMethod()->update(Qt::ImQueryAll);
    emit Mainapp::getInstance()->sigShowKeyboard(true);
}

void TextInput::focusedLost()
{
    Tooltip::enableTooltip();
    emit Mainapp::getInstance()->sigShowKeyboard(false);
}

void TextInput::looseFocusInternal()
{
    emit Mainapp::getInstance()->sigShowKeyboard(false);
    Tooltip::looseFocusInternal();
}

QString TextInput::getDrawText(const QString & text)
{
    QString drawText = text;
    if(m_focused)
    {
        auto cursor = m_lineEdit.textCursor();
        qint32 curmsgpos = cursor.position();
        // create output text
        if (m_toggle.elapsed() < BLINKFREQG)
        {
            drawText.insert(curmsgpos, "|");
        }
        else
        {
            drawText.insert(curmsgpos, " ");
        }
        qint32 startMarked = cursor.selectionStart();
        qint32 endMarked = cursor.selectionEnd();
        if (startMarked > -1 && endMarked > -1)
        {
            if (curmsgpos <= startMarked)
            {
                startMarked += 1;
            }
            if (curmsgpos <= startMarked)
            {
                endMarked += 1;
            }
            if (endMarked > startMarked)
            {
                drawText.insert(endMarked, "</div><r>");
                drawText.insert(startMarked, "</r><div c='#00ff00'>");
            }
        }
        if (m_toggle.elapsed() > BLINKFREQG * 2)
        {
            m_toggle.start();
        }
    }
    drawText = "<r>" + drawText + "</r>";
    return drawText;
}

qint32 TextInput::getClickedLinePosition(qint32 x, const QString & line, const QFont & font)
{
    qint32 cursorPos = 0;
    QFontMetrics metrics(font);
    QString testString;
    for (qint32 i = 0; i < line.size(); ++i)
    {
        testString += line[i];
        qint32 width = metrics.boundingRect(testString).width();
        if (x < width)
        {
            break;
        }
        ++cursorPos;
    }
    return cursorPos;
}

void TextInput::focusedInternal()
{
    m_lineEdit.setFocus();
    FocusableObject::focusedInternal();
}