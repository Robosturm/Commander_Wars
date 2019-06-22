#include "scriptevent.h"

#include "scripteventdialog.h"
#include "scripteventspawnunit.h"
#include "scripteventdefeatplayer.h"

const QString ScriptEvent::EventDialog  = "Dialog";
const QString ScriptEvent::EventSpawnUnit = "Spawn Unit";
const QString ScriptEvent::EventDefeatPlayer = "Defeat Player";

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
    if (line.endsWith(EventDefeatPlayer))
    {
         ret = new ScriptEventDefeatPlayer();
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
    }
    return nullptr;
}
