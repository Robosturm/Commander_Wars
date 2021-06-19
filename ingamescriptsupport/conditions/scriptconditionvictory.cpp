#include "ingamescriptsupport/conditions/scriptconditionvictory.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

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

void ScriptConditionVictory::readCondition(QTextStream& rStream, QString line)
{
    Console::print("Reading ConditionVictory", Console::eDEBUG);
    line = line.simplified();
    QStringList items = line.replace("if (team === ", "").replace(") { // ", ",").split(",");
    if (items.size() > 0)
    {
        team = items[0].toInt();
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(rStream, ConditionVictory, line))
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

void ScriptConditionVictory::writeCondition(QTextStream& rStream)
{
    Console::print("Writing ConditionVictory", Console::eDEBUG);
    rStream << "        if (team === " + QString::number(team) +") { // "
            << QString::number(getVersion()) << " " << ConditionVictory + "\n";
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    if (subCondition.get() != nullptr)
    {
        subCondition->writeCondition(rStream);
    }
    rStream << "        } // " + ConditionVictory + " End\n";
}

void ScriptConditionVictory::showEditCondition(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("Team: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("Team that has to win the game to executed the events."));
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
