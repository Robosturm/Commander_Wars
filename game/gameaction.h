#ifndef GAMEACTION_H
#define GAMEACTION_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QBuffer>
#include <QDataStream>

#include "gameinput/menudata.h"
#include "gameinput/markedfielddata.h"
#include "gameinput/cursordata.h"

#include "game/unit.h"
#include "game/building.h"

#include "coreengine/fileserializable.h"
#include "coreengine/jsthis.h"

class GameMap;
class GameAction;
using spGameAction = std::shared_ptr<GameAction>;

class GameAction final : public QObject, public FileSerializable, public JsThis
{
    Q_OBJECT

public:
    enum class InputData
    {
        Int,
        Float,
        String
    };
    static const char* const INPUTSTEP_FIELD;
    static const char* const INPUTSTEP_MENU;

    explicit GameAction(GameMap* pMap);
    explicit GameAction(const QString & actionID, GameMap* pMap);
    ~GameAction() = default;
    /**
     * @brief setTarget sets the target for the current action
     * @param point
     */
    void setTarget(QPoint point);

    /**
     * @brief getActionText the displayed text for this action
     * @param actionID
     * @return
     */
    static QString getActionText(GameMap* pMap, const QString & actionID);
    /**
     * @brief getActionIcon the icon shown for this menue item
     * @param actionID
     * @return
     */
    static QString getActionIcon(GameMap* pMap, const QString & actionID);

