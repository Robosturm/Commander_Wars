#include "perkselectiondialog.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "objects/label.h"
#include "objects/dropdownmenu.h"

PerkSelectionDialog::PerkSelectionDialog(Player* pPlayer)
    : m_pPlayer(pPlayer)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        detach();
    });

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    spLabel pLabel = new Label(200);
    pLabel->setStyle(style);
    pLabel->setText("Perk's of CO:");
    pLabel->setPosition(30, 30);
    pSpriteBox->addChild(pLabel);
    QVector<QString> list;
    CO* pCO = pPlayer->getCO(0);
    CO* firstCO = nullptr;
    if (pCO != nullptr)
    {
        firstCO = pCO;
        list.append(pCO->getCOName());
    }
    pCO = pPlayer->getCO(1);
    if (pCO != nullptr)
    {
        list.append(pCO->getCOName());
        if (firstCO == nullptr)
        {
            firstCO = pCO;
        }
    }
    spDropDownmenu pDropDownmenu = new DropDownmenu(250, list);
    pDropDownmenu->setPosition(210, 30);
    pSpriteBox->addChild(pDropDownmenu);
    connect(pDropDownmenu.get(), &DropDownmenu::sigItemChanged, this, &PerkSelectionDialog::changeCO, Qt::QueuedConnection);

    QSize size(Settings::getWidth() - 60, Settings::getHeight() - 40 * 3 - m_OkButton->getHeight());
    m_pPanel = new  Panel(true, size, size);
    m_pPanel->setPosition(30, 75);
    m_pPerkSelection = new PerkSelection(firstCO, Settings::getWidth() - 80);
    m_pPanel->addItem(m_pPerkSelection);
    m_pPanel->setContentHeigth(m_pPerkSelection->getHeight() + 40);
    m_pPanel->setContentWidth(m_pPerkSelection->getWidth());
    pSpriteBox->addChild(m_pPanel);
}

void PerkSelectionDialog::changeCO(qint32 index)
{
    CO* pCO = nullptr;
    if (index == 0)
    {
        pCO = m_pPlayer->getCO(index);
        if (pCO == nullptr)
        {
            pCO = m_pPlayer->getCO(1);
        }
    }
    else
    {
        pCO = m_pPlayer->getCO(index);
    }
    m_pPerkSelection->updatePerksView(pCO);
}
