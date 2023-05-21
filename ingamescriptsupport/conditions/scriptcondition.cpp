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
#include "ingamescriptsupport/conditions/scriptconditionisco.h"

#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"

const char* const ScriptCondition::ConditionVictory = "Victory";
const char* const ScriptCondition::ConditionStartOfTurn = "Start Of Turn";
const char* const ScriptCondition::ConditionEachDay = "Each Day";
const char* const ScriptCondition::ConditionUnitDestroyed = "Unit Destroyed";
const char* const ScriptCondition::ConditionBuildingDestroyed = "Building Destroyed";
const char* const ScriptCondition::ConditionTerrainDestroyed = "Terrain Destroyed";
const char* const ScriptCondition::ConditionBuildingCaptured = "Building Captured";
const char* const ScriptCondition::ConditionPlayerDefeated = "Player Defeated";
const char* const ScriptCondition::ConditionUnitsDestroyed = "Units Destroyed";
const char* const ScriptCondition::ConditionBuildingsOwned = "Buildings Owned";
const char* const ScriptCondition::ConditionPlayerReachedArea = "Player in Area";
const char* const ScriptCondition::ConditionUnitReachedArea = "Unit in Area";
const char* const ScriptCondition::ConditionCheckVariable = "Check Variable";
const char* const ScriptCondition::ConditionIsCo = "Is selected Co";

ScriptCondition::ScriptCondition(GameMap* pMap, ConditionType type)
    : m_Type(type),
      m_pMap(pMap)
{
    Interpreter::setCppOwnerShip(this);
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

bool ScriptCondition::readSubCondition(GameMap* pMap, QTextStream& rStream, QString id, QString & line)
{
    line = rStream.readLine();
    line = line.simplified();
    if (line.endsWith(id + " End"))
    {
        return true;
    }
    if (subCondition.get() == nullptr)
    {
        setSubCondition(createReadCondition(pMap, rStream, line));
    }
    if (subCondition.get() != nullptr)
    {
        CONSOLE_PRINT("Added sub condition to current condition", GameConsole::eDEBUG);
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

spScriptCondition ScriptCondition::createCondition(GameMap* pMap, ConditionType type)
{
    switch (type)
    {
        case ConditionType::victory:
        {
            return spScriptConditionVictory::create(pMap);
        }
        case ConditionType::startOfTurn:
        {
            return spScriptConditionStartOfTurn::create(pMap);
        }
        case ConditionType::eachDay:
        {
            return spScriptConditionEachDay::create(pMap);
        }
        case ConditionType::unitDestroyed:
        {
            return spScriptConditionUnitDestroyed::create(pMap);
        }
        case ConditionType::buildingDestroyed:
        {
            return spScriptConditionBuildingDestroyed::create(pMap);
        }
        case ConditionType::buildingCaptured:
        {
            return spScriptConditionBuildingCaptured::create(pMap);
        }
        case ConditionType::playerDefeated:
        {
            return spScriptConditionPlayerDefeated::create(pMap);
        }
        case ConditionType::unitsDestroyed:
        {
            return spScriptConditionUnitsDestroyed::create(pMap);
        }
        case ConditionType::buildingsOwned:
        {
            return spScriptConditionBuildingsOwned::create(pMap);
        }
        case ConditionType::playerReachedArea:
        {
            return spScriptConditionPlayerReachedArea::create(pMap);
        }
        case ConditionType::unitReachedArea:
        {
            return spScriptConditionUnitReachedArea::create(pMap);
        }
        case ConditionType::terrainDestroyed:
        {
            return spScriptConditionTerrainDestroyed::create(pMap);
        }
        case ConditionType::checkVariable:
        {
            return spScriptConditionCheckVariable::create(pMap);
        }
        case ConditionType::isCo:
        {
            return spScriptConditionIsCo::create(pMap);
        }
    }
    return spScriptCondition();
}


spScriptCondition ScriptCondition::createReadCondition(GameMap* pMap, QTextStream& rStream, QString & line)
{
    line = line.simplified();
    CONSOLE_PRINT("Creating condition for line " + line, GameConsole::eDEBUG);
    spScriptCondition ret;
    if (line.endsWith(ConditionEachDay))
    {
         ret = spScriptConditionEachDay::create(pMap);
    }
    else if (line.endsWith(ConditionStartOfTurn))
    {
        ret = spScriptConditionStartOfTurn::create(pMap);
    }
    else if (line.endsWith(ConditionVictory))
    {
        ret = spScriptConditionVictory::create(pMap);
    }
    else if (line.endsWith(ConditionUnitDestroyed))
    {
        ret = spScriptConditionUnitDestroyed::create(pMap);
    }
    else if (line.endsWith(ConditionBuildingDestroyed))
    {
        ret = spScriptConditionBuildingDestroyed::create(pMap);
    }
    else if (line.endsWith(ConditionBuildingCaptured))
    {
        ret = spScriptConditionBuildingCaptured::create(pMap);
    }
    else if (line.endsWith(ConditionPlayerDefeated))
    {
        ret = spScriptConditionPlayerDefeated::create(pMap);
    }
    else if (line.endsWith(ConditionUnitsDestroyed))
    {
        ret = spScriptConditionUnitsDestroyed::create(pMap);
    }
    else if (line.endsWith(ConditionBuildingsOwned))
    {
        ret = spScriptConditionBuildingsOwned::create(pMap);
    }
    else if (line.endsWith(ConditionUnitReachedArea))
    {
        ret = spScriptConditionUnitReachedArea::create(pMap);
    }
    else if (line.endsWith(ConditionPlayerReachedArea))
    {
        ret = spScriptConditionPlayerReachedArea::create(pMap);
    }
    else if (line.endsWith(ConditionTerrainDestroyed))
    {
        ret = spScriptConditionTerrainDestroyed::create(pMap);
    }
    else if (line.endsWith(ConditionCheckVariable))
    {
        ret = spScriptConditionCheckVariable::create(pMap);
    }
    else if (line.endsWith(ConditionIsCo))
    {
        ret = spScriptConditionIsCo::create(pMap);
    }
    if (ret.get() != nullptr)
    {
        CONSOLE_PRINT("Found valid condition of type " + QString::number(static_cast<qint32>(ret->getType())), GameConsole::eDEBUG);
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
    return spScriptEvent();
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
        case ScriptCondition::ConditionType::isCo:
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
                case ScriptCondition::ConditionType::isCo:
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
                case ScriptCondition::ConditionType::isCo:
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
                case ScriptCondition::ConditionType::isCo:
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
    CONSOLE_PRINT("ScriptCondition::writePreCondition", GameConsole::eDEBUG);
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptCondition::writePostCondition(QTextStream& rStream)
{
    CONSOLE_PRINT("ScriptCondition::writePostCondition", GameConsole::eDEBUG);
    if (pParent != nullptr)
    {
        pParent->writePostCondition(rStream);
    }
}