    void setTargetUnit(Unit *pTargetUnit);
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 4;
    }
    /**
     * @brief getSyncCounter
     * @return
     */
    qint64 getSyncCounter() const;
    /**
     * @brief setSyncCounter
     * @param syncCounter
     */
    void setSyncCounter(const qint64 &syncCounter);
    /**
     * @brief setSeed
     */
    void setSeed(quint32 seed);
    /**
     * @brief getRoundTimerTime
     * @return
     */
    qint64 getRoundTimerTime() const;
    /**
     * @brief setRoundTimerTime
     * @param roundTimerTime
     */
    void setRoundTimerTime(const qint64 &roundTimerTime);
    /**
     * @brief getStepCursor
     * @return the cursor we want to show during this step. Needs to be deleted by the reciever
     */
    spCursorData getStepCursor();
    /**
     * @brief getMenuStepData
     * @return the data needed to create an input menu. the data needs to be deleted by the caller
     */
    spMenuData getMenuStepData();
    /**
     * @brief getMarkedFieldStepData
     * @return
     */
    spMarkedFieldData getMarkedFieldStepData();
    /**
     * @brief GameAction::setMovepath
     * @param points
     * @param fuelCost
     */
    void setMovepath(const std::vector<QPoint> & points, qint32 fuelCost);
    /**
     * @brief getMapHash
     * @return
     */
    QByteArray getMapHash() const;
    /**
     * @brief setMapHash
     * @param newMapHash
     */
    void setMapHash(const QByteArray & newMapHash);
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap *getMap() const;
    /**
     * @brief getPlayer
     * @return
     */
    Q_INVOKABLE qint32 getPlayer() const;
    /**
     * @brief setPlayer
     * @param player
     */
    Q_INVOKABLE void setPlayer(qint32 player);
    /**
     * @brief getSeed
     * @return
     */
    Q_INVOKABLE quint32 getSeed() const;
    /**
     * @brief setActionID sets the action id for this action
     * @param actionID
     */
    Q_INVOKABLE void setActionID(QString actionID);
    /**
     * @brief getActionID
     * @return
     */
    Q_INVOKABLE QString getActionID();
    /**
     * @brief getInputStep step the ai or player performs for adding info to this action
     * @return
     */
    Q_INVOKABLE qint32 getInputStep() const;
    /**
     * @brief setInputStep updates the input step
     * @param value
     */
    Q_INVOKABLE void setInputStep(const qint32 value);
    /**
     * @brief perform executes this action
     */
    Q_INVOKABLE void perform();
    /**
     * @brief canBePerformed checks if this action can be performed
     * @param x
     * @param y
     * @return
     */
    Q_INVOKABLE bool canBePerformed();
    /**
     * @brief canBePerformed checks if this action can be performed
     * @param actionID id of the action we want to check
     * @return
     */
    Q_INVOKABLE bool canBePerformed(const QString & actionID, bool emptyField = false, Player* pUsingPlayer = nullptr);
    /**
     * @brief isFinalStep
     * @return true if we have all data to perform this action
     */
    Q_INVOKABLE bool isFinalStep();
    /**
     * @brief isFinalStep
     * @param actionID id of the action we want to perform
     * @return true if we have all data to perform this action
     */
    Q_INVOKABLE bool isFinalStep(const QString & actionID);
    /**
     * @brief getTargetUnit the unit that will perform the action
     * @return
     */
    Q_INVOKABLE Unit* getTargetUnit();
    /**
     * @brief getTargetBuilding the building that will perform the action
     * @return
     */
    Q_INVOKABLE Building* getTargetBuilding();
    /**
     * @brief setMovepath sets the movepath for this unit
     * @param points
     */
    Q_INVOKABLE void setMovepath(const QVector<QPoint> points, qint32 fuelCost);
    /**
     * @brief getActionTarget the target action
     * @return
     */
    Q_INVOKABLE QPoint getActionTarget();
    /**
     * @brief getTarget the field or unit with which we want to perform the action
     * @return
     */
    Q_INVOKABLE QPoint getTarget();
    /**
     * @brief getMovePath
     * @return the move path
     */
    Q_INVOKABLE QVector<QPoint> getMovePath();
    /**
     * @brief getMovePathLength
     * @return
     */
    Q_INVOKABLE qint32 getMovePathLength();
    /**
     * @brief getMovementTarget the unit on the final movement field
     * @return
     */
    Q_INVOKABLE Unit* getMovementTarget();
    /**
     * @brief getMovementBuilding the building on the final movement field
     * @return
     */
    Q_INVOKABLE Building* getMovementBuilding();
    /**
     * @brief getMovementTerrain the terrain on the final movement field
     * @return
     */
    Q_INVOKABLE Terrain* getMovementTerrain();
    /**
     * @brief getCosts gets the costs of this actions
     * @return
     */
    Q_INVOKABLE qint32 getCosts() const;
    /**
     * @brief setCosts sets the costs of this actions
     * @param value
     */
    Q_INVOKABLE void setCosts(const qint32 value);
    /**
     * @brief getStepInputType
     * @return the input type used during this step
     */
    Q_INVOKABLE QString getStepInputType();
    /**
     * @brief getRequiresEmptyField
     * @return
     */
    Q_INVOKABLE bool getRequiresEmptyField();
    /**
     * @brief getMenuStepData
     * @return the data needed to create an input menu. the data needs to be deleted by the caller
     */
    Q_INVOKABLE MenuData* getJsMenuStepData();
    /**
     * @brief getMarkedFieldStepData
     * @return
     */
    Q_INVOKABLE MarkedFieldData* getJMarkedFieldStepData();
    /************** reading and writing data to the action buffer *****************/
    /**
     * @brief writeDataString adds a string to the action data
     * @param data
     */
    Q_INVOKABLE void writeDataString(const QString & data)
    {
        m_buffer.seek(m_buffer.size());
        m_actionData << data;
        m_storedDataTypes.push_back(InputData::String);
    }
    /**
     * @brief readDataString
     * @return reads a string from the action data
     */
    Q_INVOKABLE QString readDataString()
    {
        QString data;
        m_actionData >> data;
        return data;
    }
    /**
     * @brief writeDataInt32 adds a int32 to the action data
     * @param data
     */
    Q_INVOKABLE void writeDataInt32(qint32 data)
    {
        m_buffer.seek(m_buffer.size());
        m_actionData << data;
        m_storedDataTypes.push_back(InputData::Int);
    }
    /**
     * @brief readDataInt32
     * @return reads a int32 from the action data
     */
    Q_INVOKABLE qint32 readDataInt32()
    {
        qint32 data;
        m_actionData >> data;
        return data;
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    Q_INVOKABLE void writeDataFloat(float data)
    {
        m_buffer.seek(m_buffer.size());
        m_actionData << data;
        m_storedDataTypes.push_back(InputData::Float);
    }
    /**
     * @brief readDataFloat
     * @return reads a float from the action data
     */
    Q_INVOKABLE float readDataFloat()
    {
        float data;
        m_actionData >> data;
        return data;
    }
    /**
     * @brief startReading starts the reading of the buffer
     */
    Q_INVOKABLE void startReading()
    {
        // go to start again
        m_buffer.seek(0);
    }
    /**
     * @brief getIsLocal
     * @return
     */
    Q_INVOKABLE bool getIsLocal() const;
    /**
     * @brief setIsLocal
     * @param value
     */
    Q_INVOKABLE void setIsLocal(bool value);
    /**
     * @brief getMultiTurnPath
     * @return
     */
    Q_INVOKABLE QVector<QPoint> getMultiTurnPath() const;
    /**
     * @brief setMultiTurnPath
     * @param MultiTurnPath
     */
    Q_INVOKABLE void setMultiTurnPath(const QVector<QPoint> multiTurnPath);
    /**
     * @brief reset
     */
    Q_INVOKABLE void reset();
    /**
     * @brief getPerformingUnit returns the unit performing unit mainly useful in case of post actions where the unit already moved or got loaded etc.
     * @return
     */
    Q_INVOKABLE Unit* getPerformingUnit()
    {
        return m_perfomingUnit.get();
    }
    Q_INVOKABLE qint32 getVariableCount() const
    {
        return m_storedDataTypes.size();
    }
    /**
     * @brief revertLastInputStep
     */
    Q_INVOKABLE void revertLastInputStep(const QString & stepType);
protected:
    void printAction();
private:
    QString m_actionID;
    /**
     * @brief m_target unit, building on which we perfom the action
     */
    QPoint m_target;
    /**
     * @brief m_Movepath the path this unit will go
     */
    QVector<QPoint> m_Movepath;
    /**
      * @brief current input step for tracking when all data is gathered to perform the action
      */
    qint32 m_inputStep{0};
    /**
      * @brief costs needed to be paid to perform this action
      */
    qint32 m_costs{0};
    /**
     * @brief actionData data needed to perform this action
     */
    QBuffer m_buffer;
    QDataStream m_actionData{&m_buffer};
    std::vector<InputData> m_storedDataTypes;

    quint32 m_seed;
    /**
      * needed for ai simulations
      */
    Unit* m_pTargetUnit{nullptr};
    spUnit m_perfomingUnit;
    QVector<QPoint> m_MultiTurnPath;
    bool m_isLocal{false};
    qint64 m_syncCounter{0};
    qint64 m_roundTimerTime{0};
    qint32 m_player{-1};
    GameMap* m_pMap{nullptr};
    QByteArray m_mapHash;
};

Q_DECLARE_INTERFACE(GameAction, "GameAction");

#endif // GAMEACTION_H
