#include "scriptcondition.h"

#include "scriptconditionvictory.h"
#include "scriptconditioneachday.h"
#include "scriptconditionstartofturn.h"
#include "scriptconditionunitdestroyed.h"
#include "scriptconditionbuildingdestroyed.h"
#include "scriptconditionterraindestroyed.h"
#include "scriptconditionbuildingcaptured.h"
#include "scriptconditionplayerdefeated.h"
#include "scriptconditionunitsdestroyed.h"
#include "scriptconditionbuildingsowned.h"
#include "ScriptConditionUnitReachedArea.h"
#include "scriptconditionplayerreachedarea.h"
#include "scriptconditioncheckvariable.h"

const QString ScriptCondition::ConditionVictory = "Victory";
const QString ScriptCondition::ConditionStartOfTurn = "Start Of Turn";
const QString ScriptCondition::ConditionEachDay = "Each Day";
const QString ScriptCondition::ConditionUnitDestroyed = "Unit Destroyed";
const QString ScriptCondition::ConditionBuildingDestroyed = "Building Destroyed";
const QString ScriptCondition::ConditionTerrainDestroyed = "Terrain Destroyed";
const QString ScriptCondition::ConditionBuildingCaptured = "Building Captured";
const QString ScriptCondition::ConditionPlayerDefeated = "Player Defeated";
const QString ScriptCondition::ConditionUnitsDestroyed = "Units Destroyed";
const QString ScriptCondition::ConditionBuildingsOwned = "Buildings Owned";
const QString ScriptCondition::ConditionPlayerReachedArea = "Player in Area";
const QString ScriptCondition::ConditionUnitReachedArea = "Unit in Area";
const QString ScriptCondition::ConditionCheckVariable = "Check Variable";

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

        case ConditionType::playerReachedArea:
        {
            return new ScriptConditionPlayerReachedArea();
        }
        case ConditionType::unitReachedArea:
        {
            return new ScriptConditionUnitReachedArea();
        }
        case ConditionType::terrainDestroyed:
        {
            return new ScriptConditionTerrainDestroyed();
        }
        case ConditionType::checkVariable:
        {
            return new ScriptConditionCheckVariable();
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
    else if (line.endsWith(ConditionUnitReachedArea))
    {
        ret = new ScriptConditionUnitReachedArea();
    }
    else if (line.endsWith(ConditionPlayerReachedArea))
    {
        ret = new ScriptConditionPlayerReachedArea();
    }
    else if (line.endsWith(ConditionTerrainDestroyed))
    {
        ret = new ScriptConditionTerrainDestroyed();
    }
    else if (line.endsWith(ConditionCheckVariable))
    {
        ret = new ScriptConditionCheckVariable();
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
        case ScriptCondition::ConditionType::checkVariable:
        {
            return true;
        }
        case ScriptCondition::ConditionType::victory:
        {
            switch (type2)
            {
                case ScriptCondition::ConditionType::victory:
                case ScriptCondition::ConditionType::checkVariable:
                {
                    return true;
                }
                default:
                {
                    return false;
                }
            }
        }
        case ScriptCondition::ConditionType::startOfTurn:
        case ScriptCondition::ConditionType::eachDay:
        {
            switch (type2)
            {
                case ScriptCondition::ConditionType::startOfTurn:
                case ScriptCondition::ConditionType::eachDay:
                case ScriptCondition::ConditionType::checkVariable:
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
        case ScriptCondition::ConditionType::terrainDestroyed:
        case ScriptCondition::ConditionType::buildingCaptured:
        case ScriptCondition::ConditionType::playerDefeated:
        case ScriptCondition::ConditionType::unitsDestroyed:
        case ScriptCondition::ConditionType::buildingsOwned:
        case ScriptCondition::ConditionType::unitDestroyed:
        case ScriptCondition::ConditionType::unitReachedArea:
        case ScriptCondition::ConditionType::playerReachedArea:
        {
            switch (type2)
            {
                case ScriptCondition::ConditionType::buildingDestroyed:
                case ScriptCondition::ConditionType::terrainDestroyed:
                case ScriptCondition::ConditionType::buildingCaptured:
                case ScriptCondition::ConditionType::playerDefeated:
                case ScriptCondition::ConditionType::unitsDestroyed:
                case ScriptCondition::ConditionType::buildingsOwned:
                case ScriptCondition::ConditionType::unitDestroyed:
                case ScriptCondition::ConditionType::unitReachedArea:
                case ScriptCondition::ConditionType::playerReachedArea:
                case ScriptCondition::ConditionType::checkVariable:
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
    return false;
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
