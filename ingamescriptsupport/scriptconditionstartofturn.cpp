#include "scriptconditionstartofturn.h"

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
            if (line.endsWith(ConditionStartOfTurn))
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
}

void ScriptConditionStartOfTurn::writeCondition(QTextStream& rStream)
{
    rStream << "        if (turn === " + QString::number(day) + " && player === " + QString::number(player) + ") { // " + ConditionStartOfTurn +"\n";
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "        } // " + ConditionStartOfTurn + "\n";
}
