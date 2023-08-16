#include "ingamescriptsupport/events/scripteventchangebuildlist.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"
#include "resource_management/unitspritemanager.h"

#include "objects/base/spinbox.h"
#include "objects/base/dropdownmenusprite.h"
#include "objects/base/checkbox.h"
#include "objects/base/label.h"

ScriptEventChangeBuildlist::ScriptEventChangeBuildlist(GameMap* pMap)
    : ScriptEvent(pMap, EventType::changeBuildlist),
      m_pMap(pMap)
{
    m_dropDownPlayer = MemoryManagement::create<Player>(pMap);
    m_dropDownPlayer->init();
}

QString ScriptEventChangeBuildlist::getUnitID() const
{
    return unitID;
}

void ScriptEventChangeBuildlist::setUnitID(const QString &value)
{
    unitID = value;
}

bool ScriptEventChangeBuildlist::getRemove() const
{
    return remove;
}

void ScriptEventChangeBuildlist::setRemove(bool value)
{
    remove = value;
}


void ScriptEventChangeBuildlist::readEvent(QTextStream& rStream, QString line)
{
    line = line.simplified();
    QStringList items = line.replace("map.getPlayer(", "")
                            .replace(").changeBuildlist(\"", ",")
                        .replace("\", ", ",")
                            .replace("); // ", ",").split(",");
    if (items.size() >= 3)
    {
        player = items[0].toInt();
        unitID = items[1];
        remove = QVariant(items[2]).toBool();
    }
}

void ScriptEventChangeBuildlist::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.getPlayer(" << QString::number(player) << ").changeBuildlist(\"" + unitID + "\", " + QVariant(remove).toString() + "); // "
            << QString::number(getVersion()) << " " << EventChangeBuildlist << "\n";
}

qint32 ScriptEventChangeBuildlist::getPlayer() const
{
    return player;
}

void ScriptEventChangeBuildlist::setPlayer(const qint32 &value)
{
    player = value;
}

void ScriptEventChangeBuildlist::showEditEvent(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = MemoryManagement::create<GenericBox>();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    spLabel pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = MemoryManagement::create<SpinBox>(300, 1, 9999);
    spinBox->setTooltipText(tr("Player who's buildlist will be modified."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this,
            [this](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Unit ID: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    QStringList items;
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    qint32 currentItem = sortedUnits.indexOf(unitID);
    if (currentItem < 0)
    {
        currentItem = 0;
    }
    for (const auto& curUnitId : sortedUnits)
    {
        items.append(curUnitId);
    }
    Player* pPlayer = m_dropDownPlayer.get();
    auto unitCreator = [this, pPlayer](QString id)
    {
        spUnit pSprite = MemoryManagement::create<Unit>(id, pPlayer, false, m_pMap);
        return pSprite;
    };
    spDropDownmenuSprite pMenu = MemoryManagement::create<DropDownmenuSprite>(105, items, unitCreator, 30);
    pMenu->setTooltipText(tr("The unit that will be changed in the build list of the player."));
    pMenu->setPosition(width, 70);
    pMenu->setCurrentItem(currentItem);
    pBox->addItem(pMenu);
    DropDownmenuSprite* pPtrMenu = pMenu.get();
    connect(pMenu.get(), &DropDownmenuSprite::sigItemChanged, this, [this, pPtrMenu](qint32)
    {
        unitID = pPtrMenu->getCurrentItemText();
    });

    pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Remove: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);

    spCheckbox checkBox = MemoryManagement::create<Checkbox>();
    checkBox->setTooltipText(tr("If the checked the unit will be forbidden to be build, else it gets allowed to be build."));
    checkBox->setPosition(width, 110);
    checkBox->setChecked(remove);
    connect(checkBox.get(), &Checkbox::checkChanged, this,
            [this](bool value)
    {
        setRemove(value);
    });
    pBox->addItem(checkBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
