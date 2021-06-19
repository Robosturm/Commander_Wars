#include "ingamescriptsupport/conditions/scriptcondition.h"
#include "ingamescriptsupport/conditions/scriptconditionvictory.h"
#include "ingamescriptsupport/conditions/scriptconditioneachday.h"
#include "ingamescriptsupport/conditions/scriptconditionstartofturn.h"
#include "ingamescriptsupport/conditions/scriptconditionunitdestroyed.h"
#include "ingamescriptsupport/conditions/scriptconditionbuildingdestroyed.h"
#include "ingamescriptsupport/conditions/scriptconditionterraindestroyed.h"
#include "ingamescriptsupport/conditions/scriptconditionbuildingcaptured.h"
#include "ingamescriptsupport/conditions/scriptconditionplayerdefeated.h"
#include "ingamescriptsupport/conditions/scriptconditionunitsdestroyed.h"
#include "ingamescriptsupport/conditions/scriptconditionbuildingsowned.h"
#include "ingamescriptsupport/conditions/ScriptConditionUnitReachedArea.h"
#include "ingamescriptsupport/conditions/scriptconditionplayerreachedarea.h"
#include "ingamescriptsupport/conditions/scriptconditioncheckvariable.h"

#include "coreengine/console.h"

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

bool ScriptCondition::readSubCondition(QTextStream& rStream, QString id, QString & line)
{
    line = rStream.readLine();
    line = line.simplified();
    if (line.endsWith(id + " End"))
    {
        return true;
    }
    if (subCondition.get() == nullptr)
    {
        setSubCondition(createReadCondition(rStream, line));
    }
    if (subCondition.get() != nullptr)
    {
        line = rStream.readLine();
        line = line.simplified();
        if (line.endsWith(id + " End"))
        {
            return true;
        }
    }
    return false;
}

ScriptCondition::ConditionType ScriptCondition::getType() const
{
    return m_Type;
}

spScriptCondition ScriptCondition::createCondition(ConditionType type)
{
    switch (type)
    {
        case ConditionType::victory:
        {
            return spScriptConditionVictory::create();
        }
        case ConditionType::startOfTurn:
        {
            return spScriptConditionStartOfTurn::create();
        }
        case ConditionType::eachDay:
        {
            return spScriptConditionEachDay::create();
        }
        case ConditionType::unitDestroyed:
        {
            return spScriptConditionUnitDestroyed::create();
        }
        case ConditionType::buildingDestroyed:
        {
            return spScriptConditionBuildingDestroyed::create();
        }
        case ConditionType::buildingCaptured:
        {
            return spScriptConditionBuildingCaptured::create();
        }
        case ConditionType::playerDefeated:
        {
            return spScriptConditionPlayerDefeated::create();
        }
        case ConditionType::unitsDestroyed:
        {
            return spScriptConditionUnitsDestroyed::create();
        }
        case ConditionType::buildingsOwned:
        {
            return spScriptConditionBuildingsOwned::create();
        }
        case ConditionType::playerReachedArea:
        {
            return spScriptConditionPlayerReachedArea::create();
        }
        case ConditionType::unitReachedArea:
        {
            return spScriptConditionUnitReachedArea::create();
        }
        case ConditionType::terrainDestroyed:
        {
            return spScriptConditionTerrainDestroyed::create();
        }
        case ConditionType::checkVariable:
        {
            return spScriptConditionCheckVariable::create();
        }
    }
    return nullptr;
}


spScriptCondition ScriptCondition::createReadCondition(QTextStream& rStream, QString & line)
{
    line = line.simplified();
    spScriptCondition ret = nullptr;
    if (line.endsWith(ConditionEachDay))
    {
         ret = spScriptConditionEachDay::create();
    }
    else if (line.endsWith(ConditionStartOfTurn))
    {
        ret = spScriptConditionStartOfTurn::create();
    }
    else if (line.endsWith(ConditionVictory))
    {
        ret = spScriptConditionVictory::create();
    }
    else if (line.endsWith(ConditionUnitDestroyed))
    {
        ret = spScriptConditionUnitDestroyed::create();
    }
    else if (line.endsWith(ConditionBuildingDestroyed))
    {
        ret = spScriptConditionBuildingDestroyed::create();
    }
    else if (line.endsWith(ConditionBuildingCaptured))
    {
        ret = spScriptConditionBuildingCaptured::create();
    }
    else if (line.endsWith(ConditionPlayerDefeated))
    {
        ret = spScriptConditionPlayerDefeated::create();
    }
    else if (line.endsWith(ConditionUnitsDestroyed))
    {
        ret = spScriptConditionUnitsDestroyed::create();
    }
    else if (line.endsWith(ConditionBuildingsOwned))
    {
        ret = spScriptConditionBuildingsOwned::create();
    }
    else if (line.endsWith(ConditionUnitReachedArea))
    {
        ret = spScriptConditionUnitReachedArea::create();
    }
    else if (line.endsWith(ConditionPlayerReachedArea))
    {
        ret = spScriptConditionPlayerReachedArea::create();
    }
    else if (line.endsWith(ConditionTerrainDestroyed))
    {
        ret = spScriptConditionTerrainDestroyed::create();
    }
    else if (line.endsWith(ConditionCheckVariable))
    {
        ret = spScriptConditionCheckVariable::create();
    }
    if (ret.get() != nullptr)
    {
        ret->readCondition(rStream, line);
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
    Console::print("ScriptCondition::writePreCondition", Console::eDEBUG);
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptCondition::writePostCondition(QTextStream& rStream)
{
    Console::print("ScriptCondition::writePostCondition", Console::eDEBUG);
    if (pParent != nullptr)
    {
        pParent->writePostCondition(rStream);
    }
}
