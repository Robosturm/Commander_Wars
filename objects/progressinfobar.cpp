#include "progressinfobar.h"
#include "resource_management/fontmanager.h"

ProgressInfoBar::ProgressInfoBar(qint32 width, qint32 heigth, QString text, float progress)
{
    m_Background = new oxygine::ColorRectSprite();
    m_Background->setSize(width, heigth);
    m_Background->setColor(QColor(100, 100, 100, 150));
    addChild(m_Background);

    m_Progressbar = new oxygine::ColorRectSprite();
    m_Progressbar->setSize(width * progress, heigth);
    m_Progressbar->setColor(QColor(35, 180, 80, 255));
    addChild(m_Progressbar);

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;
    m_Label = new Label(width);
    m_Label->setStyle(style);
    m_Label->setText(text);
    addChild(m_Label);
}

void ProgressInfoBar::setText(QString text)
{
    m_Label->setText(text);
}

void ProgressInfoBar::setProgress(qint32 progress)
{
    if (progress > 1.0f)
    {
        progress = 1.0f;
    }
    else if (progress < 0.0f)
    {
        progress = 0.0f;
    }
    m_Progressbar->setWidth(m_Background->getWidth() * progress);
}
