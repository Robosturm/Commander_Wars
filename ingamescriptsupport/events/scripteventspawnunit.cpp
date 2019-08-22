#include "scripteventspawnunit.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"
#include "resource_management/unitspritemanager.h"

#include "coreengine/mainapp.h"

#include "objects/spinbox.h"

#include "objects/dropdownmenu.h"

ScriptEventSpawnUnit::ScriptEventSpawnUnit()
    : ScriptEvent (ScriptEvent::EventType::spawnUnit)
{

}

void ScriptEventSpawnUnit::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
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
    spGenericBox pBox = new GenericBox();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;
    oxygine::spTextField pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("X: ").toStdString().c_str());
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = new SpinBox(150, 0, 9999);
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(x);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventSpawnUnit::setX, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Y: ").toStdString().c_str());
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 9999);
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(y);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventSpawnUnit::setY, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: ").toStdString().c_str());
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 1, 9999);
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Unit ID: ").toStdString().c_str());
    pText->setPosition(30, 150);
    pBox->addItem(pText);
    QVector<QString> items;
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 currentItem = 0;
    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        QString id = pUnitSpriteManager->getUnitID(i);
        items.append(id);
        if (unitID == id)
        {
            currentItem = i;
        }
    }
    spDropDownmenu pMenu = new DropDownmenu(300, items);
    pMenu->setPosition(width, 150);
    pMenu->setCurrentItem(currentItem);
    pBox->addItem(pMenu);
    connect(pMenu.get(), &DropDownmenu::sigItemChanged, [=](qint32)
    {
        unitID = pMenu->getCurrentItemText();
    });

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Spawn Radius: ").toStdString().c_str());
    pText->setPosition(30, 190);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 9999);
    spinBox->setPosition(width, 190);
    spinBox->setCurrentValue(radius);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventSpawnUnit::setRadius, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
