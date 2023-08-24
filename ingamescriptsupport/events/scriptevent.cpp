#include "ingamescriptsupport/events/scriptevent.h"
#include "ingamescriptsupport/events/scripteventdialog.h"
#include "ingamescriptsupport/events/scripteventspawnunit.h"
#include "ingamescriptsupport/events/scripteventdefeatplayer.h"
#include "ingamescriptsupport/events/scripteventchangebuildlist.h"
#include "ingamescriptsupport/events/scripteventaddfunds.h"
#include "ingamescriptsupport/events/scripteventchangecobar.h"
#include "ingamescriptsupport/events/scripteventchangeweather.h"
#include "ingamescriptsupport/events/scripteventmodifyterrain.h"
#include "ingamescriptsupport/events/scripteventmodifyunit.h"
#include "ingamescriptsupport/events/scripteventanimation.h"
#include "ingamescriptsupport/events/scripteventvictoryinfo.h"
#include "ingamescriptsupport/events/scripteventmodifyvariable.h"
#include "ingamescriptsupport/events/scripteventchangeunitai.h"
#include "ingamescriptsupport/events/scripteventchangeunitowner.h"
#include "ingamescriptsupport/events/scripteventchangebuildingowner.h"
#include "ingamescriptsupport/events/scripteventchangeplayerteam.h"
#include "ingamescriptsupport/events/scripteventspawnbuilding.h"
#include "ingamescriptsupport/events/scripteventcentermap.h"
#include "ingamescriptsupport/events/scripteventplaysound.h"
#include "ingamescriptsupport/events/scripteventbuildingfirecounter.h"
#include "ingamescriptsupport/events/scripteventvolcanfire.h"
#include "ingamescriptsupport/events/scripteventextendmap.h"

#include "coreengine/interpreter.h"

const char* const ScriptEvent::EventDialog  = "Dialog";
const char* const ScriptEvent::EventSpawnUnit = "Spawn Unit";
const char* const ScriptEvent::EventDefeatPlayer = "Defeat Player";
const char* const ScriptEvent::EventChangeBuildlist = "Change Buildlist";
const char* const ScriptEvent::EventAddFunds = "Add Funds";
const char* const ScriptEvent::EventChangeWeather = "Change Weather";
const char* const ScriptEvent::EventChangeCOBar = "Change CO Bar";
const char* const ScriptEvent::EventModifyUnit = "Modify Unit";
const char* const ScriptEvent::EventAnimation = "Animation";
const char* const ScriptEvent::EventModifyTerrain = "Modify Terrain";
const char* const ScriptEvent::EventVictoryInfo = "Victory Info";
const char* const ScriptEvent::EventModifyVariable = "Modify Variable";
const char* const ScriptEvent::EventChangeUnitAI = "Change Unit AI";
const char* const ScriptEvent::EventChangeBuildingOwner = "Change Building Owner";
const char* const ScriptEvent::EventChangeUnitOwner = "Change Unit Owner";
const char* const ScriptEvent::EventChangePlayerTeam = "Change Player Team";
const char* const ScriptEvent::EventSpawnBuilding = "Spawn Building";
const char* const ScriptEvent::EventCenterMap = "Center Map";
const char* const ScriptEvent::EventPlaySound = "Play Sound";
const char* const ScriptEvent::EventBuildingFireCounter = "BuildingFireCounter";
const char* const ScriptEvent::EventVolcanFire = "VolcanFire";
const char* const ScriptEvent::EventExtendMap = "ExtendMap";


ScriptEvent::ScriptEvent(GameMap* pMap, EventType type)
    : m_Type(type),
      m_pMap(pMap)
{
    Interpreter::setCppOwnerShip(this);
}

