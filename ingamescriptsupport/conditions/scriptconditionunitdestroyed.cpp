#include "ingamescriptsupport/conditions/scriptconditionunitdestroyed.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionUnitDestroyed::ScriptConditionUnitDestroyed()
    : ScriptCondition (ScriptCondition::ConditionType::unitDestroyed)
{

}


void ScriptConditionUnitDestroyed::readCondition(QTextStream& rStream, QString line)
{
    line = line.simplified();
    QStringList list = line.split("//")[1].split(" ");
    if (list.size() > 2)
    {
        m_x = list[1].toInt();
        m_y = list[2].toInt();
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(rStream, ConditionUnitDestroyed, line))
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

void ScriptConditionUnitDestroyed::writePreCondition(QTextStream& rStream)
{
    Console::print("Reading ConditionUnitDestroyed", Console::eDEBUG);
    m_executed = ScriptData::getVariableName();
    m_unitID = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    rStream << "        var " << m_unitID << " = " << ScriptData::variables << ".createVariable(\"" << m_unitID << "\");\n";
    rStream << "        var " << m_unitID << "Value = " << m_unitID << ".readDataInt32();\n";
    rStream << "        if (" << m_unitID << "Value === 0){" << m_unitID << ".writeDataInt32(map.getTerrain(" << m_x << ", " << m_y << ").getUnit().getUniqueID());}\n";
    rStream << "        " << m_unitID << "Value = " << m_unitID << ".readDataInt32();\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionUnitDestroyed::writeCondition(QTextStream& rStream)
{
    Console::print("Writing ConditionUnitDestroyed", Console::eDEBUG);
    rStream << "        if (map.getUnit(" << m_unitID << "Value) === null && " << m_executed << ".readDataBool() === false) {"
            << "// " << m_x << " " << m_y << " " << QString::number(getVersion()) << " " << ConditionUnitDestroyed << "\n";
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
    rStream << "        } // " + ConditionUnitDestroyed + " End\n";
}

void ScriptConditionUnitDestroyed::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

qint32 ScriptConditionUnitDestroyed::getX() const
{
    return m_x;
}

void ScriptConditionUnitDestroyed::setX(const qint32 &x)
{
    m_x = x;
}

qint32 ScriptConditionUnitDestroyed::getY() const
{
    return m_y;
}

void ScriptConditionUnitDestroyed::setY(const qint32 &y)
{
    m_y = y;
}

void ScriptConditionUnitDestroyed::showEditCondition(spScriptEditor pScriptEditor)
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
    spinBox->setTooltipText(tr("X Location of the unit at the start of the game that needs to be destroyed. The unit is allowed to move."));
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
    spinBox->setTooltipText(tr("Y Location of the unit at the start of the game that needs to be destroyed. The unit is allowed to move."));
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
