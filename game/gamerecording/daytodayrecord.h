#ifndef DAYTODAYRECORD_H
#define DAYTODAYRECORD_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/fileserializable.h"

#include "game/gamerecording/playerrecord.h"
#include "game/gamerecording/specialevent.h"

class GameMap;
class DayToDayRecord;
using spDayToDayRecord = oxygine::intrusive_ptr<DayToDayRecord>;

class DayToDayRecord : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit DayToDayRecord(GameMap* pMap);
    explicit DayToDayRecord(qint32 playerCount);
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
    void addSpecialEvent(qint32 player, qint32 day, GameEnums::GameRecord_SpecialEvents event);
    /**
     * @brief addPlayerRecord
     * @param player
     */
    void addPlayerRecord(qint32 player, qint32 day);
    /**
     * @brief getPlayerRecord
     * @param player
     * @return
     */
    PlayerRecord* getPlayerRecord(qint32 player);
    qint32 getEventRecordCount()
    {
        return m_SpecialEvents.size();
    }
    SpecialEvent* getSpecialEvent(qint32 index);

private:
    QVector<spSpecialEvent> m_SpecialEvents;
    QVector<spPlayerRecord> m_PlayerRecords;
    GameMap* m_pMap{nullptr};
};

#endif // DAYTODAYRECORD_H
