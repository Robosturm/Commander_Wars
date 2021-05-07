#include "passwordbox.h"

Passwordbox::Passwordbox(qint32 width, qint32 heigth)
    : Textbox(width, heigth)
{
    setObjectName("Passwordbox");
}

void Passwordbox::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    if(m_focused)
    {
        // create output text
        QString drawText = getVisiblePasswordText();
        if (m_toggle.elapsed() < BLINKFREQG)
        {
            drawText.insert(m_curmsgpos,"|");
        }
        else
        {
            drawText.insert(m_curmsgpos," ");
        }
        if (m_toggle.elapsed() > BLINKFREQG * 2)
        {
            m_toggle.start();
        }
        m_Textfield->setHtmlText(drawText);

        if (m_Text.size() > 0)
        {
            // calc text field position based on curmsgpos
            qint32 xPos = 0;
            qint32 fontWidth = m_Textfield->getTextRect().getWidth() / m_Text.size();
            qint32 boxSize = (m_Textbox->getWidth() - 40 - fontWidth);
            xPos = -fontWidth * m_curmsgpos + boxSize / 2;
            if (xPos > 0)
            {
                xPos = 0;
            }
            else if ((m_Text.size() - m_curmsgpos + 3) * fontWidth < boxSize)
            {
                xPos = m_Textbox->getWidth() - m_Textfield->getTextRect().getWidth() - fontWidth * 3;
                if (xPos > 0)
                {
                    xPos = 0;
                }
            }
            else
            {
                // all fine
            }
            m_Textfield->setX(xPos);
        }
    }
    else
    {
        m_Textfield->setHtmlText(getVisiblePasswordText());
    }
    oxygine::Actor::update(us);
}

QString Passwordbox::getVisiblePasswordText()

{
    QString ret;
    for (qint32 i = 0; i < m_Text.size(); i++)
    {
        ret += '*';
    }
    return ret;
}
