#include "dialogmodifybuilding.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "objects/base/label.h"
#include "objects/base/textbox.h"
#include "objects/base/dropdownmenu.h"

#include "game/gamemap.h"

DialogModifyBuilding::DialogModifyBuilding(GameMap* pMap, Building* pBuilding)
    : m_pBuilding(pBuilding)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DialogModifyBuilding");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });


    m_pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont48());
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    qint32 sliderOffset = 400;
    spLabel pLabel = spLabel::create(m_pPanel->getWidth() - 80);
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Building: ") + m_pBuilding->getName());
    pLabel->setPosition(m_pPanel->getWidth() / 2 - pLabel->getTextRect().getWidth() / 2, 10);
    m_pPanel->addItem(pLabel);

    qint32 y = 30 + pLabel->getTextRect().getHeight();
    pLabel = spLabel::create(sliderOffset - 140);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Player: "));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    QStringList items;
    
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        items.append(tr("Player ") + QString::number(i + 1));
    }
    items.append(tr("Neutral"));
    spDropDownmenu pDropdownmenu = spDropDownmenu::create(300, items);
    pDropdownmenu->setTooltipText(tr("Selects the Owner of the current unit. This is immediately applied."));
    pDropdownmenu->setPosition(sliderOffset - 160, y);

    Player* pPlayer = m_pBuilding->getOwner();
    if (pPlayer != nullptr)
    {
        pDropdownmenu->setCurrentItem(pPlayer->getPlayerID());
    }
    else
    {
        pDropdownmenu->setCurrentItem(pMap->getPlayerCount());
    }

    connect(pDropdownmenu.get(), &DropDownmenu::sigItemChanged, this, [this, pMap](qint32 value)
    {
        if (value >= pMap->getPlayerCount())
        {
            m_pBuilding->setOwner(nullptr);
        }
        else
        {
            m_pBuilding->setOwner(pMap->getPlayer(value));
        }
    });
    m_pPanel->addItem(pDropdownmenu);
    y += 40;

    pLabel = spLabel::create(190);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Name:"));
    pLabel->setPosition(10, y);
    spTextbox pTextbox = spTextbox::create(m_pPanel->getContentWidth() - 100 - 200 - pLabel->getWidth());
    pTextbox->setTooltipText(tr("Custom Name of the Terrain. Leave the name empty to use its Default Name."));
    pTextbox->setPosition(sliderOffset - 160, y);
    pTextbox->setCurrentText(m_pBuilding->getBuildingName());
    connect(pTextbox.get(), &Textbox::sigTextChanged, m_pBuilding, &Building::setBuildingName, Qt::QueuedConnection);
    m_pPanel->addItem(pTextbox);
    m_pPanel->addItem(pLabel);
    y += 40;
    connect(this, &DialogModifyBuilding::sigFinished, this, &DialogModifyBuilding::remove, Qt::QueuedConnection);
}

void DialogModifyBuilding::remove()
{
    detach();
}
