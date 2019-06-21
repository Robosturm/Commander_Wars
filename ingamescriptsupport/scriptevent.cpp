#include "scriptevent.h"

#include "scripteventdialog.h"

const QString ScriptEvent::EventDialog  = "EventDialog";

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
    }
    return nullptr;
}
