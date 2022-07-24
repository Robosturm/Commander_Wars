#include "objects/base/textinput.h"

#include "coreengine/console.h"
#include "coreengine/mainapp.h"

TextInput::TextInput()
{
    Mainapp* pApp = Mainapp::getInstance();
    emit pApp->sigCreateLineEdit();
    m_lineEdit = pApp->getLastCreateLineEdit();    
    connect(m_lineEdit, &QLineEdit::returnPressed, this, &TextInput::editFinished, Qt::QueuedConnection);
    connect(m_lineEdit, &QLineEdit::cursorPositionChanged, pApp, &Mainapp::slotCursorPositionChanged);
    m_toggle.start();
}

TextInput::~TextInput()
{
    m_lineEdit->deleteLater();
}

void TextInput::editFinished()
{
    if (onEditFinished())
    {
        looseFocusInternal();
        Tooltip::setEnabled(true);
    }
}

void TextInput::inputMethodQuery(Qt::InputMethodQuery query, QVariant arg)
{
    m_lineEdit->inputMethodQuery(query, arg);
}

QString TextInput::getCurrentText() const
{
    return m_lineEdit->text().trimmed();
}

void TextInput::setCurrentText(const QString & text)
{
    m_lineEdit->setText(text);
}

qint32 TextInput::getCursorPosition() const
{
    return m_lineEdit->cursorPosition();
}

void TextInput::setCursorPosition(qint32 position)
{
    m_lineEdit->setCursorPosition(position);
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
                ret = m_lineEdit->event(event);
                break;
            }
            default:
            {
                CONSOLE_PRINT("Ignoring event: " + QString::number(event->type()), Console::eDEBUG);
                break;
            }
        }
    }
    return ret;
}

void TextInput::focused()
{
    Tooltip::disableTooltip();
    m_lineEdit->setCursorPosition(m_lineEdit->text().size());
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
        qint32 curmsgpos = getCursorPosition();
        // create output text
        if (m_toggle.elapsed() < BLINKFREQG)
        {
            drawText.insert(curmsgpos, "|");
        }
        else
        {
            drawText.insert(curmsgpos, " ");
        }
        qint32 startMarked = m_lineEdit->selectionStart();
        qint32 endMarked = m_lineEdit->selectionEnd();
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
            drawText.insert(endMarked, "</div><r>");
            drawText.insert(startMarked, "</r><div c='#00ff00'>");
        }
        if (m_toggle.elapsed() > BLINKFREQG * 2)
        {
            m_toggle.start();
        }
    }
    drawText = "<r>" + drawText + "</r>";
    return drawText;
}
