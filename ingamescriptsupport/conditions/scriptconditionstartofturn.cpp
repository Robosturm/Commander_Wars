#include "ingamescriptsupport/conditions/scriptconditionstartofturn.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptConditionStartOfTurn::ScriptConditionStartOfTurn()
    : ScriptCondition (ConditionType::startOfTurn)
{

}

qint32 ScriptConditionStartOfTurn::getDay() const
{
    return day;
}

void ScriptConditionStartOfTurn::setDay(const qint32 &value)
{
    day = value;
}

qint32 ScriptConditionStartOfTurn::getPlayer() const
{
    return player;
}

void ScriptConditionStartOfTurn::setPlayer(const qint32 &value)
{
    player = value;
}

void ScriptConditionStartOfTurn::readCondition(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace("if (turn === ", "")
                            .replace(" && player === ", ",")
                            .replace(") { // ", ",").split(",");
    if (items.size() >= 2)
    {
        day = items[0].toInt();
        player = items[1].toInt();
        while (!rStream.atEnd())
        {
            if (readSubCondition(rStream, ConditionStartOfTurn))
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

void ScriptConditionStartOfTurn::writeCondition(QTextStream& rStream)
{
    rStream << "        if (turn === " + QString::number(day) + " && player === " + QString::number(player) + ") { // "
            << QString::number(getVersion()) << " " << ConditionStartOfTurn +"\n";
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    if (subCondition.get() != nullptr)
    {
        subCondition->writeCondition(rStream);
    }
    rStream << "        } // " + ConditionStartOfTurn + " End\n";
}

void ScriptConditionStartOfTurn::showEditCondition(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("At Day: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("Day at which the events get executed at the start of this turn."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(day);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptConditionStartOfTurn::setDay, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("Player at which the events get executed at the start of this turn."));
    spinBox->setPosition(width, 70);
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