spScriptEvent ScriptEvent::createReadEvent(GameMap* pMap, QTextStream& rStream, QString line)
{
    line = line.simplified();
    spScriptEvent ret;
    if (line.endsWith(EventDialog))
    {
         ret = MemoryManagement::create<ScriptEventDialog>(pMap);
    }
    else if (line.endsWith(EventSpawnUnit))
    {
         ret = MemoryManagement::create<ScriptEventSpawnUnit>(pMap);
    }
    else if (line.endsWith(EventDefeatPlayer))
    {
         ret = MemoryManagement::create<ScriptEventDefeatPlayer>(pMap);
    }
    else if (line.endsWith(EventAddFunds))
    {
         ret = MemoryManagement::create<ScriptEventAddFunds>(pMap);
    }
    else if (line.endsWith(EventChangeBuildlist))
    {
         ret = MemoryManagement::create<ScriptEventChangeBuildlist>(pMap);
    }
    else if (line.endsWith(EventChangeWeather))
    {
         ret = MemoryManagement::create<ScriptEventChangeWeather>(pMap);
    }
    else if (line.endsWith(EventChangeCOBar))
    {
         ret = MemoryManagement::create<ScriptEventChangeCOBar>(pMap);
    }
    else if (line.endsWith(EventModifyTerrain))
    {
         ret = MemoryManagement::create<ScriptEventModifyTerrain>(pMap);
    }
    else if (line.endsWith(EventModifyUnit))
    {
         ret = MemoryManagement::create<ScriptEventModifyUnit>(pMap);
    }
    else if (line.endsWith(EventAnimation))
    {
         ret = MemoryManagement::create<ScriptEventAnimation>(pMap);
    }
    else if (line.endsWith(EventVictoryInfo))
    {
         ret = MemoryManagement::create<ScriptEventVictoryInfo>(pMap);
    }
    else if (line.endsWith(EventModifyVariable))
    {
         ret = MemoryManagement::create<ScriptEventModifyVariable>(pMap);
    }
    else if (line.endsWith(EventChangeUnitAI))
    {
        ret = MemoryManagement::create<ScriptEventChangeUnitAI>(pMap);
    }
    else if (line.endsWith(EventChangeBuildingOwner))
    {
        ret = MemoryManagement::create<ScriptEventChangeBuildingOwner>(pMap);
    }
    else if (line.endsWith(EventChangeUnitOwner))
    {
        ret = MemoryManagement::create<ScriptEventChangeUnitOwner>(pMap);
    }
    else if (line.endsWith(EventChangePlayerTeam))
    {
        ret = MemoryManagement::create<ScriptEventChangePlayerTeam>(pMap);
    }
    else if (line.endsWith(EventSpawnBuilding))
    {
        ret = MemoryManagement::create<ScriptEventSpawnBuilding>(pMap);
    }
    else if (line.endsWith(EventCenterMap))
    {
        ret = MemoryManagement::create<ScriptEventCenterMap>(pMap);
    }    
    else if (line.endsWith(EventPlaySound))
    {
        ret = MemoryManagement::create<ScriptEventPlaySound>(pMap);
    }
    else if (line.endsWith(EventBuildingFireCounter))
    {
        ret = MemoryManagement::create<ScriptEventBuildingFireCounter>(pMap);
    }
    else if (line.endsWith(EventVolcanFire))
    {
        ret = MemoryManagement::create<ScriptEventVolcanFire>(pMap);
    }
    else if (line.endsWith(EventExtendMap))
    {
        ret = MemoryManagement::create<ScriptEventExtendMap>(pMap);
    }
    if (ret.get() != nullptr)
    {
        ret->readEvent(rStream, line);
    }
    else
    {
    }
    return ret;
}

spScriptEvent ScriptEvent::createEvent(GameMap* pMap, EventType type)
{
    switch (type)
    {
        case EventType::dialog:
        {
            return MemoryManagement::create<ScriptEventDialog>(pMap);
        }
        case EventType::spawnUnit:
        {
            return MemoryManagement::create<ScriptEventSpawnUnit>(pMap);
        }
        case EventType::defeatPlayer:
        {
            return MemoryManagement::create<ScriptEventDefeatPlayer>(pMap);
        }
        case EventType::addFunds:
        {
            return MemoryManagement::create<ScriptEventAddFunds>(pMap);
        }
        case EventType::changeCOBar:
        {
            return MemoryManagement::create<ScriptEventChangeCOBar>(pMap);
        }
        case EventType::changeWeather:
        {
            return MemoryManagement::create<ScriptEventChangeWeather>(pMap);
        }
        case EventType::changeBuildlist:
        {
            return MemoryManagement::create<ScriptEventChangeBuildlist>(pMap);
        }
        case EventType::modifyTerrain:
        {
            return MemoryManagement::create<ScriptEventModifyTerrain>(pMap);
        }
        case EventType::modifyUnit:
        {
            return MemoryManagement::create<ScriptEventModifyUnit>(pMap);
        }
        case EventType::animation:
        {
            return MemoryManagement::create<ScriptEventAnimation>(pMap);
        }
        case EventType::victoryInfo:
        {
            return MemoryManagement::create<ScriptEventVictoryInfo>(pMap);
        }
        case EventType::modifyVariable:
        {
            return MemoryManagement::create<ScriptEventModifyVariable>(pMap);
        }
        case EventType::ChangeUnitAI:
        {
            return MemoryManagement::create<ScriptEventChangeUnitAI>(pMap);
        }
        case EventType::ChangeBuildingOwner:
        {
            return MemoryManagement::create<ScriptEventChangeBuildingOwner>(pMap);
        }
        case EventType::ChangeUnitOwner:
        {
            return MemoryManagement::create<ScriptEventChangeUnitOwner>(pMap);
        }
        case EventType::ChangePlayerTeam:
        {
            return MemoryManagement::create<ScriptEventChangePlayerTeam>(pMap);
        }
        case EventType::SpawnBuilding:
        {
            return MemoryManagement::create<ScriptEventSpawnBuilding>(pMap);
        }
        case EventType::CenterMap:
        {
            return MemoryManagement::create<ScriptEventCenterMap>(pMap);
        }
        case EventType::PlayGameSound:
        {
            return MemoryManagement::create<ScriptEventPlaySound>(pMap);
        }
        case EventType::BuildingFireCounter:
        {
            return MemoryManagement::create<ScriptEventBuildingFireCounter>(pMap);
        }
        case EventType::VolcanFire:
        {
            return MemoryManagement::create<ScriptEventVolcanFire>(pMap);
        }
        case EventType::ExtendMap:
        {
            return MemoryManagement::create<ScriptEventExtendMap>(pMap);
        }
    }        
    return spScriptEvent();
}
