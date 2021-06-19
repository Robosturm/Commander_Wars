#include "ingamescriptsupport/conditions/scriptconditionbuildingdestroyed.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionBuildingDestroyed::ScriptConditionBuildingDestroyed()
    : ScriptCondition(ConditionType::buildingDestroyed)
{

}

void ScriptConditionBuildingDestroyed::readCondition(QTextStream& rStream, QString line)
{
    Console::print("Reading ConditionBuildingDestroyed", Console::eDEBUG);
    line = line.simplified();
    QStringList items = line.replace("if ((map.getTerrain(", "")
                            .replace(", ", ",")
                            .replace(").getBuilding() === ", ",")
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

void ScriptConditionBuildingDestroyed::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionBuildingDestroyed::writeCondition(QTextStream& rStream)
{
    Console::print("Writing ConditionBuildingDestroyed", Console::eDEBUG);
    rStream << "        if ((map.getTerrain(" << QString::number(m_x) << ", " << QString::number(m_y) << ").getBuilding() === null || "
            << "map.getTerrain(" << QString::number(m_x) << ", " << QString::number(m_y) << ").getBuilding().getHp() <= 0) && " << m_executed << ".readDataBool() === false) {"
            << "// " << QString::number(getVersion()) << " " << ConditionBuildingDestroyed << "\n";
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

void ScriptConditionBuildingDestroyed::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

void ScriptConditionBuildingDestroyed::showEditCondition(spScriptEditor pScriptEditor)
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
    spinBox->setTooltipText(tr("X Location of the Building that has to be destroyed."));
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
    spinBox->setTooltipText(tr("Y Location of the Building that has to be destroyed."));
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

qint32 ScriptConditionBuildingDestroyed::getX() const
{
    return m_x;
}

void ScriptConditionBuildingDestroyed::setX(const qint32 &x)
{
    m_x = x;
}

qint32 ScriptConditionBuildingDestroyed::getY() const
{
    return m_y;
}

void ScriptConditionBuildingDestroyed::setY(const qint32 &y)
{
    m_y = y;
}
