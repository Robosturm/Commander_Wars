#ifndef SCRIPTEVENT_H
#define SCRIPTEVENT_H

#include <QObject>
#include <QTextStream>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/LUPDATE_MACROS.h"

class ScriptEvent;
typedef oxygine::intrusive_ptr<ScriptEvent> spScriptEvent;

class ScriptEditor;
typedef oxygine::intrusive_ptr<ScriptEditor> spScriptEditor;

class ScriptEvent : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static const QString EventDialog;
    static const QString EventSpawnUnit;
    static const QString EventDefeatPlayer;
    static const QString EventChangeBuildlist;
    static const QString EventAddFunds;
    static const QString EventChangeWeather;
    static const QString EventChangeCOBar;
    static const QString EventModifyUnit;
    static const QString EventAnimation;
    static const QString EventModifyTerrain;
    static const QString EventVictoryInfo;
    static const QString EventModifyVariable;
    static const QString EventChangeUnitAI;
    static const QString EventChangeBuildingOwner;
    static const QString EventChangeUnitOwner;
    static const QString EventChangePlayerTeam;
    static const QString EventSpawnBuilding;
    static const QString EventCenterMap;
    static const QString EventPlaySound;


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
    explicit ScriptEvent(EventType type);
    EventType m_Type;
};

#endif // SCRIPTEVENT_H
