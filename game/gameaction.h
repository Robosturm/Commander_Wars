#ifndef GAMEACTION_H
#define GAMEACTION_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QBuffer>
#include <QDataStream>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "gameinput/menudata.h"
#include "gameinput/markedfielddata.h"
#include "gameinput/cursordata.h"

#include "game/unit.h"
#include "game/building.h"

#include "coreengine/fileserializable.h"

class GameMap;
class GameAction;
using spGameAction = oxygine::intrusive_ptr<GameAction>;

class GameAction : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT

public:
    explicit GameAction(GameMap* pMap);
    explicit GameAction(const QString & actionID, GameMap* pMap);
    virtual ~GameAction() = default;
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
        return 3;
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

public slots:
    /**
     * @brief getMap
     * @return
     */
    GameMap *getMap() const;
    /**
     * @brief getPlayer
     * @return
     */
    qint32 getPlayer() const;
    /**
     * @brief setPlayer
     * @param player
     */
    void setPlayer(qint32 player);
    /**
     * @brief getSeed
     * @return
     */
    quint32 getSeed() const;
    /**
     * @brief setActionID sets the action id for this action
     * @param actionID
     */
    void setActionID(QString actionID);
    /**
     * @brief getActionID
     * @return
     */
    QString getActionID();
    /**
     * @brief getInputStep step the ai or player performs for adding info to this action
     * @return
     */
    qint32 getInputStep() const;
    /**
     * @brief setInputStep updates the input step
     * @param value
     */
    void setInputStep(const qint32 &value);
    /**
     * @brief perform executes this action
     */
    void perform();
    /**
     * @brief canBePerformed checks if this action can be performed
     * @param x
     * @param y
     * @return
     */
    bool canBePerformed();
    /**
     * @brief canBePerformed checks if this action can be performed
     * @param actionID id of the action we want to check
     * @return
     */
    bool canBePerformed(const QString & actionID, bool emptyField = false, Player* pUsingPlayer = nullptr);
    /**
     * @brief isFinalStep
     * @return true if we have all data to perform this action
     */
    bool isFinalStep();
    /**
     * @brief isFinalStep
     * @param actionID id of the action we want to perform
     * @return true if we have all data to perform this action
     */
    bool isFinalStep(const QString & actionID);
    /**
     * @brief getTargetUnit the unit that will perform the action
     * @return
     */
    Unit* getTargetUnit();
    /**
     * @brief getTargetBuilding the building that will perform the action
     * @return
     */
    Building* getTargetBuilding();
    /**
     * @brief setMovepath sets the movepath for this unit
     * @param points
     */
    void setMovepath(const QVector<QPoint> & points, qint32 fuelCost);
    /**
     * @brief getActionTarget the target action
     * @return
     */
    QPoint getActionTarget();
    /**
     * @brief getTarget the field or unit with which we want to perform the action
     * @return
     */
    QPoint getTarget();
    /**
     * @brief getMovePath
     * @return the move path
     */
    QVector<QPoint> getMovePath();
    /**
     * @brief getMovePathLength
     * @return
     */
    qint32 getMovePathLength();
    /**
     * @brief getMovementTarget the unit on the final movement field
     * @return
     */
    Unit* getMovementTarget();
    /**
     * @brief getMovementBuilding the building on the final movement field
     * @return
     */
    Building* getMovementBuilding();
    /**
     * @brief getMovementTerrain the terrain on the final movement field
     * @return
     */
    Terrain* getMovementTerrain();
    /**
     * @brief getCosts gets the costs of this actions
     * @return
     */
    qint32 getCosts() const;
    /**
     * @brief setCosts sets the costs of this actions
     * @param value
     */
    void setCosts(const qint32 &value);
    /**
     * @brief getStepInputType
     * @return the input type used during this step
     */
    QString getStepInputType();
    /**
     * @brief getRequiresEmptyField
     * @return
     */
    bool getRequiresEmptyField();
    /************** reading and writing data to the action buffer *****************/
    /**
     * @brief writeDataString adds a string to the action data
     * @param data
     */
    void writeDataString(const QString & data)
    {
        m_buffer.seek(m_buffer.size());
        m_actionData << data;
        ++m_variableCount;
    }
    /**
     * @brief readDataString
     * @return reads a string from the action data
     */
    QString readDataString()
    {
        QString data;
        m_actionData >> data;
        return data;
    }
    /**
     * @brief writeDataInt32 adds a int32 to the action data
     * @param data
     */
    void writeDataInt32(qint32 data)
    {
        m_buffer.seek(m_buffer.size());
        m_actionData << data;
        ++m_variableCount;
    }
    /**
     * @brief readDataInt32
     * @return reads a int32 from the action data
     */
    qint32 readDataInt32()
    {
        qint32 data;
        m_actionData >> data;
        return data;
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    void writeDataFloat(float data)
    {
        m_buffer.seek(m_buffer.size());
        m_actionData << data;
        ++m_variableCount;
    }
    /**
     * @brief readDataFloat
     * @return reads a float from the action data
     */
    float readDataFloat()
    {
        float data;
        m_actionData >> data;
        return data;
    }
    /**
     * @brief startReading starts the reading of the buffer
     */
    void startReading()
    {
        // go to start again
        m_buffer.seek(0);
    }
    /**
     * @brief getIsLocal
     * @return
     */
    bool getIsLocal() const;
    /**
     * @brief setIsLocal
     * @param value
     */
    void setIsLocal(bool value);
    /**
     * @brief getMultiTurnPath
     * @return
     */
    QVector<QPoint> getMultiTurnPath() const;
    /**
     * @brief setMultiTurnPath
     * @param MultiTurnPath
     */
    void setMultiTurnPath(const QVector<QPoint> &MultiTurnPath);
    /**
     * @brief reset
     */
    void reset();
    /**
     * @brief getPerformingUnit returns the unit performing unit mainly useful in case of post actions where the unit already moved or got loaded etc.
     * @return
     */
    Unit* getPerformingUnit()
    {
        return m_perfomingUnit.get();
    }
    qint32 getVariableCount() const
    {
        return m_variableCount;
    }
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
    qint32 m_variableCount{0};
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(GameAction, "GameAction");

#endif // GAMEACTION_H
