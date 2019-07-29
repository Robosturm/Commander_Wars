#include "scriptconditionvictory.h"

#include "scripteditor.h"
#include "genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/spinbox.h"

ScriptConditionVictory::ScriptConditionVictory()
    : ScriptCondition (ConditionType::victory)
{

}

qint32 ScriptConditionVictory::getTeam() const
{
    return team;
}

void ScriptConditionVictory::setTeam(const qint32 &value)
{
    team = value;
}

void ScriptConditionVictory::readCondition(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    team = line.replace("if (team === ", "").replace(") { // " + ConditionVictory, "").toInt();
    while (!rStream.atEnd())
    {
        qint64 pos = rStream.pos();
        line = rStream.readLine().simplified();
        if (line.endsWith(ConditionVictory))
        {
            break;
        }
        else
        {
            rStream.seek(pos);
        }
        events.append(ScriptEvent::createReadEvent(rStream));
    }
}

void ScriptConditionVictory::writeCondition(QTextStream& rStream)
{
    rStream << "        if (team === " + QString::number(team) +") { // " + ConditionVictory +"\n";
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "        } // " + ConditionVictory + "\n";
}

void ScriptConditionVictory::showEditCondition(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("Team: ").toStdString().c_str());
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = new SpinBox(150, 1, 9999);
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(team + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setTeam(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateConditios, Qt::QueuedConnection);
}
