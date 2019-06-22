#ifndef SCRIPTCONDITION_H
#define SCRIPTCONDITION_H

#include <QObject>

#include <QVector>

#include <QTextStream>

#include "oxygine-framework.h"

#include "ingamescriptsupport/scriptevent.h"

class ScriptCondition;
typedef oxygine::intrusive_ptr<ScriptCondition> spScriptCondition;

class ScriptEditor;
typedef oxygine::intrusive_ptr<ScriptEditor> spScriptEditor;

class ScriptCondition : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    enum class ConditionType
    {
        startOfTurn,
        victory,
        eachDay
    };
    static const QString ConditionVictory;
    static const QString ConditionStartOfTurn;
    static const QString ConditionEachDay;
    /**
     * @brief createCondition
     * @param type
     * @return
     */
    static ScriptCondition* createCondition(ConditionType type);
    /**
     * @brief createReadCondition
     * @param rStream
     * @return
     */
    static ScriptCondition* createReadCondition(QTextStream& rStream);
    /**
     * @brief readCondition
     * @param rStream
     */
    virtual void readCondition(QTextStream& rStream) = 0;
    /**
     * @brief writeCondition
     * @param rStream
     */
    virtual void writeCondition(QTextStream& rStream) = 0;
    /**
     * @brief addEvent
     * @param event
     */
    void addEvent(spScriptEvent event);
    /**
     * @brief getEvent
     * @param index
     * @return
     */
    spScriptEvent getEvent(qint32 index);
    /**
     * @brief getEventSize
     * @return
     */
    qint32 getEventSize()
    {
        return events.size();
    }
    /**
     * @brief removeEvent
     * @param pEvent
     */
    void removeEvent(spScriptEvent pEvent);
    /**
     * @brief getType
     * @return
     */
    ConditionType getType() const;
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() = 0;
    /**
     * @brief showEditCondition
     */
    virtual void showEditCondition(spScriptEditor pScriptEditor) = 0;
protected:
    explicit ScriptCondition(ConditionType type);
    QVector<spScriptEvent> events;

    ConditionType m_Type;
};

#endif // SCRIPTCONDITION_H
