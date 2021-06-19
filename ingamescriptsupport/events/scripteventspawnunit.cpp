#include "ingamescriptsupport/events/scripteventspawnunit.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"
#include "resource_management/unitspritemanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenusprite.h"
#include "objects/base/label.h"

#include "game/unit.h"
#include "game/player.h"

ScriptEventSpawnUnit::ScriptEventSpawnUnit()
    : ScriptEvent (ScriptEvent::EventType::spawnUnit)
{

}

void ScriptEventSpawnUnit::readEvent(QTextStream& rStream, QString line)
{
    line = line.simplified();
    QStringList items = line.replace("map.spawnUnit(", "")
                            .replace(", ", ",")
                            .replace(",\"", ",")
                            .replace("\",map.getPlayer(", ",")
                            .replace("),", ",")
                            .replace("); // " , ",").split(",");
    if (items.size() >= 5)
    {
        x = items[0].toInt();
        y = items[1].toInt();
        unitID = items[2];
        player = items[3].toInt();
        radius = items[4].toInt();
    }
}

void ScriptEventSpawnUnit::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.spawnUnit(" << QString::number(x) << ", "
            << QString::number(y) << ", \"" << unitID << "\", map.getPlayer("
            << QString::number(player) << "), " << QString::number(radius) << "); // "
            << QString::number(getVersion()) << " " << EventSpawnUnit << "\n";
}

qint32 ScriptEventSpawnUnit::getX() const
{
    return x;
}

void ScriptEventSpawnUnit::setX(const qint32 &value)
{
    x = value;
}

qint32 ScriptEventSpawnUnit::getY() const
{
    return y;
}

void ScriptEventSpawnUnit::setY(const qint32 &value)
{
    y = value;
}

QString ScriptEventSpawnUnit::getUnitID() const
{
    return unitID;
}

void ScriptEventSpawnUnit::setUnitID(const QString &value)
{
    unitID = value;
}

qint32 ScriptEventSpawnUnit::getPlayer() const
{
    return player;
}

void ScriptEventSpawnUnit::setPlayer(const qint32 &value)
{
    player = value;
}

qint32 ScriptEventSpawnUnit::getRadius() const
{
    return radius;
}

void ScriptEventSpawnUnit::setRadius(const qint32 &value)
{
    radius = value;
}

void ScriptEventSpawnUnit::showEditEvent(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;
    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("X: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setTooltipText(tr("X Location at which the unit gets spawned."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(x);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventSpawnUnit::setX, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Y: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setTooltipText(tr("Y Location at which the unit gets spawned."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(y);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventSpawnUnit::setY, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("Player for which the unit gets spawned."));
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Unit: "));
    pText->setPosition(30, 150);
    pBox->addItem(pText);
    QVector<QString> items;
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
    auto creator = [=](QString id)
    {
        spPlayer pPlayer = spPlayer::create();
        pPlayer->init();
        spUnit pSprite = spUnit::create(id, pPlayer.get(), false);
        pSprite->setOwner(nullptr);
        return pSprite;
    };
    spDropDownmenuSprite pMenu = spDropDownmenuSprite::create(105, items, creator, 30);
    pMenu->setTooltipText(tr("Unit which gets spawned."));
    pMenu->setPosition(width, 150);
    pMenu->setCurrentItem(currentItem);
    pBox->addItem(pMenu);
    connect(pMenu.get(), &DropDownmenuSprite::sigItemChanged, this, [=](qint32)
    {
        unitID = pMenu->getCurrentItemText();
    });

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Spawn Radius: "));
    pText->setPosition(30, 190);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setTooltipText(tr("Radius around the given location at which the unit gets tried to be spawned, if either the field is blocked or the unit can't move over the given field."));
    spinBox->setPosition(width, 190);
    spinBox->setCurrentValue(radius);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventSpawnUnit::setRadius, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
