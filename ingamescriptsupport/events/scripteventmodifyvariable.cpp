#include "scripteventmodifyvariable.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/textbox.h"
#include "objects/spinbox.h"
#include "objects/dropdownmenu.h"
#include "objects/checkbox.h"

ScriptEventModifyVariable::ScriptEventModifyVariable()
    : ScriptEvent (EventType::modifyVariable)
{

}

void ScriptEventModifyVariable::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QString variableName;
    if (line.startsWith(ScriptData::variables))
    {
        variableName = ScriptData::variables;
        m_CampaignVariable = false;
    }
    else
    {
        variableName = ScriptData::campaignVariables;
        m_CampaignVariable = true;
    }
    line = line.replace(variableName + ".createVariable(\"", "");
    QStringList list = line.replace("\").writeDataInt32(", "@").replace("); // ", "@").split("@");
    if (list.size() > 2)
    {
        m_variable = list[0];
        if (list[1].startsWith(m_variable))
        {
            QString item = list[1].replace(m_variable + "\").readDataInt32() ", "");
            QStringList items = list[1].split(" ");
            if (items.size() == 2)
            {
                m_Modifier = items[0];
                m_value = items[1].toInt();
            }
        }
        else
        {
            m_Modifier = "=";
            m_value = list[1].toInt();
        }
    }
}

void ScriptEventModifyVariable::writeEvent(QTextStream& rStream)
{
    QString equation;
    QString variableName = ScriptData::variables;
    if (m_CampaignVariable)
    {
        variableName = ScriptData::campaignVariables;
    }
    if (m_Modifier == "=")
    {
        equation = QString::number(m_value);
    }
    else
    {
        equation = variableName + ".createVariable(\"" + m_variable + "\").readDataInt32() " + m_Modifier + " " + QString::number(m_value);
    }
    rStream <<  "            " << variableName << ".createVariable(\"" << m_variable << "\").writeDataInt32(" << equation << "); // "
            << QString::number(getVersion()) << " " << EventModifyVariable << "\n";
}

void ScriptEventModifyVariable::showEditEvent(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("Variable: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spTextbox textBox = new Textbox(300);
    textBox->setTooltipText(tr("Name of the Variable that should be changed."));
    textBox->setPosition(width, 30);
    textBox->setCurrentText(m_variable);
    connect(textBox.get(), &Textbox::sigTextChanged,
            [=](QString value)
    {
        m_variable = value;
    });
    pBox->addItem(textBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Modifier: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    QVector<QString> items = {"=", "+", "-", "*", "/", "%"};
    spDropDownmenu dropDown = new DropDownmenu(150, items);
    dropDown->setTooltipText(tr("The way how the variable gets modified."));
    dropDown->setPosition(width, 70);
    dropDown->setCurrentItemText(m_Modifier);
    connect(dropDown.get(), &DropDownmenu::sigItemChanged,
            [=](qint32)
    {
        m_Modifier = dropDown->getCurrentItemText();
    });
    pBox->addItem(dropDown);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Value: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spSpinBox spinBox = new SpinBox(150, 0, 999999);
    spinBox->setTooltipText(tr("The value modifying the variable."));
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(m_value);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        m_value = value;
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Campaign Variable: "));
    pText->setPosition(30, 150);
    pBox->addItem(pText);
    spCheckbox checkBox = new Checkbox();
    checkBox->setTooltipText(tr("If checked the map needs to be played in a campaign and the variable is avaible during all campaign maps and in the campaign."));
    checkBox->setPosition(width, 150);
    checkBox->setChecked(m_CampaignVariable);
    connect(checkBox.get(), &Checkbox::checkChanged,
            [=](bool value)
    {
        m_CampaignVariable = value;
    });
    pBox->addItem(checkBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
