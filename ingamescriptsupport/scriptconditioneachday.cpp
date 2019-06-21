#include "scriptconditioneachday.h"

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
                            .replace(") { // " + ConditionEachDay, "").split(",");
    if (items.size() == 3)
    {
        day = items[0].toInt();
        intervall = items[1].toInt();
        player = items[2].toInt();
        while (!rStream.atEnd())
        {
            qint64 pos = rStream.pos();
            line = rStream.readLine().simplified();
            if (line.endsWith(ConditionEachDay))
            {
                break;
            }
            else
            {
                rStream.seek(pos);
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
    rStream << "        if ((turn - " + QString::number(day)  +  ") % " + QString::number(intervall) +  " === 0 && player === " + QString::number(player) + ") { // " + ConditionEachDay +"\n";
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "        } // " + ConditionEachDay + "\n";
}
