#include "objects/base/coloredbar.h"

ColoredBar::ColoredBar(qint32 width, qint32 height, const QString & backSpriteId, const QString & foreSpriteId)
{
    Interpreter::setCppOwnerShip(this);
    ObjectManager *pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim * pAnim = pObjectManager->getResAnim(backSpriteId);
    setSize(width, height);
    m_background = MemoryManagement::create<oxygine::Sprite>();
    m_background->setResAnim(pAnim);
    if (pAnim != nullptr)
    {
        m_background->setScaleX(static_cast<float>(width) / static_cast<float>(pAnim->getWidth()));
        m_background->setScaleX(static_cast<float>(height) / static_cast<float>(pAnim->getHeight()));
    }
    addChild(m_background);
    m_leftBar = MemoryManagement::create<oxygine::ColorRectSprite>();
    addChild(m_leftBar);
    m_rightBar = MemoryManagement::create<oxygine::ColorRectSprite>();
    addChild(m_rightBar);
    m_foreground = MemoryManagement::create<oxygine::Sprite>();
    pAnim = pObjectManager->getResAnim(foreSpriteId);
    m_foreground->setResAnim(pAnim);
    if (pAnim != nullptr)
    {
        m_foreground->setScaleX(static_cast<float>(width) / static_cast<float>(pAnim->getWidth()));
        m_foreground->setScaleX(static_cast<float>(height) / static_cast<float>(pAnim->getHeight()));
    }
    addChild(m_foreground);
    m_label = MemoryManagement::create<Label>(width - 5);
    m_label->setPosition(5, 0);
    oxygine::TextStyle smallStyle = oxygine::TextStyle(FontManager::getFont("infoBar16"));
    smallStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    smallStyle.multiline = false;
    m_label->setStyle(smallStyle);
    addChild(m_label);
}

Label* ColoredBar::getLabel()
{
    return m_label.get();
}

void ColoredBar::setIsVisible(bool vis)
{
    setVisible(vis);
}

void ColoredBar::changeBar(float divider, QColor leftColor, QColor rightColor)
{
    m_leftBar->setColor(leftColor);
    m_leftBar->setSize(getWidth() * divider, getHeight());
    m_rightBar->setX(m_leftBar->getWidth());
    m_rightBar->setColor(rightColor);
    m_rightBar->setSize(getWidth() * (1.0f - divider), getHeight());
}

void ColoredBar::setText(const QString & text)
{
    m_label->setHtmlText(text);
}

void ColoredBar::setPosX(qint32 x)
{
    setX(x);
}

void ColoredBar::setPosY(qint32 y)
{
    setY(y);
}