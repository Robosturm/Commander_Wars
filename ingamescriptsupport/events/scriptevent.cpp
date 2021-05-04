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


ScriptEvent::ScriptEvent(EventType type)
    : QObject(),
      m_Type(type)
{
}

spScriptEvent ScriptEvent::createReadEvent(QTextStream& rStream)
{
    qint64 pos = rStream.pos();
    QString line = rStream.readLine().simplified();
    rStream.seek(pos);
    spScriptEvent ret = nullptr;
    if (line.endsWith(EventDialog))
    {
         ret = spScriptEventDialog::create();
    }
    else if (line.endsWith(EventSpawnUnit))
    {
         ret = spScriptEventSpawnUnit::create();
    }
    else if (line.endsWith(EventDefeatPlayer))
    {
         ret = spScriptEventDefeatPlayer::create();
    }
    else if (line.endsWith(EventAddFunds))
    {
         ret = spScriptEventAddFunds::create();
    }
    else if (line.endsWith(EventChangeBuildlist))
    {
         ret = spScriptEventChangeBuildlist::create();
    }
    else if (line.endsWith(EventChangeWeather))
    {
         ret = spScriptEventChangeWeather::create();
    }
    else if (line.endsWith(EventChangeCOBar))
    {
         ret = spScriptEventChangeCOBar::create();
    }
    else if (line.endsWith(EventModifyTerrain))
    {
         ret = spScriptEventModifyTerrain::create();
    }
    else if (line.endsWith(EventModifyUnit))
    {
         ret = spScriptEventModifyUnit::create();
    }
    else if (line.endsWith(EventAnimation))
    {
         ret = spScriptEventAnimation::create();
    }
    else if (line.endsWith(EventVictoryInfo))
    {
         ret = spScriptEventVictoryInfo::create();
    }
    else if (line.endsWith(EventModifyVariable))
    {
         ret = spScriptEventModifyVariable::create();
    }
    else if (line.endsWith(EventChangeUnitAI))
    {
        ret = spScriptEventChangeUnitAI::create();
    }
    else if (line.endsWith(EventChangeBuildingOwner))
    {
        ret = spScriptEventChangeBuildingOwner::create();
    }
    else if (line.endsWith(EventChangeUnitOwner))
    {
        ret = spScriptEventChangeUnitOwner::create();
    }
    else if (line.endsWith(EventChangePlayerTeam))
    {
        ret = spScriptEventChangePlayerTeam::create();
    }
    else if (line.endsWith(EventSpawnBuilding))
    {
        ret = spScriptEventSpawnBuilding::create();
    }
    else if (line.endsWith(EventCenterMap))
    {
        ret = spScriptEventCenterMap::create();
    }
    else if (line.endsWith(EventPlaySound))
    {
        ret = spScriptEventPlaySound::create();
    }
    if (ret.get() != nullptr)
    {
        ret->readEvent(rStream);
    }
    else
    {
        // skip line
        rStream.readLine();
    }
    return ret;
}

spScriptEvent ScriptEvent::createEvent(EventType type)
{
    switch (type)
    {
        case EventType::dialog:
        {
            return spScriptEventDialog::create();
        }
        case EventType::spawnUnit:
        {
            return spScriptEventSpawnUnit::create();
        }
        case EventType::defeatPlayer:
        {
            return spScriptEventDefeatPlayer::create();
        }
        case EventType::addFunds:
        {
            return spScriptEventAddFunds::create();
        }
        case EventType::changeCOBar:
        {
            return spScriptEventChangeCOBar::create();
        }
        case EventType::changeWeather:
        {
            return spScriptEventChangeWeather::create();
        }
        case EventType::changeBuildlist:
        {
            return spScriptEventChangeBuildlist::create();
        }
        case EventType::modifyTerrain:
        {
            return spScriptEventModifyTerrain::create();
        }
        case EventType::modifyUnit:
        {
            return spScriptEventModifyUnit::create();
        }
        case EventType::animation:
        {
            return spScriptEventAnimation::create();
        }
        case EventType::victoryInfo:
        {
            return spScriptEventVictoryInfo::create();
        }
        case EventType::modifyVariable:
        {
            return spScriptEventModifyVariable::create();
        }
        case EventType::ChangeUnitAI:
        {
            return spScriptEventChangeUnitAI::create();
        }
        case EventType::ChangeBuildingOwner:
        {
            return spScriptEventChangeBuildingOwner::create();
        }
        case EventType::ChangeUnitOwner:
        {
            return spScriptEventChangeUnitOwner::create();
        }
        case EventType::ChangePlayerTeam:
        {
            return spScriptEventChangePlayerTeam::create();
        }
        case EventType::SpawnBuilding:
        {
            return spScriptEventSpawnBuilding::create();
        }
        case EventType::CenterMap:
        {
            return spScriptEventCenterMap::create();
        }
        case EventType::PlayGameSound:
        {
            return spScriptEventPlaySound::create();
        }
    }
    return nullptr;
}
