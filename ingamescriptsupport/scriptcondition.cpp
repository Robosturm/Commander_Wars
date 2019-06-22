#include "scriptcondition.h"

#include "scriptconditionvictory.h"
#include "scriptconditioneachday.h"
#include "scriptconditionstartofturn.h"

const QString ScriptCondition::ConditionVictory = "Victory";
const QString ScriptCondition::ConditionStartOfTurn = "Start Of Turn";
const QString ScriptCondition::ConditionEachDay = "Each Day";

ScriptCondition::ScriptCondition(ConditionType type)
    : QObject(),
      m_Type(type)
{
}

ScriptCondition::ConditionType ScriptCondition::getType() const
{
    return m_Type;
}

ScriptCondition* ScriptCondition::createCondition(ConditionType type)
{
    switch (type)
    {
        case ConditionType::victory:
        {
            return new ScriptConditionVictory();
        }
        case ConditionType::startOfTurn:
        {
            return new ScriptConditionEachDay();
        }
        case ConditionType::eachDay:
        {
            return new ScriptConditionStartOfTurn();
        }
    }
    return nullptr;
}


ScriptCondition* ScriptCondition::createReadCondition(QTextStream& rStream)
{
    qint64 pos = rStream.pos();
    QString line = rStream.readLine().simplified();
    rStream.seek(pos);
    ScriptCondition* ret = nullptr;
    if (line.endsWith(ConditionEachDay))
    {
         ret = new ScriptConditionEachDay();
    }
    else if (line.endsWith(ConditionStartOfTurn))
    {
        ret = new ScriptConditionStartOfTurn();
    }
    else if (line.endsWith(ConditionVictory))
    {
        ret = new ScriptConditionVictory();
    }
    if (ret != nullptr)
    {
        ret->readCondition(rStream);
    }
    return ret;
}
void ScriptCondition::addEvent(spScriptEvent event)
{
    events.append(event);
}

spScriptEvent ScriptCondition::getEvent(qint32 index)
{
    if (index >= 0 && index < events.size())
    {
       return events[index];
    }
    return nullptr;
}

void ScriptCondition::removeEvent(qint32 index)
{
    if (index >= 0 && index < events.size())
    {
        events.removeAt(index);
    }
}
