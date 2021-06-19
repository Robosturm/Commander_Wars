#include "ingamescriptsupport/events/scripteventmodifyterrain.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "objects/base/dropdownmenusprite.h"
#include "objects/base/spinbox.h"
#include "objects/base/label.h"

#include "game/terrain.h"

#include "resource_management/fontmanager.h"
#include "resource_management/terrainmanager.h"

ScriptEventModifyTerrain::ScriptEventModifyTerrain()
    : ScriptEvent (ScriptEvent::EventType::modifyTerrain)
{

}

void ScriptEventModifyTerrain::readEvent(QTextStream& rStream, QString line)
{
    line = line.simplified();
    QStringList items = line.replace("map.replaceTerrain(\"", "")
                            .replace(", ", ",")
                            .replace("\",", ",")
                            .replace("); // " , ",").split(",");
    if (items.size() >= 3)
    {
        m_newTerrainID = items[0];
        m_x = items[1].toInt();
        m_y = items[2].toInt();
    }
}

void ScriptEventModifyTerrain::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.replaceTerrain(\""
            << m_newTerrainID << "\", "
            << QString::number(m_x) << ", "
            << QString::number(m_y) << ", false, true); // "
            << QString::number(getVersion()) << " " << EventModifyTerrain << "\n";
}


void ScriptEventModifyTerrain::showEditEvent(spScriptEditor pScriptEditor)
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
    spinBox->setTooltipText(tr("X Location at which the terrain gets modified."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(m_x);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventModifyTerrain::setX, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Y: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setTooltipText(tr("Y Location at which the terrain gets modified."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(m_y);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventModifyTerrain::setY, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Terrain: "));
    pText->setPosition(30, 150);
    pBox->addItem(pText);
    QVector<QString> items;
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    QStringList sortedTerrains = pTerrainManager->getTerrainsSorted();
    qint32 currentItem = sortedTerrains.indexOf(m_newTerrainID);
    if (currentItem < 0)
    {
        currentItem = 0;
    }
    for (const auto& curTerrainId : sortedTerrains)
    {
        items.append(curTerrainId);
    }
    auto creator = [=](QString id)
    {
        spTerrain pTerrain = Terrain::createTerrain(id, -1, -1, "");
        pTerrain->loadSprites();
        return pTerrain;
    };
    spDropDownmenuSprite pMenu = spDropDownmenuSprite::create(105, items, creator, 30);
    pMenu->setTooltipText(tr("New Terrain which gets spawned. Note: all units on the terrain get deleted."));
    pMenu->setPosition(width, 150);
    pMenu->setCurrentItem(currentItem);
    pBox->addItem(pMenu);
    connect(pMenu.get(), &DropDownmenuSprite::sigItemChanged, this, [=](qint32)
    {
        m_newTerrainID = pMenu->getCurrentItemText();
    });

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}

qint32 ScriptEventModifyTerrain::getX() const
{
    return m_x;
}

void ScriptEventModifyTerrain::setX(const qint32 &x)
{
    m_x = x;
}

qint32 ScriptEventModifyTerrain::getY() const
{
    return m_y;
}

void ScriptEventModifyTerrain::setY(const qint32 &y)
{
    m_y = y;
}

QString ScriptEventModifyTerrain::getNewTerrainID() const
{
    return m_newTerrainID;
}

void ScriptEventModifyTerrain::setNewTerrainID(const QString &newTerrainID)
{
    m_newTerrainID = newTerrainID;
}
