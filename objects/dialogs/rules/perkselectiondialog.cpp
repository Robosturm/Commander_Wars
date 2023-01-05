#include "objects/dialogs/rules/perkselectiondialog.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/label.h"
#include "objects/base/dropdownmenu.h"
#include "objects/dialogs/dialogtextinput.h"

#include "game/gamemap.h"

#include "coreengine/filesupport.h"

PerkSelectionDialog::PerkSelectionDialog(GameMap* pMap, Player* pPlayer, qint32 maxPerkcount, bool banning, QStringList hiddenList)
    : m_pPlayer(pPlayer),
      m_banning(banning),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("PerkSelectionDialog");
#endif
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getScaledWidth() / 2,
                            Settings::getHeight() - 30 - m_OkButton->getScaledHeight());
    m_pSpriteBox->addChild(m_OkButton);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    CO* firstCO = nullptr;
    if (!banning)
    {
        spLabel pLabel = spLabel::create(200);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Perks of CO:"));
        pLabel->setPosition(30, 30);
        m_pSpriteBox->addChild(pLabel);
        QStringList list;
        CO* pCO = pPlayer->getCO(0);

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
        spDropDownmenu pDropDownmenu = spDropDownmenu::create(250, list);
        pDropDownmenu->setPosition(210, 30);
        m_pSpriteBox->addChild(pDropDownmenu);
        connect(pDropDownmenu.get(), &DropDownmenu::sigItemChanged, this, &PerkSelectionDialog::changeCO, Qt::QueuedConnection);

        pLabel = spLabel::create(100);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Fill:"));
        pLabel->setPosition(pDropDownmenu->getX() + pDropDownmenu->getScaledWidth() + 10, 30);
        m_pSpriteBox->addChild(pLabel);
        m_randomFillCheckbox = spCheckbox::create();
        m_randomFillCheckbox->setTooltipText(tr("If checked: clicking the 'Random' Button will fill all remaining perk slots instead of replacing all."));
        m_randomFillCheckbox->setPosition(pLabel->getX() + pLabel->getScaledWidth() + 10, 30);
        m_pSpriteBox->addChild(m_randomFillCheckbox);
        oxygine::spButton randomButton = pObjectManager->createButton(tr("Random"), 150);
        randomButton->setPosition(m_randomFillCheckbox->getX() + m_randomFillCheckbox->getScaledWidth() + 10, 30);
        m_pSpriteBox->addChild(randomButton);
        randomButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            emit sigSelectRandomPerks();
        });
        connect(this, &PerkSelectionDialog::sigSelectRandomPerks, this, &PerkSelectionDialog::selectRandomPerks, Qt::QueuedConnection);
    }

    QSize size(Settings::getWidth() - 60,
               Settings::getHeight() - 40 * 3 - m_OkButton->getScaledHeight());
    m_pPanel = spPanel::create(true, size, size);
    if (banning)
    {
        m_pPanel->setPosition(30, 30);
    }
    else
    {
        m_pPanel->setPosition(30, 75);
    }


    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont48());
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;
    spLabel pLabel = spLabel::create(m_pPanel->getScaledWidth() - 60);
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Perk List"));
    pLabel->setPosition(m_pPanel->getScaledWidth() / 2 - pLabel->getTextRect().getWidth() / 2, 10);
    m_pPanel->addItem(pLabel);

    m_pPerkSelection = spPerkSelection::create(firstCO, Settings::getWidth() - 80, maxPerkcount, banning, hiddenList, m_pMap);
    m_pPerkSelection->setY(pLabel->getY() + pLabel->getTextRect().getHeight() + 10);
    m_pPanel->addItem(m_pPerkSelection);
    m_pPanel->setContentHeigth(m_pPerkSelection->getScaledHeight() + m_pPerkSelection->getY() + 40);
    m_pPanel->setContentWidth(m_pPerkSelection->getScaledWidth());
    m_pSpriteBox->addChild(m_pPanel);

    if (banning)
    {
        m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getScaledWidth() - 30,
                                Settings::getHeight() - 30 - m_OkButton->getScaledHeight());

        m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
        m_CancelButton->setPosition(30,
                                    Settings::getHeight() - 30 - m_CancelButton->getScaledHeight());
        m_pSpriteBox->addChild(m_CancelButton);
        m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            emit sigCancel();
        });

        m_pSave = pObjectManager->createButton(tr("Save"), 150);
        m_pSave->setPosition(Settings::getWidth() / 2 - m_pSave->getScaledWidth() / 2,
                           Settings::getHeight() - 30 - m_CancelButton->getScaledHeight());
        m_pSave->addClickListener([this](oxygine::Event*)
        {
            emit sigShowSavePerklist();
        });
        m_pSpriteBox->addChild(m_pSave);
        connect(this, &PerkSelectionDialog::sigShowSavePerklist, this, &PerkSelectionDialog::showSavePerklist, Qt::QueuedConnection);

        m_ToggleAll = pObjectManager->createButton(tr("Un/Select All"), 180);
        m_ToggleAll->setPosition(Settings::getWidth() / 2 + 60 ,
                                 Settings::getHeight() - 75 - m_ToggleAll->getScaledHeight());
        m_pSpriteBox->addChild(m_ToggleAll);
        m_ToggleAll->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            m_toggle = !m_toggle;
            emit sigToggleAll(m_toggle);
        });
        connect(this, &PerkSelectionDialog::sigToggleAll, m_pPerkSelection.get(), &PerkSelection::toggleAll, Qt::QueuedConnection);
        auto items = getNameList("data/perkbannlist/");
        m_PredefinedLists = spDropDownmenu::create(260, items);

        m_PredefinedLists->setPosition(Settings::getWidth() / 2 + 40 - m_PredefinedLists->getScaledWidth(),
                                       Settings::getHeight() - 75 - m_ToggleAll->getScaledHeight());
        m_pSpriteBox->addChild(m_PredefinedLists);
        connect(m_PredefinedLists.get(), &DropDownmenu::sigItemChanged, this, &PerkSelectionDialog::setPerkBannlist, Qt::QueuedConnection);
    }
    else
    {
        m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getScaledWidth() - 30,
                                Settings::getHeight() - 30 - m_OkButton->getScaledHeight());

        m_pSave = pObjectManager->createButton(tr("Save"), 150);
        m_pSave->setPosition(Settings::getWidth() / 2 + 60 ,
                           Settings::getHeight() - 30 - m_pSave->getScaledHeight());
        m_pSave->addClickListener([this](oxygine::Event*)
        {
            emit sigShowSavePerklist();
        });
        m_pSpriteBox->addChild(m_pSave);
        connect(this, &PerkSelectionDialog::sigShowSavePerklist, this, &PerkSelectionDialog::showSavePerklist, Qt::QueuedConnection);
        updatePredefinedList();
    }

    auto* pPerkSelection = m_pPerkSelection.get();
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this, pPerkSelection](oxygine::Event*)
    {
        emit editFinished(pPerkSelection->getPerks());
        emit sigFinished();
    });

    connect(this, &PerkSelectionDialog::sigCancel, this, &PerkSelectionDialog::remove, Qt::QueuedConnection);
    connect(this, &PerkSelectionDialog::sigFinished, this, &PerkSelectionDialog::remove, Qt::QueuedConnection);
}

