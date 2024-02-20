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
#include "ingamescriptsupport/conditions/scriptconditiongatheredfunds.h"

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
const char* const ScriptCondition::ConditionGatheredFunds = "Gathered Funds";

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

void ScriptCondition::setSubCondition(const spScriptCondition & value)
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
            return MemoryManagement::create<ScriptConditionVictory>(pMap);
        }
        case ConditionType::startOfTurn:
        {
            return MemoryManagement::create<ScriptConditionStartOfTurn>(pMap);
        }
        case ConditionType::eachDay:
        {
            return MemoryManagement::create<ScriptConditionEachDay>(pMap);
        }
        case ConditionType::unitDestroyed:
        {
            return MemoryManagement::create<ScriptConditionUnitDestroyed>(pMap);
        }
        case ConditionType::buildingDestroyed:
        {
            return MemoryManagement::create<ScriptConditionBuildingDestroyed>(pMap);
        }
        case ConditionType::buildingCaptured:
        {
            return MemoryManagement::create<ScriptConditionBuildingCaptured>(pMap);
        }
        case ConditionType::playerDefeated:
        {
            return MemoryManagement::create<ScriptConditionPlayerDefeated>(pMap);
        }
        case ConditionType::unitsDestroyed:
        {
            return MemoryManagement::create<ScriptConditionUnitsDestroyed>(pMap);
        }
        case ConditionType::buildingsOwned:
        {
            return MemoryManagement::create<ScriptConditionBuildingsOwned>(pMap);
        }
        case ConditionType::playerReachedArea:
        {
            return MemoryManagement::create<ScriptConditionPlayerReachedArea>(pMap);
        }
        case ConditionType::unitReachedArea:
        {
            return MemoryManagement::create<ScriptConditionUnitReachedArea>(pMap);
        }
        case ConditionType::terrainDestroyed:
        {
            return MemoryManagement::create<ScriptConditionTerrainDestroyed>(pMap);
        }
        case ConditionType::checkVariable:
        {
            return MemoryManagement::create<ScriptConditionCheckVariable>(pMap);
        }
        case ConditionType::isCo:
        {
            return MemoryManagement::create<ScriptConditionIsCo>(pMap);
        }
        case ConditionType::gatheredFunds:
        {
            return MemoryManagement::create<ScriptConditionGatheredFunds>(pMap);
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
         ret = MemoryManagement::create<ScriptConditionEachDay>(pMap);
    }
    else if (line.endsWith(ConditionStartOfTurn))
    {
        ret = MemoryManagement::create<ScriptConditionStartOfTurn>(pMap);
    }
    else if (line.endsWith(ConditionVictory))
    {
        ret = MemoryManagement::create<ScriptConditionVictory>(pMap);
    }
    else if (line.endsWith(ConditionUnitDestroyed))
    {
        ret = MemoryManagement::create<ScriptConditionUnitDestroyed>(pMap);
    }
    else if (line.endsWith(ConditionBuildingDestroyed))
    {
        ret = MemoryManagement::create<ScriptConditionBuildingDestroyed>(pMap);
    }
    else if (line.endsWith(ConditionBuildingCaptured))
    {
        ret = MemoryManagement::create<ScriptConditionBuildingCaptured>(pMap);
    }
    else if (line.endsWith(ConditionPlayerDefeated))
    {
        ret = MemoryManagement::create<ScriptConditionPlayerDefeated>(pMap);
    }
    else if (line.endsWith(ConditionUnitsDestroyed))
    {
        ret = MemoryManagement::create<ScriptConditionUnitsDestroyed>(pMap);
    }
    else if (line.endsWith(ConditionBuildingsOwned))
    {
        ret = MemoryManagement::create<ScriptConditionBuildingsOwned>(pMap);
    }
    else if (line.endsWith(ConditionUnitReachedArea))
    {
        ret = MemoryManagement::create<ScriptConditionUnitReachedArea>(pMap);
    }
    else if (line.endsWith(ConditionPlayerReachedArea))
    {
        ret = MemoryManagement::create<ScriptConditionPlayerReachedArea>(pMap);
    }
    else if (line.endsWith(ConditionTerrainDestroyed))
    {
        ret = MemoryManagement::create<ScriptConditionTerrainDestroyed>(pMap);
    }
    else if (line.endsWith(ConditionCheckVariable))
    {
        ret = MemoryManagement::create<ScriptConditionCheckVariable>(pMap);
    }
    else if (line.endsWith(ConditionIsCo))
    {
        ret = MemoryManagement::create<ScriptConditionIsCo>(pMap);
    }
    else if (line.endsWith(ConditionGatheredFunds))
    {
        ret = MemoryManagement::create<ScriptConditionGatheredFunds>(pMap);
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

void ScriptCondition::removeEvent(ScriptEvent* pEvent)
{
    for (qint32 i = 0; i < events.size(); i++)
    {
        if (events[i].get() == pEvent)
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
                case ScriptCondition::ConditionType::gatheredFunds:
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
        case ScriptCondition::ConditionType::gatheredFunds:
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
                case ScriptCondition::ConditionType::gatheredFunds:
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
