#include "ingamescriptsupport/conditions/scriptconditionunitsdestroyed.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionUnitsDestroyed::ScriptConditionUnitsDestroyed()
    : ScriptCondition(ConditionType::unitsDestroyed)
{

}

qint32 ScriptConditionUnitsDestroyed::getPlayer() const
{
    return m_player;
}

void ScriptConditionUnitsDestroyed::setPlayer(const qint32 &player)
{
    m_player = player;
}

qint32 ScriptConditionUnitsDestroyed::getCount() const
{
    return m_count;
}

void ScriptConditionUnitsDestroyed::setCount(const qint32 &count)
{
    m_count = count;
}

void ScriptConditionUnitsDestroyed::readCondition(QTextStream& rStream, QString line)
{
    Console::print("Reading ConditionUnitsDestroyed", Console::eDEBUG);
    line = line.simplified();
    QStringList items = line.replace("if (map.getGameRecorder().getDestroyedUnits(", "")
                            .replace(") >= ", ",")
                            .replace(" && ", ",").split(",");
    if (items.size() >= 2)
    {
        m_player = items[0].toInt();
        m_count = items[1].toInt();
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(rStream, ConditionUnitsDestroyed, line))
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

void ScriptConditionUnitsDestroyed::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionUnitsDestroyed::writeCondition(QTextStream& rStream)
{
    Console::print("Writing ConditionUnitsDestroyed", Console::eDEBUG);
    rStream << "        if (map.getGameRecorder().getDestroyedUnits(" << QString::number(m_player) << ") >= " << QString::number(m_count)
            << " && " << m_executed << ".readDataBool() === false) {"
            << "// " << QString::number(getVersion()) << " " << ConditionUnitsDestroyed << "\n";
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
    rStream << "        } // " + ConditionUnitsDestroyed + " End\n";
}

void ScriptConditionUnitsDestroyed::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

void ScriptConditionUnitsDestroyed::showEditCondition(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("Count: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 1, 99999);
    spinBox->setTooltipText(tr("Amount of units that has to be killed."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(m_count);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setCount(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    spinBox->setTooltipText(tr("Player who has to kill the units."));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 99999);
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(m_player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}
