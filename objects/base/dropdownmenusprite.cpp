#include "objects/base/dropdownmenusprite.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DropDownmenuSprite::DropDownmenuSprite(qint32 width, QStringList& items, std::function<oxygine::spActor(QString item)> creator, qint32 dropDownWidth, bool autoScale)
    : DropDownmenuBase(width, items.size()),
      m_Creator(creator),
      m_autoScale(autoScale)

{
#ifdef GRAPHICSUPPORT
    setObjectName("DropDownmenuSprite");
#endif
    if (items.size() < 0)
    {
        oxygine::handleErrorPolicy(oxygine::ep_show_error, "DropDownmenuSprite item count is below 0");
    }
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setWidth(width);
    m_pClipActor->setY(5);

    for (qint32 i = 0; i < items.size(); i++)
    {
        m_ItemTexts.append(items[i]);
        addDropDownText(m_ItemTexts[i], i, dropDownWidth);
    }
    DropDownmenuSprite::setCurrentItem(0);
}

void DropDownmenuSprite::setCurrentItem(qint32 index)
{
    if ((index >= 0) && (index < m_ItemTexts.size()))
    {
        m_currentItem = index;
        m_currentText = m_ItemTexts[index];
        m_pClipActor->removeChildren();
        oxygine::spActor pSprite = m_Creator(m_ItemTexts[index]);
        if (m_autoScale)
        {
            pSprite->setScaleX((m_Box->getWidth() - 13 - 65) / pSprite->getScaledWidth());
            pSprite->setScaleY((m_Box->getHeight() - 12) / pSprite->getScaledHeight());
        }
        m_pClipActor->addChild(pSprite);
        
    }
}

void DropDownmenuSprite::setCurrentItem(QString item)
{
    for (qint32 i = 0; i < m_ItemTexts.size(); i++)
    {
        if (m_ItemTexts[i] == item)
        {
            setCurrentItem(i);
            break;
        }
    }
}

QString DropDownmenuSprite::getCurrentItemText()
{
    return m_currentText;
}

void DropDownmenuSprite::addDropDownText(QString spriteID, qint32 id, qint32 dropDownWidth)
{
    oxygine::spActor pSprite = m_Creator(spriteID);
    pSprite->setPosition(pSprite->getX() + 8, pSprite->getY() + 5);
    auto size = addDropDownItem(pSprite, id);
    if (m_autoScale)
    {
        if (dropDownWidth > 0)
        {
            pSprite->setScaleX((dropDownWidth) / pSprite->getScaledWidth());
        }
        else
        {
            pSprite->setScaleX((size.x - 13.0f) / pSprite->getScaledWidth());
        }
        float spriteHeigth = pSprite->getScaledHeight();
        pSprite->setScaleY((size.y - 12.0f) / spriteHeigth);
    }
}

void DropDownmenuSprite::itemChanged(qint32 id)
{
    setCurrentItem(id);
    emit sigItemChanged(m_currentItem);
}
