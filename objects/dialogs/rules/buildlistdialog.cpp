#include "buildlistdialog.h"

#include "qdiriterator.h"

#include "coreengine/filesupport.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/unitspritemanager.h"

#include "game/gamemap.h"

#include "objects/base/panel.h"

#include "objects/dialogs/dialogtextinput.h"
#include "objects/base/label.h"

BuildListDialog::BuildListDialog(qint32 player, QStringList buildList)
    : QObject(),
      m_CurrentBuildList(buildList),
      m_Player(player)
{
    setObjectName("BuildListDialog");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getWidth() - 30, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_Player, m_CurrentBuildList);
        detach();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, Settings::getHeight() - 30 - m_ExitButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit canceled();
        detach();
    });

    m_ToggleAll = pObjectManager->createButton(tr("Un/Select All"), 180);
    m_ToggleAll->setPosition(Settings::getWidth() / 2 + 60 , Settings::getHeight() - 30 - m_ToggleAll->getHeight());
    pSpriteBox->addChild(m_ToggleAll);
    m_ToggleAll->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        m_toggle = !m_toggle;
        for (qint32 i = 0; i < m_Checkboxes.size(); i++)
        {
            m_Checkboxes[i]->setChecked(m_toggle);
            emit m_Checkboxes[i]->checkChanged(m_toggle);
        }
    });
    auto items = getNameList();
    m_PredefinedLists = spDropDownmenu::create(300, items);

    m_PredefinedLists->setPosition(Settings::getWidth() / 2 + 40 - m_PredefinedLists->getWidth(), Settings::getHeight() - 30 - m_ToggleAll->getHeight());
    pSpriteBox->addChild(m_PredefinedLists);
    connect(m_PredefinedLists.get(), &DropDownmenu::sigItemChanged, this, &BuildListDialog::setBuildlist, Qt::QueuedConnection);

    oxygine::spButton pSave = pObjectManager->createButton(tr("Save"), 150);
    pSave->setPosition(Settings::getWidth() / 2 - pSave->getWidth() / 2, Settings::getHeight() - 75 - m_ToggleAll->getHeight());
    pSave->addClickListener([=](oxygine::Event*)
    {
        emit sigShowSaveBannlist();
    });
    pSpriteBox->addChild(pSave);
    connect(this, &BuildListDialog::sigShowSaveBannlist, this, &BuildListDialog::showSaveBannlist, Qt::QueuedConnection);


    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    // no the fun begins create checkboxes and stuff and a panel down here
    spPanel pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 150),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 150));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;
    spLabel pLabel = spLabel::create(pPanel->getWidth() - 60);
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Build List"));
    pLabel->setPosition(pPanel->getWidth() / 2 - pLabel->getTextRect().getWidth() / 2, 10);
    pPanel->addItem(pLabel);

    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    m_UnitList = pUnitSpriteManager->getUnitsSorted();

    qint32 y = 30 + pLabel->getTextRect().getHeight() * 2;
    qint32 x = 10;
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < m_UnitList.size(); i++)
    {
        QString unitID = m_UnitList[i];

        spUnit pUnit = spUnit::create(unitID, pMap->getPlayer(player), false);

        pLabel = spLabel::create(250);
        pLabel->setStyle(style);
        pLabel->setHtmlText(pUnit->getName());

        pLabel->setPosition(x + 80, y);
        pUnit->setPosition(x + 45, y);
        pUnit->setScale(pUnit->getScale() * 1.25f);
        spCheckbox pCheckbox = spCheckbox::create();
        pCheckbox->setPosition(x, y);
        m_Checkboxes.append(pCheckbox);

        if (m_CurrentBuildList.contains(unitID))
        {
            pCheckbox->setChecked(true);
        }
        else
        {
            pCheckbox->setChecked(false);
        }
        connect(pCheckbox.get(), &Checkbox::checkChanged, this, [=](bool checked)
        {
            if (checked)
            {
                m_CurrentBuildList.append(unitID);
            }
            else
            {
                m_CurrentBuildList.removeAll(unitID);
            }
        });

        pPanel->addItem(pCheckbox);
        pPanel->addItem(pLabel);
        pPanel->addItem(pUnit);

        x += 340;
        if (x + 340 > pPanel->getContentWidth())
        {
            y += 40;
            x = 10;
        }
    }
    pPanel->setContentHeigth(y + 50);
}

