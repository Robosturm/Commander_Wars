#ifndef SCRIPTCONDITION_H
#define SCRIPTCONDITION_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "ingamescriptsupport/events/scriptevent.h"

#include "coreengine/LUPDATE_MACROS.h"

class GameMap;

class ScriptCondition;
using spScriptCondition = std::shared_ptr<ScriptCondition>;

class ScriptEditor;
using spScriptEditor = std::shared_ptr<ScriptEditor>;

class ScriptCondition : public QObject, public RefObject<ScriptCondition>
{
    Q_OBJECT
public:
    ENUM_CLASS ConditionType
    {
        startOfTurn,
        victory,
        eachDay,
        unitDestroyed,
        buildingDestroyed,
        terrainDestroyed,
        buildingCaptured,
        playerDefeated,
        unitsDestroyed,
        buildingsOwned,
        playerReachedArea,
        unitReachedArea,
        checkVariable,
        isCo,
    };
    static const char* const ConditionVictory;
    static const char* const ConditionStartOfTurn;
    static const char* const ConditionEachDay;
    static const char* const ConditionUnitDestroyed;
    static const char* const ConditionBuildingDestroyed;
    static const char* const ConditionTerrainDestroyed;
    static const char* const ConditionBuildingCaptured;
    static const char* const ConditionPlayerDefeated;
    static const char* const ConditionUnitsDestroyed;
    static const char* const ConditionBuildingsOwned;
    static const char* const ConditionPlayerReachedArea;
    static const char* const ConditionUnitReachedArea;
    static const char* const ConditionCheckVariable;
    static const char* const ConditionIsCo;

    /**
     * @brief createCondition
     * @param type
     * @return
     */
    static spScriptCondition createCondition(GameMap* pMap, ConditionType type);
    /**
     * @brief createReadCondition
     * @param rStream
     * @return
     */
    static spScriptCondition createReadCondition(GameMap* pMap, QTextStream& rStream, QString & line);
    /**
     * @brief readCondition
     * @param rStream
     */
    virtual void readCondition(QTextStream& rStream, QString line) = 0;
    /**
     * @brief writeCondition
     * @param rStream
     */
    virtual void writeCondition(QTextStream& rStream) = 0;
    /**
     * @brief writePreCondition
     * @param rStream
     */
    virtual void writePreCondition(QTextStream& rStream);
    /**
     * @brief writePostCondition
     * @param rStream
     */
    virtual void writePostCondition(QTextStream& rStream);
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
    /**
     * @brief getSubCondition
     * @return
     */
    spScriptCondition getSubCondition() const;
    /**
     * @brief setSubCondition
     * @param value
     */
    void setSubCondition(const spScriptCondition &value);
    /**
     * @brief sameConditionGroup
     * @param type1
     * @param type2
     * @return
     */
    static bool sameConditionGroup(ConditionType type1, ConditionType type2);
    /**
     * @brief getParent
     * @return
     */
    ScriptCondition *getParent() const;
    /**
     * @brief setParent
     * @param value
     */
    void setParent(ScriptCondition *value);
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const = 0;
    /**
     * @brief readSubCondition
     * @param rStream
     * @param id
     * @return
     */
    bool readSubCondition(GameMap* pMap, QTextStream& rStream, QString id, QString & line);
protected:
    explicit ScriptCondition(GameMap* pMap, ConditionType type);
    QVector<spScriptEvent> events;
    spScriptCondition subCondition;
    ScriptCondition* pParent{nullptr};

    ConditionType m_Type;
    GameMap* m_pMap{nullptr};
};

#endif // SCRIPTCONDITION_H