void PerkSelectionDialog::remove()
{
    detach();
}

void PerkSelectionDialog::setPerkBannlist(qint32)
{
    if (m_banning)
    {
        QString file = m_PredefinedLists->getCurrentItemText();
        auto fileData = Filesupport::readList(file + ".bl", "data/perkbannlist/");
        m_pPerkSelection->setPerks(fileData.items);
    }
    else
    {
        QString file = m_PredefinedLists->getCurrentItemText();
        auto fileData = Filesupport::readList(file + ".bl", "data/perkselection/");
        QStringList perks = fileData.items;
        qint32 i = 0;
        
        while (i < perks.size())
        {
            if (m_pMap->getGameRules()->getAllowedPerks().contains(perks[i]))
            {
                ++i;
            }
            else
            {
                perks.removeAt(i);
            }
        }
        qint32 maxPerkCount = m_pMap->getGameRules()->getMaxPerkCount();
        while (perks.size() > maxPerkCount)
        {
            perks.removeLast();
        }
        m_pPerkSelection->setPerks(perks);
    }
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

void PerkSelectionDialog::showSavePerklist()
{    
    spDialogTextInput pSaveInput = spDialogTextInput::create(tr("Perklist Name"), true, "");
    connect(pSaveInput.get(), &DialogTextInput::sigTextChanged, this, &PerkSelectionDialog::savePerklist, Qt::QueuedConnection);
    addChild(pSaveInput);
}

QStringList PerkSelectionDialog::getNameList(QString path)
{
    QStringList items;
    QStringList filters;
    filters << "*.bl";
    QDirIterator dirIter(path, filters, QDir::Files, QDirIterator::IteratorFlag::NoIteratorFlags);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().canonicalFilePath();
        auto data = Filesupport::readList(file);
        items.append(data.name);
    }
    return items;
}

void PerkSelectionDialog::savePerklist(QString filename)
{
    QString path;
    if (m_banning)
    {
        path = "data/perkbannlist/";
    }
    else
    {
        path = "data/perkselection/";
    }
    Filesupport::storeList(filename, m_pPerkSelection->getPerks(), path);
    auto items = getNameList(path);
    updatePredefinedList();
}

void PerkSelectionDialog::selectRandomPerks()
{
    m_pPerkSelection->selectRandomPerks(m_randomFillCheckbox->getChecked());
}

void PerkSelectionDialog::updatePredefinedList()
{
    if (m_PredefinedLists.get() != nullptr)
    {
        m_PredefinedLists->detach();
    }
    QString path;
    if (m_banning)
    {
        path = "data/perkbannlist/";
    }
    else
    {
        path = "data/perkselection/";
    }
    auto items = getNameList(path);
    m_PredefinedLists = spDropDownmenu::create(260, items);
    m_PredefinedLists->setPosition(Settings::getWidth() / 2 + 40 - m_PredefinedLists->getScaledWidth(),
                                   Settings::getHeight() - 30 - m_pSave->getScaledHeight());
    m_pSpriteBox->addChild(m_PredefinedLists);
    connect(m_PredefinedLists.get(), &DropDownmenu::sigItemChanged, this, &PerkSelectionDialog::setPerkBannlist, Qt::QueuedConnection);
}