void BuildListDialog::setBuildlist(qint32 item)
{
    QStringList data;
    if (item == 0) // Commander Wars
    {
        data = QStringList({QString("APC"), QString("ARTILLERY"), QString("ARTILLERYCRAFT"), QString("BATTLESHIP"), QString("BOMBER"),
                            QString("CRUISER"), QString("FIGHTER"), QString("FLAK"), QString("SNIPER"), QString("WATERMINE"),
                            QString("HEAVY_TANK"), QString("INFANTRY"), QString("K_HELI"), QString("LANDER"), QString("LIGHT_TANK"), QString("MECH"), QString("MISSILE"),
                            QString("RECON"), QString("ROCKETTHROWER"), QString("SUBMARINE"), QString("T_HELI"), QString("NEOTANK"), QString("MEGATANK"),
                            QString("AIRCRAFTCARRIER"), QString("ANTITANKCANNON"), QString("CANNONBOAT"), QString("DUSTER"), QString("FLARE"),
                            QString("MOTORBIKE"), QString("WATERPLANE"), QString("BLACK_BOAT"), QString("BLACK_BOMB"), QString("PIPERUNNER"),
                            QString("STEALTHBOMBER"), QString("DESTROYER"), QString("HEAVY_HOVERCRAFT"), QString("HOVERCRAFT"),
                            QString("HOVERFLAK"), QString("TRANSPORTPLANE"), QString("HOELLIUM")});
    }
    else if (item == 1) // advance wars dc
    {
        data = QStringList({QString("APC"), QString("ARTILLERY"), QString("BATTLESHIP"), QString("BOMBER"), QString("CRUISER"), QString("FIGHTER"), QString("FLAK"),
                QString("HEAVY_TANK"), QString("INFANTRY"), QString("K_HELI"), QString("LANDER"), QString("LIGHT_TANK"), QString("MECH"), QString("MISSILE"),
                QString("RECON"), QString("ROCKETTHROWER"), QString("SUBMARINE"), QString("T_HELI"), QString("MEGATANK"),
                QString("AIRCRAFTCARRIER"), QString("ANTITANKCANNON"), QString("CANNONBOAT"), QString("DUSTER"), QString("FLARE"),
                QString("MOTORBIKE"), QString("WATERPLANE")});
    }
    else if (item == 2) // advance wars ds
    {
        data = QStringList({QString("APC"), QString("ARTILLERY"), QString("BATTLESHIP"), QString("BOMBER"), QString("CRUISER"), QString("FIGHTER"), QString("FLAK"),
                QString("HEAVY_TANK"), QString("INFANTRY"), QString("K_HELI"), QString("LANDER"), QString("LIGHT_TANK"), QString("MECH"), QString("MISSILE"),
                QString("RECON"), QString("ROCKETTHROWER"), QString("SUBMARINE"), QString("T_HELI"), QString("NEOTANK"), QString("MEGATANK"),
                QString("AIRCRAFTCARRIER"), QString("BLACK_BOAT"), QString("BLACK_BOMB"), QString("PIPERUNNER"), QString("STEALTHBOMBER")});
    }
    else if (item == 3) // advance wars 2
    {
        data = QStringList({QString("APC"), QString("ARTILLERY"), QString("BATTLESHIP"), QString("BOMBER"), QString("CRUISER"), QString("FIGHTER"), QString("FLAK"),
                QString("HEAVY_TANK"), QString("INFANTRY"), QString("K_HELI"), QString("LANDER"), QString("LIGHT_TANK"), QString("MECH"), QString("MISSILE"),
                QString("RECON"), QString("ROCKETTHROWER"), QString("SUBMARINE"), QString("T_HELI"), QString("NEOTANK")});
    }
    else if (item == 4)  // advance wars
    {
        data = QStringList({QString("APC"), QString("ARTILLERY"), QString("BATTLESHIP"), QString("BOMBER"), QString("CRUISER"), QString("FIGHTER"), QString("FLAK"),
                QString("HEAVY_TANK"), QString("INFANTRY"), QString("K_HELI"), QString("LANDER"), QString("LIGHT_TANK"), QString("MECH"), QString("MISSILE"),
                QString("RECON"), QString("ROCKETTHROWER"), QString("SUBMARINE"), QString("T_HELI")});
    }
    else
    {
        QString file = m_PredefinedLists->getCurrentItemText();
        auto fileData = Filesupport::readList(file + ".bl", "data/unitbannlist/");
        data = std::get<1>(fileData);
    }
    for (qint32 i = 0; i < m_UnitList.size(); i++)
    {
        if (data.contains(m_UnitList[i]))
        {
            m_Checkboxes[i]->setChecked(true);
        }
        else
        {
            m_Checkboxes[i]->setChecked(false);
        }
    }
    m_CurrentBuildList = data;
}

void BuildListDialog::showSaveBannlist()
{
    
    spDialogTextInput pSaveInput = spDialogTextInput::create(tr("Banlist Name"), true, "");
    connect(pSaveInput.get(), &DialogTextInput::sigTextChanged, this, &BuildListDialog::saveBannlist, Qt::QueuedConnection);
    addChild(pSaveInput);
    
}

QVector<QString> BuildListDialog::getNameList()
{
    QVector<QString> items = {tr("Commander Wars"),
                              tr("Advance Wars DoR"),
                              tr("Advance Wars DS"),
                              tr("Advance Wars 2"),
                              tr("Advance Wars")};
    QStringList filters;
    filters << "*.bl";
    QDirIterator dirIter("data/unitbannlist/", filters, QDir::Files, QDirIterator::IteratorFlag::NoIteratorFlags);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        std::tuple<QString, QStringList> data = Filesupport::readList(file);
        items.append(std::get<0>(data));
    }
    return items;
}

void BuildListDialog::saveBannlist(QString filename)
{    
    Filesupport::storeList(filename, m_CurrentBuildList, "data/unitbannlist/");
    auto items = getNameList();
    m_PredefinedLists->changeList(items);
}
