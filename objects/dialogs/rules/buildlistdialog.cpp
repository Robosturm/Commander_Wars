#include "objects/dialogs/rules/buildlistdialog.h"

#include <QDirIterator>

#include "coreengine/filesupport.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/unitspritemanager.h"

#include "game/gamemap.h"

#include "objects/base/panel.h"

#include "objects/dialogs/dialogtextinput.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/label.h"

const char* const BuildListDialog::FILEPATH = "data/unitbannlist/";

BuildListDialog::BuildListDialog(GameMap* pMap, qint32 player, QStringList buildList)
    : m_CurrentBuildList(buildList),
      m_Player(player),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("BuildListDialog");
#endif
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    // no the fun begins create checkboxes and stuff and a panel down here
    spPanel pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 150),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 150));
    pPanel->setPosition(30, 30);
    m_pSpriteBox->addChild(pPanel);

    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont48());
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    spLabel pLabel = spLabel::create(pPanel->getScaledWidth() - 60);
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Build List"));
    pLabel->setPosition(pPanel->getScaledWidth() / 2 - pLabel->getTextRect().getWidth() / 2, 10);
    pPanel->addItem(pLabel);
    qint32 y = 10 + pLabel->getY() + pLabel->getTextRect().getHeight();
    qint32 x = 10;
    

    auto unitGroups = getUnitGroups();

    for (const auto & group : unitGroups)
    {
        spLabel textField = spLabel::create(pPanel->getScaledWidth() - 40);
        textField->setStyle(headerStyle);
        textField->setHtmlText(group.name);
        textField->setPosition(pPanel->getScaledWidth() / 2 - textField->getTextRect().getWidth() / 2, y);
        pPanel->addItem(textField);
        y += textField->getHeight() + 10;
        x = 10;
        for (qint32 i = 0; i < group.units.size(); ++i)
        {
            QString unitID = group.units[i];
            spUnit pUnit = spUnit::create(unitID, m_pMap->getPlayer(player), false, m_pMap);
            pLabel = spLabel::create(250);
            pLabel->setStyle(style);
            pLabel->setHtmlText(pUnit->getName());
            pLabel->setPosition(x + 90, y);
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
            connect(pCheckbox.get(), &Checkbox::checkChanged, this, [this, unitID](bool checked)
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

            x += 350;
            if (x + 350 > pPanel->getContentWidth() &&
                i < group.units.size() - 1)
            {
                y += pLabel->getHeight() + 10;
                x = 10;
            }
        }
        y += textField->getHeight() + 10;
    }
    pPanel->setContentHeigth(y + 50);
    connect(this, &BuildListDialog::canceled, this, &BuildListDialog::remove, Qt::QueuedConnection);
    connect(this, &BuildListDialog::sigFinished, this, &BuildListDialog::remove, Qt::QueuedConnection);

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getScaledWidth() - 30,
                            Settings::getHeight() - 30 - m_OkButton->getScaledHeight());
    m_pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit editFinished(m_Player, m_CurrentBuildList);
        emit sigFinished();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30,
                              Settings::getHeight() - 30 - m_ExitButton->getScaledHeight());
    m_pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit canceled();
    });

    oxygine::spButton pSave = pObjectManager->createButton(tr("Save"), 150);
    pSave->setPosition(Settings::getWidth() / 2 + 10,
                       Settings::getHeight() - 30 - m_ExitButton->getScaledHeight());
    pSave->addClickListener([this](oxygine::Event*)
    {
        emit sigShowSaveBannlist();
    });
    m_pSpriteBox->addChild(pSave);
    connect(this, &BuildListDialog::sigShowSaveBannlist, this, &BuildListDialog::showSaveBannlist, Qt::QueuedConnection);
    connect(this, &BuildListDialog::sigDoSaveBannlist, this, &BuildListDialog::doSaveBannlist, Qt::QueuedConnection);

    oxygine::spButton pDelete = pObjectManager->createButton(tr("Delete"), 150);
    pDelete->setPosition(Settings::getWidth() / 2 - pDelete->getScaledWidth() - 10,
                       Settings::getHeight() - 30 - m_ExitButton->getScaledHeight());
    pDelete->addClickListener([this](oxygine::Event*)
    {
        emit sigShowDeleteBannlist();
    });
    m_pSpriteBox->addChild(pDelete);
    connect(this, &BuildListDialog::sigShowDeleteBannlist, this, &BuildListDialog::showDeleteBannlist, Qt::QueuedConnection);
    connect(this, &BuildListDialog::sigDeleteBannlist, this, &BuildListDialog::deleteBannlist, Qt::QueuedConnection);

    m_ToggleAll = pObjectManager->createButton(tr("Un/Select All"), 180);
    m_ToggleAll->setPosition(Settings::getWidth() / 2 + 10,
                             Settings::getHeight() - 75 - m_ToggleAll->getScaledHeight());
    m_pSpriteBox->addChild(m_ToggleAll);
    m_ToggleAll->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        m_toggle = !m_toggle;
        for (qint32 i = 0; i < m_Checkboxes.size(); i++)
        {
            m_Checkboxes[i]->setChecked(m_toggle);
            emit m_Checkboxes[i]->checkChanged(m_toggle);
        }
    });
    updatePredefinedList();
}

