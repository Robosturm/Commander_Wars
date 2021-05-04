#include "ingamescriptsupport/conditions/ScriptConditionUnitReachedArea.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionUnitReachedArea::ScriptConditionUnitReachedArea()
    : ScriptCondition (ConditionType::unitReachedArea)
{
    setObjectName("ScriptConditionUnitReachedArea");
}

qint32 ScriptConditionUnitReachedArea::getUnitX() const
{
    return m_UnitX;
}

void ScriptConditionUnitReachedArea::setUnitX(const qint32 &UnitX)
{
    m_UnitX = UnitX;
}

qint32 ScriptConditionUnitReachedArea::getUnitY() const
{
    return m_UnitY;
}

void ScriptConditionUnitReachedArea::setUnitY(const qint32 &UnitY)
{
    m_UnitY = UnitY;
}

qint32 ScriptConditionUnitReachedArea::getX() const
{
    return m_x;
}

void ScriptConditionUnitReachedArea::setX(const qint32 &x)
{
    m_x = x;
}

qint32 ScriptConditionUnitReachedArea::getY() const
{
    return m_y;
}

void ScriptConditionUnitReachedArea::setY(const qint32 &y)
{
    m_y = y;
}

qint32 ScriptConditionUnitReachedArea::getWidth() const
{
    return m_width;
}

void ScriptConditionUnitReachedArea::setWidth(const qint32 &width)
{
    m_width = width;
}

qint32 ScriptConditionUnitReachedArea::getHeigth() const
{
    return m_heigth;
}

void ScriptConditionUnitReachedArea::setHeigth(const qint32 &heigth)
{
    m_heigth = heigth;
}

void ScriptConditionUnitReachedArea::readCondition(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();

    QStringList list = line.split("//");
    if (list.size() >= 2)
    {
        QStringList items0 = list[0].replace("if (map.isUnitInArea(Qt.rect(", "")
                             .replace(", ", ",").replace("),", ",").split(",");
        QStringList items1 = line.split("//")[1].split(" ");
        if (items0.size() >= 4)
        {
            m_x = items0[0].toInt();
            m_y = items0[1].toInt();
            m_width = items0[2].toInt();
            m_heigth = items0[3].toInt();
        }
        if (items1.size() >= 3)
        {
            m_UnitX = items1[1].toInt();
            m_UnitY = items1[2].toInt();
        }
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(rStream, ConditionUnitReachedArea))
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

void ScriptConditionUnitReachedArea::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    m_unitID = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    rStream << "        var " << m_unitID << " = " << ScriptData::variables << ".createVariable(\"" << m_unitID << "\");\n";
    rStream << "        var " << m_unitID << "Value = " << m_unitID << ".readDataInt32();\n";
    rStream << "        if (" << m_unitID << "Value === 0){" << m_unitID << ".writeDataInt32(map.getTerrain(" << m_UnitX << ", " << m_UnitY << ").getUnit().getUniqueID());}\n";
    rStream << "        " << m_unitID << "Value = " << m_unitID << ".readDataInt32();\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionUnitReachedArea::writeCondition(QTextStream& rStream)
{
    rStream << "        if (map.isUnitInArea(Qt.rect(" << QString::number(m_x) << ", " << QString::number(m_y) << ", "
            << QString::number(m_width) << ", " << QString::number(m_heigth) << "), "
            << m_unitID << "Value) && " << m_executed << ".readDataBool() === false) {"
            << "// " << m_UnitX << " " << m_UnitY << " " << QString::number(getVersion()) << " " << ConditionUnitReachedArea << "\n";
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
    rStream << "        } // " + ConditionUnitReachedArea + " End\n";
}

void ScriptConditionUnitReachedArea::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}


void ScriptConditionUnitReachedArea::showEditCondition(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("Unit X: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setTooltipText(tr("Start X of the unit that should reach the area."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(m_UnitX);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setUnitX(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Unit Y: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setTooltipText(tr("Start Y of the unit that should reach the area."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(m_UnitY);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setUnitY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Target X: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setTooltipText(tr("Target Area X Position which the unit needs to reach."));
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(m_x);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setX(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Y: "));
    pText->setPosition(30, 150);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setTooltipText(tr("Target Area Y Position which the unit needs to reach."));
    spinBox->setPosition(width, 150);
    spinBox->setCurrentValue(m_y);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Width: "));
    pText->setPosition(30, 190);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(190, 1, 99999);
    spinBox->setTooltipText(tr("Target Area width which the unit needs to reach."));
    spinBox->setPosition(width, 190);
    spinBox->setCurrentValue(m_width);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setWidth(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Heigth: "));
    pText->setPosition(30, 230);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 99999);
    spinBox->setTooltipText(tr("Target Area heigth which the unit needs to reach."));
    spinBox->setPosition(width, 230);
    spinBox->setCurrentValue(m_heigth);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setHeigth(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}
