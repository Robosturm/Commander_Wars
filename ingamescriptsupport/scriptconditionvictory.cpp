#include "scriptconditionvictory.h"

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
