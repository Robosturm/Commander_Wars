#include "scriptcondition.h"

#include "scriptconditionvictory.h"
#include "scriptconditioneachday.h"
#include "scriptconditionstartofturn.h"
#include "scriptconditionunitdestroyed.h"
#include "scriptconditionbuildingdestroyed.h"
#include "scriptconditionbuildingcaptured.h"
#include "scriptconditionplayerdefeated.h"
#include "scriptconditionunitsdestroyed.h"
#include "scriptconditionbuildingsowned.h"

const QString ScriptCondition::ConditionVictory = "Victory";
const QString ScriptCondition::ConditionStartOfTurn = "Start Of Turn";
const QString ScriptCondition::ConditionEachDay = "Each Day";
const QString ScriptCondition::ConditionUnitDestroyed = "Unit Destroyed";
const QString ScriptCondition::ConditionBuildingDestroyed = "Building Destroyed";
const QString ScriptCondition::ConditionBuildingCaptured = "Building Captured";
const QString ScriptCondition::ConditionPlayerDefeated = "Player Defeated";
const QString ScriptCondition::ConditionUnitsDestroyed = "Units Destroyed";
const QString ScriptCondition::ConditionBuildingsOwned = "Buildings Owned";
const QString ScriptCondition::ConditionPlayerReachedArea = "Player Reached Area";
const QString ScriptCondition::ConditionUnitReachedArea = "Unit Reached Area";


ScriptCondition::ScriptCondition(ConditionType type)
    : QObject(),
      m_Type(type)
{
}

ScriptCondition *ScriptCondition::getParent() const
{
    return pParent;
}

void ScriptCondition::setParent(ScriptCondition *value)
{
    pParent = value;
}

spScriptCondition ScriptCondition::getSubCondition() const
{
    return subCondition;
}

void ScriptCondition::setSubCondition(const spScriptCondition &value)
{
    subCondition = value;
    if (subCondition.get() != nullptr)
    {
        subCondition->setParent(this);
    }
}

bool ScriptCondition::readSubCondition(QTextStream& rStream, QString id)
{
    qint64 pos = rStream.pos();
    QString line = rStream.readLine().simplified();
    if (line.endsWith(id + " End"))
    {
        return true;
    }
    rStream.seek(pos);
    if (subCondition.get() == nullptr)
    {
        setSubCondition(createReadCondition(rStream));
    }
    if (subCondition.get() != nullptr)
    {
        pos = rStream.pos();
        line = rStream.readLine().simplified();
        if (line.endsWith(id + " End"))
        {
            return true;
        }
        rStream.seek(pos);
    }
    return false;
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
            return new ScriptConditionStartOfTurn();
        }
        case ConditionType::eachDay:
        {
            return new ScriptConditionEachDay();
        }
        case ConditionType::unitDestroyed:
        {
            return new ScriptConditionUnitDestroyed();
        }
        case ConditionType::buildingDestroyed:
        {
            return new ScriptConditionBuildingDestroyed();
        }
        case ConditionType::buildingCaptured:
        {
            return new ScriptConditionBuildingCaptured();
        }
        case ConditionType::playerDefeated:
        {
            return new ScriptConditionPlayerDefeated();
        }
        case ConditionType::unitsDestroyed:
        {
            return new ScriptConditionUnitsDestroyed();
        }
        case ConditionType::buildingsOwned:
        {
            return new ScriptConditionBuildingsOwned();
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
    else if (line.endsWith(ConditionUnitDestroyed))
    {
        ret = new ScriptConditionUnitDestroyed();
    }
    else if (line.endsWith(ConditionBuildingDestroyed))
    {
        ret = new ScriptConditionBuildingDestroyed();
    }
    else if (line.endsWith(ConditionBuildingCaptured))
    {
        ret = new ScriptConditionBuildingCaptured();
    }
    else if (line.endsWith(ConditionPlayerDefeated))
    {
        ret = new ScriptConditionPlayerDefeated();
    }
    else if (line.endsWith(ConditionUnitsDestroyed))
    {
        ret = new ScriptConditionUnitsDestroyed();
    }
    else if (line.endsWith(ConditionBuildingsOwned))
    {
        ret = new ScriptConditionBuildingsOwned();
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

void ScriptCondition::removeEvent(spScriptEvent pEvent)
{
    for (qint32 i = 0; i < events.size(); i++)
    {
        if (events[i].get() == pEvent.get())
        {
            events.removeAt(i);
            break;
        }
    }
}

bool ScriptCondition::sameConditionGroup(ConditionType type1, ConditionType type2)
{
    switch (type1)
    {
        case ScriptCondition::ConditionType::victory:
        {
            return (type2 == type1);
        }
        case ScriptCondition::ConditionType::startOfTurn:
        case ScriptCondition::ConditionType::eachDay:
        {
            switch (type2)
            {
                case ScriptCondition::ConditionType::startOfTurn:
                case ScriptCondition::ConditionType::eachDay:
                {
                    return true;
                }
                default:
                {
                    return false;
                }
            }
        }
        case ScriptCondition::ConditionType::buildingDestroyed:
        case ScriptCondition::ConditionType::buildingCaptured:
        case ScriptCondition::ConditionType::playerDefeated:
        case ScriptCondition::ConditionType::unitsDestroyed:
        case ScriptCondition::ConditionType::buildingsOwned:
        case ScriptCondition::ConditionType::unitDestroyed:
        {
            switch (type2)
            {
                case ScriptCondition::ConditionType::buildingDestroyed:
                case ScriptCondition::ConditionType::buildingCaptured:
                case ScriptCondition::ConditionType::playerDefeated:
                case ScriptCondition::ConditionType::unitsDestroyed:
                case ScriptCondition::ConditionType::buildingsOwned:
                case ScriptCondition::ConditionType::unitDestroyed:
                {
                    return true;
                }
                default:
                {
                    return false;
                }
            }
        }
    }
}

void ScriptCondition::writePreCondition(QTextStream& rStream)
{
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptCondition::writePostCondition(QTextStream& rStream)
{
    if (pParent != nullptr)
    {
        pParent->writePostCondition(rStream);
    }
}
