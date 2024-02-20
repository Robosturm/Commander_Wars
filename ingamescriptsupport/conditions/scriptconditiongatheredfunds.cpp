#include "ingamescriptsupport/conditions/scriptconditiongatheredfunds.h"
#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "coreengine/gameconsole.h"

#include "objects/base/label.h"
#include "objects/base/spinbox.h"

#include "resource_management/fontmanager.h"

ScriptConditionGatheredFunds::ScriptConditionGatheredFunds(GameMap* pMap)
    : ScriptCondition(pMap, ConditionType::gatheredFunds)
{
}


void ScriptConditionGatheredFunds::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionGatheredFunds::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

void ScriptConditionGatheredFunds::writeCondition(QTextStream& rStream)
{
    CONSOLE_PRINT("Writing GatheredFunds", GameConsole::eDEBUG);
    QString compare = m_Compare;
    compare = compare.replace("&gt;", ">").replace("&lt;", "<");
    rStream << "        if (map.getPlayer(" << QString::number(m_player) << ").getFunds() " << compare << " " << QString::number(m_funds) << " && " << m_executed << ".readDataBool() === false) {"
            << "// " << QString::number(getVersion()) << " "  << ConditionGatheredFunds << "\n";
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
    rStream << "        } // " + QString(ConditionGatheredFunds) + " End\n";
}

void ScriptConditionGatheredFunds::readCondition(QTextStream& rStream, QString line)
{
    CONSOLE_PRINT("Reading GatheredFunds", GameConsole::eDEBUG);
    line = line.simplified();
    QStringList items = line.replace("if (map.getPlayer(", "")
                            .replace(".getFunds() ", ",")
                            .replace(" ", ",")
                            .replace("\" && ", ",")
                            .split(",");
    if (items.size() >= 3)
    {
        m_player = items[0].toInt();
        m_Compare = items[1].replace(">", "&gt;").replace("<", "&lt;");
        m_funds = items[2].toInt();
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(m_pMap, rStream, ConditionGatheredFunds, line))
        {
            break;
        }
        spScriptEvent event = ScriptEvent::createReadEvent(m_pMap, rStream, line);
        if (event.get() != nullptr)
        {
            events.append(event);
        }
    }
}

void ScriptConditionGatheredFunds::showEditCondition(spScriptEditor pScriptEditor)
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
    spSpinBox spinBox = MemoryManagement::create<SpinBox>(300, 0, 99999);
    spinBox->setTooltipText(tr("Player of which a co should be checked."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(m_player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, [this](qreal value)
    {
        m_player = static_cast<qint32>(value) - 1;
    });
    pBox->addItem(spinBox);

    pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Compare: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    QStringList items = {"===", "!==", "&gt;=", "&lt;="};
    spDropDownmenu dropDown = MemoryManagement::create<DropDownmenu>(150, items);
    dropDown->setTooltipText(tr("The way how the funds gets compared with the constant.\n=== equal\n!== unequal\n>= greater or equal\n<= smaller or equal"));
    dropDown->setPosition(width, 70);
    dropDown->setCurrentItemText(m_Compare);
    DropDownmenu* pDropDown = dropDown.get();
    connect(dropDown.get(), &DropDownmenu::sigItemChanged, this, [this, pDropDown](qint32)
    {
        m_Compare = pDropDown->getCurrentItemText();
    });
    pBox->addItem(dropDown);

    pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Funds: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = MemoryManagement::create<SpinBox>(300, 0, 99999);
    spinBox->setTooltipText(tr("Funds the player needs to gather."));
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(m_funds);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, [this](qreal value)
    {
        m_funds = static_cast<qint32>(value);
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}
