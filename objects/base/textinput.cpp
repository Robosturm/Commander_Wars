#include "objects/base/textinput.h"

#include "coreengine/console.h"
#include "coreengine/mainapp.h"

#include <QGuiApplication>
#include <QClipboard>

TextInput::TextInput()
{
    Mainapp* pApp = Mainapp::getInstance();
    emit pApp->sigCreateLineEdit();
    m_lineEdit = pApp->getLastCreateLineEdit();
    connect(m_lineEdit, &QLineEdit::returnPressed, this, &TextInput::editFinished, Qt::QueuedConnection);
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

QString TextInput::getCurrentText() const
{
    return m_lineEdit->text().trimmed();
}

void TextInput::setCurrentText(QString text)
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

void TextInput::doHandleEvent(QEvent *event)
{
    if (m_focused)
    {
        restartTooltiptimer();
        switch (event->type())
        {
            case QEvent::KeyPress:
            case QEvent::InputMethodQuery:
            case QEvent::InputMethod:
            case QEvent::KeyRelease:
            {
                m_lineEdit->event(event);
                break;
            }
            default:
            {
                // do nothing
                break;
            }
        }
    }
    FocusableObject::doHandleEvent(event);
}

void TextInput::focused()
{
    Tooltip::disableTooltip();
    m_lineEdit->setCursorPosition(m_lineEdit->text().size());
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->show();
    }
}

void TextInput::focusedLost()
{
    Tooltip::enableTooltip();
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->hide();
    }
}

void TextInput::looseFocusInternal()
{
    auto virtualKeyboard = QGuiApplication::inputMethod();
    if (virtualKeyboard != nullptr)
    {
        virtualKeyboard->hide();
    }
    Tooltip::looseFocusInternal();
}

QString TextInput::getDrawText(QString text)
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
