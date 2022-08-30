#include "objects/base/progressinfobar.h"
#include "resource_management/fontmanager.h"

ProgressInfoBar::ProgressInfoBar(qint32 width, qint32 heigth, QString text, float progress)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ProgressInfoBar");
#endif
    m_Background = oxygine::spColorRectSprite::create();
    m_Background->setSize(width, heigth);
    m_Background->setColor(QColor(100, 100, 100, 150));
    addChild(m_Background);

    m_Progressbar = oxygine::spColorRectSprite::create();
    m_Progressbar->setSize(width * progress, heigth);
    m_Progressbar->setColor(QColor(35, 180, 80, 255));
    addChild(m_Progressbar);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;
    m_Label = spLabel::create(width);
    m_Label->setStyle(style);
    m_Label->setHtmlText(text);
    addChild(m_Label);
}

void ProgressInfoBar::setText(QString text)
{
    m_Label->setHtmlText(text);
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
