#include "ScriptConditionUnitReachedArea.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"

#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "objects/spinbox.h"

ScriptConditionUnitReachedArea::ScriptConditionUnitReachedArea()
    : ScriptCondition (ConditionType::unitReachedArea)
{

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

void ScriptConditionUnitReachedArea::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

void ScriptConditionUnitReachedArea::writePreCondition(QTextStream& rStream)
{
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = variables.createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionUnitReachedArea::showEditCondition(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = new GenericBox();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    oxygine::spTextField pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Unit X: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = new SpinBox(150, 0, 99999);
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(m_UnitX);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setUnitX(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Unit Y: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(m_UnitY);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setUnitY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Target X: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(m_x);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setX(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Y: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(m_y);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Width: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(m_width);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setWidth(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Target Heigth: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 99999);
    spinBox->setPosition(width, 110);
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
