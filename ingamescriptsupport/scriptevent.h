#ifndef SCRIPTEVENT_H
#define SCRIPTEVENT_H

#include <QObject>

#include "qtextstream.h"

#include "oxygine-framework.h"

class ScriptEvent;
typedef oxygine::intrusive_ptr<ScriptEvent> spScriptEvent;

class ScriptEvent : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static const QString EventDialog;
    static const QString EventSpawnUnit;
    static const QString EventDefeatPlayer;

    enum class EventType
    {
        dialog,
        spawnUnit,
        defeatPlayer,
    };

    static spScriptEvent createEvent(EventType type);
    /**
     * @brief createReadEvent
     * @param rStream
     * @return
     */
    static spScriptEvent createReadEvent(QTextStream& rStream);
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream) = 0;
    /**
     * @brief writeEvent
     * @param rStream
     */
    virtual void writeEvent(QTextStream& rStream) = 0;
    /**
     * @brief getEventType
     * @return
     */
    EventType getEventType()
    {
        return m_Type;
    }
protected:
    explicit ScriptEvent(EventType type);
    EventType m_Type;
};

#endif // SCRIPTEVENT_H
