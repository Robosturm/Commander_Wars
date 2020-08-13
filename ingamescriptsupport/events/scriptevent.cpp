#include "scriptevent.h"

#include "scripteventdialog.h"
#include "scripteventspawnunit.h"
#include "scripteventdefeatplayer.h"
#include "scripteventchangebuildlist.h"
#include "scripteventaddfunds.h"
#include "scripteventchangecobar.h"
#include "scripteventchangeweather.h"
#include "scripteventmodifyterrain.h"
#include "scripteventmodifyunit.h"
#include "scripteventanimation.h"
#include "scripteventvictoryinfo.h"
#include "scripteventmodifyvariable.h"
#include "scripteventchangeunitai.h"
#include "scripteventchangeunitowner.h"
#include "scripteventchangebuildingowner.h"
#include "scripteventchangeplayerteam.h"
#include "scripteventspawnbuilding.h"
#include "scripteventcentermap.h"
#include "scripteventplaysound.h"

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
    ScriptEvent* ret = nullptr;
    if (line.endsWith(EventDialog))
    {
         ret = new ScriptEventDialog();
    }
    else if (line.endsWith(EventSpawnUnit))
    {
         ret = new ScriptEventSpawnUnit();
    }
    else if (line.endsWith(EventDefeatPlayer))
    {
         ret = new ScriptEventDefeatPlayer();
    }
    else if (line.endsWith(EventAddFunds))
    {
         ret = new ScriptEventAddFunds();
    }
    else if (line.endsWith(EventChangeBuildlist))
    {
         ret = new ScriptEventChangeBuildlist();
    }
    else if (line.endsWith(EventChangeWeather))
    {
         ret = new ScriptEventChangeWeather();
    }
    else if (line.endsWith(EventChangeCOBar))
    {
         ret = new ScriptEventChangeCOBar();
    }
    else if (line.endsWith(EventModifyTerrain))
    {
         ret = new ScriptEventModifyTerrain();
    }
    else if (line.endsWith(EventModifyUnit))
    {
         ret = new ScriptEventModifyUnit();
    }
    else if (line.endsWith(EventAnimation))
    {
         ret = new ScriptEventAnimation();
    }
    else if (line.endsWith(EventVictoryInfo))
    {
         ret = new ScriptEventVictoryInfo();
    }
    else if (line.endsWith(EventModifyVariable))
    {
         ret = new ScriptEventModifyVariable();
    }
    else if (line.endsWith(EventChangeUnitAI))
    {
        ret = new ScriptEventChangeUnitAI();
    }
    else if (line.endsWith(EventChangeBuildingOwner))
    {
        ret = new ScriptEventChangeBuildingOwner();
    }
    else if (line.endsWith(EventChangeUnitOwner))
    {
        ret = new ScriptEventChangeUnitOwner();
    }
    else if (line.endsWith(EventChangePlayerTeam))
    {
        ret = new ScriptEventChangePlayerTeam();
    }
    else if (line.endsWith(EventSpawnBuilding))
    {
        ret = new ScriptEventSpawnBuilding();
    }
    else if (line.endsWith(EventCenterMap))
    {
        ret = new ScriptEventCenterMap();
    }
    else if (line.endsWith(EventPlaySound))
    {
        ret = new ScriptEventPlaySound();
    }
    if (ret != nullptr)
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
            return new ScriptEventDialog();
        }
        case EventType::spawnUnit:
        {
            return new ScriptEventSpawnUnit();
        }
        case EventType::defeatPlayer:
        {
            return new ScriptEventDefeatPlayer();
        }
        case EventType::addFunds:
        {
            return new ScriptEventAddFunds();
        }
        case EventType::changeCOBar:
        {
            return new ScriptEventChangeCOBar();
        }
        case EventType::changeWeather:
        {
            return new ScriptEventChangeWeather();
        }
        case EventType::changeBuildlist:
        {
            return new ScriptEventChangeBuildlist();
        }
        case EventType::modifyTerrain:
        {
            return new ScriptEventModifyTerrain();
        }
        case EventType::modifyUnit:
        {
            return new ScriptEventModifyUnit();
        }
        case EventType::animation:
        {
            return new ScriptEventAnimation();
        }
        case EventType::victoryInfo:
        {
            return new ScriptEventVictoryInfo();
        }
        case EventType::modifyVariable:
        {
            return new ScriptEventModifyVariable();
        }
        case EventType::ChangeUnitAI:
        {
            return new ScriptEventChangeUnitAI();
        }
        case EventType::ChangeBuildingOwner:
        {
            return new ScriptEventChangeBuildingOwner();
        }
        case EventType::ChangeUnitOwner:
        {
            return new ScriptEventChangeUnitOwner();
        }
        case EventType::ChangePlayerTeam:
        {
            return new ScriptEventChangePlayerTeam();
        }
        case EventType::SpawnBuilding:
        {
            return new ScriptEventSpawnBuilding();
        }
        case EventType::CenterMap:
        {
            return new ScriptEventCenterMap();
        }
        case EventType::PlayGameSound:
        {
            return new ScriptEventPlaySound();
        }
    }
    return nullptr;
}
