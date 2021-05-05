#include "dropdownmenusprite.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DropDownmenuSprite::DropDownmenuSprite(qint32 width, QVector<QString>& items, std::function<oxygine::spActor(QString item)> creator, qint32 dropDownWidth)
    : DropDownmenuBase(width, items.size()),
      m_ItemTexts(items),
      m_Creator(creator)
{
    setObjectName("DropDownmenuSprite");
    Q_ASSERT(items.size() != 0);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setWidth(width);
    m_pClipActor->setY(5);

    for (qint32 i = 0; i < m_ItemTexts.size(); i++)
    {
        addDropDownText(m_ItemTexts[i], i, dropDownWidth);
    }
    DropDownmenuSprite::setCurrentItem(0);
}

DropDownmenuSprite::DropDownmenuSprite(qint32 width, QStringList& items, std::function<oxygine::spActor(QString item)> creator, qint32 dropDownWidth)
    : DropDownmenuBase(width, items.size()),
      m_Creator(creator)
{
    Q_ASSERT(items.size() != 0);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setWidth(width);
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
        pSprite->setScaleX((m_Box->getWidth() - 13 - 65) / pSprite->getWidth());
        pSprite->setScaleY((m_Box->getHeight() - 12) / pSprite->getHeight());
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
    pSprite->setPosition(8, 5);
    auto size = addDropDownItem(pSprite, id);
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

void DropDownmenuSprite::itemChanged(qint32 id)
{
    setCurrentItem(id);
    emit sigItemChanged(m_currentItem);
}
