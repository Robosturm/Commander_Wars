#ifndef SCRIPTEVENT_H
#define SCRIPTEVENT_H

#include <QObject>
#include <QTextStream>

#include "coreengine/LUPDATE_MACROS.h"
#include "coreengine/memorymanagement.h"

class GameMap;
class ScriptEvent;
using spScriptEvent = std::shared_ptr<ScriptEvent>;

class ScriptEditor;
using spScriptEditor = std::shared_ptr<ScriptEditor>;

class ScriptEvent : public QObject, public RefObject<ScriptEvent>
{
    Q_OBJECT
public:
    static const char* const EventDialog;
    static const char* const EventSpawnUnit;
    static const char* const EventDefeatPlayer;
    static const char* const EventChangeBuildlist;
    static const char* const EventAddFunds;
    static const char* const EventChangeWeather;
    static const char* const EventChangeCOBar;
    static const char* const EventModifyUnit;
    static const char* const EventAnimation;
    static const char* const EventModifyTerrain;
    static const char* const EventVictoryInfo;
    static const char* const EventModifyVariable;
    static const char* const EventChangeUnitAI;
    static const char* const EventChangeBuildingOwner;
    static const char* const EventChangeUnitOwner;
    static const char* const EventChangePlayerTeam;
    static const char* const EventSpawnBuilding;
    static const char* const EventCenterMap;
    static const char* const EventPlaySound;
    static const char* const EventBuildingFireCounter;
    static const char* const EventVolcanFire;
    static const char* const EventExtendMap;


    ENUM_CLASS EventType
    {
        dialog,
        spawnUnit,
        defeatPlayer,
        changeBuildlist,
        addFunds,
        changeWeather,
        changeCOBar,
        modifyTerrain,
        modifyUnit,
        animation,
        victoryInfo,
        modifyVariable,
        ChangeUnitAI,
        ChangeBuildingOwner,
        ChangeUnitOwner,
        ChangePlayerTeam,
        SpawnBuilding,
        CenterMap,
        PlayGameSound,
        BuildingFireCounter,
        VolcanFire,
        ExtendMap,
    };

    static spScriptEvent createEvent(GameMap* pMap, EventType type);
    /**
     * @brief createReadEvent
     * @param rStream
     * @return
     */
    static spScriptEvent createReadEvent(GameMap* pMap, QTextStream& rStream, QString line);
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream, QString line) = 0;
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
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() = 0;
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) = 0;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const = 0;
protected:
    explicit ScriptEvent(GameMap* pMap, EventType type);
    EventType m_Type;
    GameMap* m_pMap{nullptr};
};

#endif // SCRIPTEVENT_H
