#include "ingamescriptsupport/conditions/scriptconditionbuildingcaptured.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionBuildingCaptured::ScriptConditionBuildingCaptured()
    : ScriptCondition (ConditionType::buildingCaptured)
{

}

qint32 ScriptConditionBuildingCaptured::getX() const
{
    return m_x;
}

void ScriptConditionBuildingCaptured::setX(const qint32 &x)
{
    m_x = x;
}

qint32 ScriptConditionBuildingCaptured::getY() const
{
    return m_y;
}

void ScriptConditionBuildingCaptured::setY(const qint32 &y)
{
    m_y = y;
}

qint32 ScriptConditionBuildingCaptured::getPlayer() const
{
    return m_player;
}

void ScriptConditionBuildingCaptured::setPlayer(const qint32 &player)
{
    m_player = player;
}

void ScriptConditionBuildingCaptured::readCondition(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace("if (map.getTerrain(", "")
                            .replace(", ", ",")
                            .replace(").getBuilding().getOwner() === null && map.getTerrain(", ",")
                            .replace(").getBuilding().getOwner().getPlayerID() === ", ",")
                            .replace(" && ", ",").split(",");
    if (items.size() >= 6)
    {
        m_x = items[0].toInt();
        m_y = items[1].toInt();
        m_player = items[4].toInt();
    }
    else if (items.size() >= 4)
    {
        m_x = items[0].toInt();
        m_y = items[1].toInt();
        m_player = items[2].toInt();
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(rStream, ConditionBuildingCaptured))
        {
            break;
        }
        spScriptEvent event = ScriptEvent::createReadEvent(rStream);
        if (event.get() != nullptr)
        {
            events.append(event);
        }
    }
}

void ScriptConditionBuildingCaptured::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionBuildingCaptured::writeCondition(QTextStream& rStream)
{
    rStream << "        if (map.getTerrain(" << QString::number(m_x) << ", " << QString::number(m_y) << ").getBuilding().getOwner() !== null && map.getTerrain(" << QString::number(m_x) << ", " << QString::number(m_y) << ").getBuilding().getOwner().getPlayerID() === "
            << QString::number(m_player) << " && " << m_executed << ".readDataBool() === false) {"
            << "// " << QString::number(getVersion()) << " "  << ConditionBuildingCaptured << "\n";
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
    rStream << "        } // " + ConditionBuildingCaptured + " End\n";
}

void ScriptConditionBuildingCaptured::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

void ScriptConditionBuildingCaptured::showEditCondition(spScriptEditor pScriptEditor)
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
    spinBox->setTooltipText(tr("X Location of the Building that has to be captured."));
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
    spinBox->setTooltipText(tr("Y Location of the Building that has to be captured."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(m_y);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 99999);
    spinBox->setTooltipText(tr("Player that has to capture the building."));
    spinBox->setPosition(width, 110);
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
