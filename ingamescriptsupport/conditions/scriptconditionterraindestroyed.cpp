#include "ingamescriptsupport/conditions/scriptconditionterraindestroyed.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionTerrainDestroyed::ScriptConditionTerrainDestroyed()
    : ScriptCondition(ConditionType::terrainDestroyed)
{

}

void ScriptConditionTerrainDestroyed::readCondition(QTextStream& rStream, QString line)
{
    Console::print("Reading ConditionTerrainDestroyed", Console::eDEBUG);
    line = line.simplified();
    QStringList items = line.replace("if (map.getTerrain(", "")
                            .replace(", ", ",")
                            .replace(").getHp() <= 0", ",")
                            .replace(" && ", "").split(",");
    if (items.size() >= 2)
    {
        m_x = items[0].toInt();
        m_y = items[1].toInt();
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(rStream, ConditionBuildingDestroyed, line))
        {
            break;
        }
        spScriptEvent event = ScriptEvent::createReadEvent(rStream, line);
        if (event.get() != nullptr)
        {
            events.append(event);
        }
    }
}

void ScriptConditionTerrainDestroyed::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionTerrainDestroyed::writeCondition(QTextStream& rStream)
{
    Console::print("Writing ConditionTerrainDestroyed", Console::eDEBUG);
    rStream << "        if (" << "map.getTerrain(" << QString::number(m_x) << ", " << QString::number(m_y) << ").getHp() <= 0 && " << m_executed << ".readDataBool() === false) {"
            << "// " << QString::number(getVersion()) << " " << ConditionTerrainDestroyed << "\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writeCondition(rStream);
    }
    else if (pParent != nullptr)
    {
        pParent->writePostCondition(rStream);
        for (qint32 i = 0; i < events.size(); i++)
        {
            events[i]->writeEvent(rStream);
        }
        rStream << "            " << m_executed << ".writeDataBool(true);\n";
    }
    else
    {
        for (qint32 i = 0; i < events.size(); i++)
        {
            events[i]->writeEvent(rStream);
        }
        rStream << "            " << m_executed << ".writeDataBool(true);\n";
    }
    rStream << "        } // " + ConditionBuildingDestroyed + " End\n";
}

void ScriptConditionTerrainDestroyed::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

void ScriptConditionTerrainDestroyed::showEditCondition(spScriptEditor pScriptEditor)
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
    spSpinBox spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setTooltipText(tr("X Location of the Terrain that has to be destroyed."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(m_x);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setX(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Y: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setTooltipText(tr("Y Location of the Terrain that has to be destroyed."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(m_y);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}

qint32 ScriptConditionTerrainDestroyed::getX() const
{
    return m_x;
}

void ScriptConditionTerrainDestroyed::setX(const qint32 &x)
{
    m_x = x;
}

qint32 ScriptConditionTerrainDestroyed::getY() const
{
    return m_y;
}

void ScriptConditionTerrainDestroyed::setY(const qint32 &y)
{
    m_y = y;
}
