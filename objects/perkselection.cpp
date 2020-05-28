#include "perkselection.h"

#include "resource_management/coperkmanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"
#include "objects/label.h"
#include "objects/checkbox.h"

PerkSelection::PerkSelection(CO* pCO, qint32 width)
    : m_pCO(pCO)
{
    setWidth(width);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    updatePerksView(pCO);
}

void PerkSelection::updatePerksView(CO* pCO)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pCO = pCO;
    removeChildren();

    QStringList perkList = pCO->getPerkList();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    qint32 count = pCOPerkManager->getCount();
    qint32 y = 0;
    qint32 x = 0;
    const qint32 width = 320;
    for (qint32 i = 0; i < count; i++)
    {
        QString id = pCOPerkManager->getID(i);
        QString name = pCOPerkManager->getName(i);
        QString icon = pCOPerkManager->getIcon(i);
        oxygine::ResAnim* pAnim = pCOPerkManager->getResAnim(icon, oxygine::error_policy::ep_ignore_error);
        QString description = pCOPerkManager->getDescription(i);

        oxygine::spSprite pSprite = new oxygine::Sprite();
        pSprite->setResAnim(pAnim);
        if (pAnim != nullptr)
        {
            pSprite->setScale((GameMap::Imagesize * 2) / pAnim->getWidth());
        }
        pSprite->setPosition(x, y);
        addChild(pSprite);
        spLabel pLabel = new Label(200);
        pLabel->setStyle(style);
        pLabel->setText(name);
        pLabel->setPosition(x + GameMap::Imagesize * 2 + 10, y + 10);
        addChild(pLabel);
        spCheckbox pCheckbox = new Checkbox();
        pCheckbox->setPosition(pLabel->getX() + 200 + 10, y + 5);
        pCheckbox->setTooltipText(description);
        pCheckbox->setChecked(perkList.contains(id));
        connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
        {
            if (value)
            {
                m_pCO->addPerk(id);
            }
            else
            {
                m_pCO->removePerk(id);
            }
        });
        addChild(pCheckbox);

        x += width;
        if (x + width > getWidth())
        {
            x = 0;
            y += GameMap::Imagesize * 2 + 10;
        }
    }
    y += GameMap::Imagesize * 2 + 10;
    setHeight(y);
    pApp->continueThread();
}
