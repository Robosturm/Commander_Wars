#ifndef DAYTODAYRECORD_H
#define DAYTODAYRECORD_H

#include <QObject>
#include <QVector>

#include "coreengine/fileserializable.h"

#include "game/gamerecording/playerrecord.h"
#include "game/gamerecording/specialevent.h"

class GameMap;
class DayToDayRecord;
using spDayToDayRecord = std::shared_ptr<DayToDayRecord>;

class DayToDayRecord final : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit DayToDayRecord(GameMap* pMap);
    explicit DayToDayRecord(GameMap* pMap, qint32 playerCount);
    virtual ~DayToDayRecord() = default;
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
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 1;
    }
    /**
     * @brief addSpecialEvent
     * @param player
     * @param day
     * @param event
     */
    Q_INVOKABLE void addSpecialEvent(qint32 player, qint32 day, GameEnums::GameRecord_SpecialEvents event);
    /**
     * @brief addPlayerRecord
     * @param player
     */
    Q_INVOKABLE void addPlayerRecord(qint32 player, qint32 day);
    /**
     * @brief addPlayerRecord
     * @param day
     * @param player
     * @param funds
     * @param income
     * @param buildings
     * @param units
     * @param playerStrength
     */
    Q_INVOKABLE void addPlayerRecord(qint32 day, qint32 player, qint32 funds, qint32 income,
                                     qint32 buildings, qint32 units, qint32 playerStrength);
    /**
     * @brief getPlayerRecord
     * @param player
     * @return
     */
    Q_INVOKABLE PlayerRecord* getPlayerRecord(qint32 player);
    Q_INVOKABLE qint32 getEventRecordCount()
    {
        return m_SpecialEvents.size();
    }
    Q_INVOKABLE SpecialEvent* getSpecialEvent(qint32 index);

private:
    QVector<spSpecialEvent> m_SpecialEvents;
    QVector<spPlayerRecord> m_PlayerRecords;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(DayToDayRecord, "DayToDayRecord");

#endif // DAYTODAYRECORD_H