QVector<BuildListDialog::UnitGroup> BuildListDialog::getUnitGroups()
{
    QVector<BuildListDialog::UnitGroup> ret;
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    m_UnitList = pUnitSpriteManager->getUnitsSorted();
    UnitGroup currentGroup;
    qint32 currentIdentifier = std::numeric_limits<qint32>::min();
    for (const auto & unit : qAsConst(m_UnitList))
    {
        qint32 newIdentifier = pUnitSpriteManager->getUnitType(unit);
        if (newIdentifier != currentIdentifier)
        {
            currentIdentifier = newIdentifier;
            if (currentGroup.units.size() > 0)
            {
                ret.append(currentGroup);
            }
            currentGroup.name = pUnitSpriteManager->getUnitTypeText(currentIdentifier);
            currentGroup.units.clear();
        }
        currentGroup.units.append(unit);
    }
    if (currentGroup.units.size() > 0)
    {
        ret.append(currentGroup);
    }
    return ret;
}

void BuildListDialog::remove()
{
    m_Checkboxes.clear();
    detach();
}

void BuildListDialog::setBuildlist(qint32 item)
{
    QStringList data;
    if (item == 0) // Commander Wars
    {
        data = QStringList({QString("AIRCRAFTCARRIER"), QString("ANTITANKCANNON"), QString("APC"), QString("ARTILLERY"), QString("ARTILLERYCRAFT"),
                            QString("BATTLECRUISER"), QString("BATTLESHIP"), QString("BLACK_BOAT"), QString("BLACK_BOMB"), QString("BOMBER"),
                            QString("CANNONBOAT"), QString("CRUISER"),
                            QString("DESTROYER"), QString("DUSTER"),
                            QString("FIGHTER"), QString("FLAK"), QString("FLARE"), QString("FRIGATE"),
                            QString("GUNBOAT"),
                            QString("HEAVY_HOVERCRAFT"), QString("HEAVY_TANK"), QString("HOELLIUM"), QString("HOVERCRAFT"), QString("HOVERFLAK"),
                            QString("INFANTRY"),
                            QString("K_HELI"),
                            QString("LANDER"),  QString("LIGHT_TANK"),
                            QString("MECH"), QString("MEGATANK"), QString("MISSILE"), QString("MOTORBIKE"),
                            QString("NEOTANK"),
                            QString("PIPERUNNER"),
                            QString("RECON"), QString("ROCKETTHROWER"),
                            QString("SNIPER"), QString("STEALTHBOMBER"), QString("SUBMARINE"),
                            QString("T_HELI"), QString("TORPEDOBOAT"), QString("TRANSPORTPLANE"),
                            QString("WATERMINE"), QString("WATERPLANE"),
                            QString("ZCOUNIT_AT_CYCLE"), QString("ZCOUNIT_AUTO_TANK"), QString("ZCOUNIT_CHAPERON"), QString("ZCOUNIT_COMMANDO"),
                            QString("ZCOUNIT_CRYSTAL_TANK"), QString("ZCOUNIT_HOT_TANK"), QString("ZCOUNIT_INTEL_TRUCK"), QString("ZCOUNIT_IRON_SHIELD_GENERATOR"),
                            QString("ZCOUNIT_CRYSTAL_KIROV"), QString("ZCOUNIT_LOGIC_TRUCK"), QString("ZCOUNIT_MISSILE_SUB"), QString("ZCOUNIT_NEOSPIDER_TANK"),
                            QString("ZCOUNIT_PARTISAN"), QString("ZCOUNIT_RANGER"), QString("ZCOUNIT_REPAIR_TANK"), QString("ZCOUNIT_ROYAL_GUARD"),
                            QString("ZCOUNIT_SIEGE_CANNON"), QString("ZCOUNIT_SMUGGLER"), QString("ZCOUNIT_TANK_HUNTER")
                             });
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
        auto fileData = Filesupport::readList(file + Filesupport::LIST_FILENAME_ENDING, FILEPATH);
        data = fileData.items;
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

void BuildListDialog::showDeleteBannlist()
{
    if (QFile::exists(FILEPATH + m_PredefinedLists->getCurrentItemText() + Filesupport::LIST_FILENAME_ENDING))
    {
        QString file = FILEPATH + m_PredefinedLists->getCurrentItemText() + Filesupport::LIST_FILENAME_ENDING;
        spDialogMessageBox pDialogOverwrite = spDialogMessageBox::create(tr("Do you want to delete the build bannlist: ") + file + "?", true);
        connect(pDialogOverwrite.get(), &DialogMessageBox::sigOk, this, [this, file]
        {
            emit sigDeleteBannlist(file);
        }, Qt::QueuedConnection);
        addChild(pDialogOverwrite);
    }
}

void BuildListDialog::deleteBannlist(const QString & file)
{
    QFile::remove(file);
    updatePredefinedList();
}

QStringList BuildListDialog::getNameList()
{
    QStringList items = {tr("Commander Wars"),
                              tr("Advance Wars 4"),
                              tr("Advance Wars DS"),
                              tr("Advance Wars 2"),
                              tr("Advance Wars")};
    QStringList filters;
    filters << QString("*") + Filesupport::LIST_FILENAME_ENDING;
    QDirIterator dirIter(FILEPATH, filters, QDir::Files, QDirIterator::IteratorFlag::NoIteratorFlags);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().canonicalFilePath();
        auto data = Filesupport::readList(file);
        items.append(data.name);
    }
    return items;
}

void BuildListDialog::saveBannlist(QString filename)
{
    if (QFile::exists(FILEPATH + filename + Filesupport::LIST_FILENAME_ENDING))
    {
        spDialogMessageBox pDialogOverwrite = spDialogMessageBox::create(tr("Do you want to overwrite the build bannlist: ") + FILEPATH + filename + Filesupport::LIST_FILENAME_ENDING + "?", true);
        connect(pDialogOverwrite.get(), &DialogMessageBox::sigOk, this, [this, filename]
        {
            emit sigDoSaveBannlist(filename);
        }, Qt::QueuedConnection);
        addChild(pDialogOverwrite);
    }
    else
    {
        doSaveBannlist(filename);
    }
}

void BuildListDialog::doSaveBannlist(QString filename)
{
    Filesupport::storeList(filename, m_CurrentBuildList, FILEPATH);
    updatePredefinedList();
    spDialogMessageBox pMessageBox = spDialogMessageBox::create(tr("Do you want to make the saved build list the default ruleset?"), true, tr("Yes"), tr("No"));
    addChild(pMessageBox);
    connect(pMessageBox.get(),  &DialogMessageBox::sigOk, this, [=]()
    {
        Settings::setDefaultBannlist(FILEPATH + filename + Filesupport::LIST_FILENAME_ENDING);
    }, Qt::QueuedConnection);
    updatePredefinedList();
}

void BuildListDialog::updatePredefinedList()
{
    if (m_PredefinedLists.get() != nullptr)
    {
        m_PredefinedLists->detach();
    }
    auto items = getNameList();
    m_PredefinedLists = spDropDownmenu::create(300, items);

    m_PredefinedLists->setPosition(Settings::getWidth() / 2 - m_PredefinedLists->getScaledWidth() - 10,
                                   Settings::getHeight() - 75 - m_ToggleAll->getScaledHeight());
    m_pSpriteBox->addChild(m_PredefinedLists);
    connect(m_PredefinedLists.get(), &DropDownmenu::sigItemChanged, this, &BuildListDialog::setBuildlist, Qt::QueuedConnection);
}
