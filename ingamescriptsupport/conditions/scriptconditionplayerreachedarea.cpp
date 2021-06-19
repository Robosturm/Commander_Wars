#include "ingamescriptsupport/conditions/scriptconditionplayerreachedarea.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"

#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "coreengine/console.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionPlayerReachedArea::ScriptConditionPlayerReachedArea()
    : ScriptCondition (ConditionType::playerReachedArea)
{

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

void ScriptConditionPlayerReachedArea::readCondition(QTextStream& rStream, QString line)
{
    Console::print("Reading ConditionPlayerReachedArea", Console::eDEBUG);
    line = line.simplified();
    QStringList list = line.split("//");
    if (list.size() >= 2)
    {
        m_Player.clear();
        if (list[1].startsWith(" 0"))
        {
            QStringList items0 = list[0].replace("if (map.isPlayerUnitInArea(Qt.rect(", "")
                                 .replace(", ", ",").replace("),", ",").replace(") && ", ",").split(",");
            if (items0.size() >= 4)
            {
                m_x = items0[0].toInt();
                m_y = items0[1].toInt();
                m_width = items0[2].toInt();
                m_heigth = items0[3].toInt();
                m_Player.append(items0[4].toInt());
            }
        }
        else
        {
            QStringList items0 = list[0].replace("if (map.isPlayersUnitInArea(Qt.rect(", "")
                                 .replace("), [", ",").replace(", ", ",").replace("]) && ", ",").split(",");
            if (items0.size() >= 4)
            {
                m_x = items0[0].toInt();
                m_y = items0[1].toInt();
                m_width = items0[2].toInt();
                m_heigth = items0[3].toInt();
                for (qint32 i = 4; i < items0.size() - 1; i++)
                {
                    m_Player.append(items0[i].toInt());
                }
            }
        }
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(rStream, ConditionPlayerReachedArea, line))
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

void ScriptConditionPlayerReachedArea::writePreCondition(QTextStream& rStream)
{
    Console::print("Writing ConditionPlayerReachedArea", Console::eDEBUG);
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionPlayerReachedArea::writeCondition(QTextStream& rStream)
{
    rStream << "        if (map.isPlayersUnitInArea(Qt.rect(" << QString::number(m_x) << ", " << QString::number(m_y) << ", "
            << QString::number(m_width) << ", " << QString::number(m_heigth) << "), [";
    for (qint32 i = 0; i < m_Player.size(); i++)
    {
        rStream << QString::number(m_Player[i]);
        if (i < m_Player.size() - 1)
        {
            rStream << ", ";
        }
    }
    rStream << "]) && " << m_executed << ".readDataBool() === false) {"
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
    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    qint32 y = 30;
    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Target X: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 0, 99999);
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

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Y: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 99999);
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

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Width: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 99999);
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

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Heigth: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 99999);
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

    spLabel pTextInfo = spLabel::create(width - 10);
    pTextInfo->setStyle(style);
    pTextInfo->setHtmlText(getPlayerInfo());
    pTextInfo->setPosition(30, y);
    pBox->addItem(pTextInfo);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Add Player: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 99999);
    spinBox->setTooltipText(tr("Player to add to the Player reached Area list."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(1);
    pBox->addItem(spinBox);
    y += 40;

    oxygine::spButton pButton = ObjectManager::createButton(tr("Add Player"), 200);
    pButton->setPosition(30, y);
    pButton->addClickListener([=](oxygine::Event*)
    {
        qint32 player = spinBox->getCurrentValue() - 1;
        if (!m_Player.contains(player))
        {
            m_Player.append(player);
            pTextInfo->setHtmlText(getPlayerInfo());
        }
    });
    pBox->addItem(pButton);
    pButton = ObjectManager::createButton(tr("Remove last Player"), 200);
    pButton->setPosition(270, y);
    pButton->addClickListener([=](oxygine::Event*)
    {
        if (m_Player.size() > 1)
        {
            m_Player.removeLast();
            pTextInfo->setHtmlText(getPlayerInfo());
        }
    });
    pBox->addItem(pButton);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}

QString ScriptConditionPlayerReachedArea::getPlayerInfo()
{
    QString item = tr("Players: ");
    for (qint32 i = 0; i < m_Player.size(); i++)
    {
        item += QString::number(m_Player[i] + 1);
        if (i < m_Player.size() - 1)
        {
            item += ", ";
        }
    }
    return item;
}
