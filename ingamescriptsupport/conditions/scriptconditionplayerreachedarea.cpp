#include "scriptconditionplayerreachedarea.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"

#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "objects/spinbox.h"

ScriptConditionPlayerReachedArea::ScriptConditionPlayerReachedArea()
    : ScriptCondition (ConditionType::playerReachedArea)
{

}

qint32 ScriptConditionPlayerReachedArea::getPlayer() const
{
    return m_Player;
}

void ScriptConditionPlayerReachedArea::setPlayer(const qint32 &player)
{
    m_Player = player;
}

qint32 ScriptConditionPlayerReachedArea::getX() const
{
    return m_x;
}

void ScriptConditionPlayerReachedArea::setX(const qint32 &x)
{
    m_x = x;
}

qint32 ScriptConditionPlayerReachedArea::getY() const
{
    return m_y;
}

void ScriptConditionPlayerReachedArea::setY(const qint32 &y)
{
    m_y = y;
}

qint32 ScriptConditionPlayerReachedArea::getWidth() const
{
    return m_width;
}

void ScriptConditionPlayerReachedArea::setWidth(const qint32 &width)
{
    m_width = width;
}

qint32 ScriptConditionPlayerReachedArea::getHeigth() const
{
    return m_heigth;
}

void ScriptConditionPlayerReachedArea::setHeigth(const qint32 &heigth)
{
    m_heigth = heigth;
}

void ScriptConditionPlayerReachedArea::readCondition(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();

    QStringList list = line.split("//");
    if (list.size() >= 2)
    {
        QStringList items0 = list[0].replace("if (map.isPlayerUnitInArea(Qt.rect(", "")
                             .replace(", ", ",").replace("),", ",").replace(") && ", ",").split(",");
        if (items0.size() >= 4)
        {
            m_x = items0[0].toInt();
            m_y = items0[1].toInt();
            m_width = items0[2].toInt();
            m_heigth = items0[3].toInt();
            m_Player = items0[4].toInt();
        }
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(rStream, ConditionPlayerReachedArea))
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

void ScriptConditionPlayerReachedArea::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = variables.createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionPlayerReachedArea::writeCondition(QTextStream& rStream)
{
    rStream << "        if (map.isPlayerUnitInArea(Qt.rect(" << QString::number(m_x) << ", " << QString::number(m_y) << ", "
            << QString::number(m_width) << ", " << QString::number(m_heigth) << "), "
            << QString::number(m_Player) << ") && " << m_executed << ".readDataBool() === false) {"
            << "// " << QString::number(getVersion()) << " " << ConditionPlayerReachedArea << "\n";
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
    rStream << "        } // " + ConditionPlayerReachedArea + " End\n";
}

void ScriptConditionPlayerReachedArea::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}


void ScriptConditionPlayerReachedArea::showEditCondition(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = new GenericBox();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    qint32 y = 30;
    oxygine::spTextField pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Target X: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox spinBox = new SpinBox(150, 0, 99999);
    spinBox->setTooltipText(tr("Target Area X Position which the player needs to reach."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(m_x);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setX(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Y: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setTooltipText(tr("Target Area Y Position which the player needs to reach."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(m_y);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Width: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setTooltipText(tr("Target Area width which the player needs to reach."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(m_width);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setWidth(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Heigth: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setTooltipText(tr("Target Area heigth which the player needs to reach."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(m_heigth);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setHeigth(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setTooltipText(tr("One of the players that needs to reach the area with a unit."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(m_Player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);
    y += 40;

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}
