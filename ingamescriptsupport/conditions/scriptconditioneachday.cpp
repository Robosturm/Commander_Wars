#include "ingamescriptsupport/conditions/scriptconditioneachday.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionEachDay::ScriptConditionEachDay()
    : ScriptCondition (ConditionType::eachDay)
{

}

qint32 ScriptConditionEachDay::getIntervall() const
{
    return intervall;
}

void ScriptConditionEachDay::setIntervall(const qint32 &value)
{
    intervall = value;
}

qint32 ScriptConditionEachDay::getDay() const
{
    return day;
}

void ScriptConditionEachDay::setDay(const qint32 &value)
{
    day = value;
}

qint32 ScriptConditionEachDay::getPlayer() const
{
    return player;
}

void ScriptConditionEachDay::setPlayer(const qint32 &value)
{
    player = value;
}

void ScriptConditionEachDay::readCondition(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace("if ((turn - ", "")
                            .replace(") % ", ",")
                            .replace(" === 0 && player === ", ",")
                            .replace(") { // ", ",").split(",");
    if (items.size() >= 3)
    {
        day = items[0].toInt();
        intervall = items[1].toInt();
        player = items[2].toInt();
        while (!rStream.atEnd())
        {
            if (readSubCondition(rStream, ConditionEachDay))
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
}

void ScriptConditionEachDay::writeCondition(QTextStream& rStream)
{
    rStream << "        if ((turn - " + QString::number(day)  +  ") % " + QString::number(intervall) +  " === 0 && player === " + QString::number(player) + ") { // "
            << QString::number(getVersion()) << " " << ConditionEachDay +"\n";
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    if (subCondition.get() != nullptr)
    {
        subCondition->writeCondition(rStream);
    }
    rStream << "        } // " + ConditionEachDay + " End\n";
}

void ScriptConditionEachDay::showEditCondition(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("Each Day: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("Periodic cycle in which this condition is executing it's events."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(intervall);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptConditionEachDay::setIntervall, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Start Day: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("The first day at which the events get executed."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(day);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptConditionEachDay::setDay, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("The first day at which the events get executed."));
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}
