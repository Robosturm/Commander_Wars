#include "scriptconditionstartofturn.h"

#include "scripteditor.h"
#include "genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/spinbox.h"

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
                            .replace(") { // " + ConditionStartOfTurn, "").split(",");
    if (items.size() == 2)
    {
        day = items[0].toInt();
        player = items[1].toInt();
        while (!rStream.atEnd())
        {
            qint64 pos = rStream.pos();
            line = rStream.readLine().simplified();
            if (line.endsWith(ConditionStartOfTurn + " End"))
            {
                break;
            }
            else
            {
                rStream.seek(pos);
            }
            if (subCondition.get() == nullptr)
            {
                setSubCondition(createReadCondition(rStream));
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
    rStream << "        if (turn === " + QString::number(day) + " && player === " + QString::number(player) + ") { // " + ConditionStartOfTurn +"\n";
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
    spGenericBox pBox = new GenericBox();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    oxygine::spTextField pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("At Day: ").toStdString().c_str());
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = new SpinBox(150, 1, 9999);
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(day);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptConditionStartOfTurn::setDay, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: ").toStdString().c_str());
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 1, 9999);
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
