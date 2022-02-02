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

#include "coreengine/console.h"

const QString ScriptEvent::EventDialog  = "Dialog";
const QString ScriptEvent::EventSpawnUnit = "Spawn Unit";
const QString ScriptEvent::EventDefeatPlayer = "Defeat Player";
const QString ScriptEvent::EventChangeBuildlist = "Change Buildlist";
const QString ScriptEvent::EventAddFunds = "Add Funds";
const QString ScriptEvent::EventChangeWeather = "Change Weather";
const QString ScriptEvent::EventChangeCOBar = "Change CO Bar";
const QString ScriptEvent::EventModifyUnit = "Modify Unit";
const QString ScriptEvent::EventAnimation = "Animation";
const QString ScriptEvent::EventModifyTerrain = "Modify Terrain";
const QString ScriptEvent::EventVictoryInfo = "Victory Info";
const QString ScriptEvent::EventModifyVariable = "Modify Variable";
const QString ScriptEvent::EventChangeUnitAI = "Change Unit AI";
const QString ScriptEvent::EventChangeBuildingOwner = "Change Building Owner";
const QString ScriptEvent::EventChangeUnitOwner = "Change Unit Owner";
const QString ScriptEvent::EventChangePlayerTeam = "Change Player Team";
const QString ScriptEvent::EventSpawnBuilding = "Spawn Building";
const QString ScriptEvent::EventCenterMap = "Center Map";
const QString ScriptEvent::EventPlaySound = "Play Sound";


ScriptEvent::ScriptEvent(GameMap* pMap, EventType type)
    : m_Type(type),
      m_pMap(pMap)
{
}

spScriptEvent ScriptEvent::createReadEvent(GameMap* pMap, QTextStream& rStream, QString line)
{
    line = line.simplified();
    spScriptEvent ret;
    if (line.endsWith(EventDialog))
    {
         ret = spScriptEventDialog::create(pMap);
    }
    else if (line.endsWith(EventSpawnUnit))
    {
         ret = spScriptEventSpawnUnit::create(pMap);
    }
    else if (line.endsWith(EventDefeatPlayer))
    {
         ret = spScriptEventDefeatPlayer::create(pMap);
    }
    else if (line.endsWith(EventAddFunds))
    {
         ret = spScriptEventAddFunds::create(pMap);
    }
    else if (line.endsWith(EventChangeBuildlist))
    {
         ret = spScriptEventChangeBuildlist::create(pMap);
    }
    else if (line.endsWith(EventChangeWeather))
    {
         ret = spScriptEventChangeWeather::create(pMap);
    }
    else if (line.endsWith(EventChangeCOBar))
    {
         ret = spScriptEventChangeCOBar::create(pMap);
    }
    else if (line.endsWith(EventModifyTerrain))
    {
         ret = spScriptEventModifyTerrain::create(pMap);
    }
    else if (line.endsWith(EventModifyUnit))
    {
         ret = spScriptEventModifyUnit::create(pMap);
    }
    else if (line.endsWith(EventAnimation))
    {
         ret = spScriptEventAnimation::create(pMap);
    }
    else if (line.endsWith(EventVictoryInfo))
    {
         ret = spScriptEventVictoryInfo::create(pMap);
    }
    else if (line.endsWith(EventModifyVariable))
    {
         ret = spScriptEventModifyVariable::create(pMap);
    }
    else if (line.endsWith(EventChangeUnitAI))
    {
        ret = spScriptEventChangeUnitAI::create(pMap);
    }
    else if (line.endsWith(EventChangeBuildingOwner))
    {
        ret = spScriptEventChangeBuildingOwner::create(pMap);
    }
    else if (line.endsWith(EventChangeUnitOwner))
    {
        ret = spScriptEventChangeUnitOwner::create(pMap);
    }
    else if (line.endsWith(EventChangePlayerTeam))
    {
        ret = spScriptEventChangePlayerTeam::create(pMap);
    }
    else if (line.endsWith(EventSpawnBuilding))
    {
        ret = spScriptEventSpawnBuilding::create(pMap);
    }
    else if (line.endsWith(EventCenterMap))
    {
        ret = spScriptEventCenterMap::create(pMap);
    }
    else if (line.endsWith(EventPlaySound))
    {
        ret = spScriptEventPlaySound::create(pMap);
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
            return spScriptEventDialog::create(pMap);
        }
        case EventType::spawnUnit:
        {
            return spScriptEventSpawnUnit::create(pMap);
        }
        case EventType::defeatPlayer:
        {
            return spScriptEventDefeatPlayer::create(pMap);
        }
        case EventType::addFunds:
        {
            return spScriptEventAddFunds::create(pMap);
        }
        case EventType::changeCOBar:
        {
            return spScriptEventChangeCOBar::create(pMap);
        }
        case EventType::changeWeather:
        {
            return spScriptEventChangeWeather::create(pMap);
        }
        case EventType::changeBuildlist:
        {
            return spScriptEventChangeBuildlist::create(pMap);
        }
        case EventType::modifyTerrain:
        {
            return spScriptEventModifyTerrain::create(pMap);
        }
        case EventType::modifyUnit:
        {
            return spScriptEventModifyUnit::create(pMap);
        }
        case EventType::animation:
        {
            return spScriptEventAnimation::create(pMap);
        }
        case EventType::victoryInfo:
        {
            return spScriptEventVictoryInfo::create(pMap);
        }
        case EventType::modifyVariable:
        {
            return spScriptEventModifyVariable::create(pMap);
        }
        case EventType::ChangeUnitAI:
        {
            return spScriptEventChangeUnitAI::create(pMap);
        }
        case EventType::ChangeBuildingOwner:
        {
            return spScriptEventChangeBuildingOwner::create(pMap);
        }
        case EventType::ChangeUnitOwner:
        {
            return spScriptEventChangeUnitOwner::create(pMap);
        }
        case EventType::ChangePlayerTeam:
        {
            return spScriptEventChangePlayerTeam::create(pMap);
        }
        case EventType::SpawnBuilding:
        {
            return spScriptEventSpawnBuilding::create(pMap);
        }
        case EventType::CenterMap:
        {
            return spScriptEventCenterMap::create(pMap);
        }
        case EventType::PlayGameSound:
        {
            return spScriptEventPlaySound::create(pMap);
        }
    }
    return spScriptEvent();
}
