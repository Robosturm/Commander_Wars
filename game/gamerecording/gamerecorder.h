#ifndef GAMERECORDER_H
#define GAMERECORDER_H

#include <QObject>
#include <qvector3d.h>

#include "game/gamerecording/daytodayrecord.h"

#include "oxygine-framework.h"

class GameRecorder;
typedef oxygine::intrusive_ptr<GameRecorder> spGameRecorder;

class GameRecorder : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    enum class Rang
    {
        S,
        A,
        B,
        C,
        D
    };

    explicit GameRecorder();
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 3;
    }
    /**
     * @brief calculateRang
     * @param player
     * @param scorePoints
     * @return
     */
    Rang calculateRang(qint32 player, QVector3D& scorePoints);
    /**
     * @brief getRank
     * @param score
     * @return
     */
    Rang getRank(qint32 score);

    DayToDayRecord* getDayRecord(qint32 index)
    {
        if (index >= 0 && index < m_Record.size())
        {
            return m_Record[index].get();
        }
        return nullptr;
    }
signals:

public slots:
    /**
     * @brief newDay
     */
    void newDay();
    /**
     * @brief updatePlayerData
     * @param player
     */
    void updatePlayerData(qint32 player);
    /**
     * @brief addSpecialEvent
     * @param player
     * @param event
     */
    void addSpecialEvent(qint32 player, GameEnums::GameRecord_SpecialEvents event);
    /**
     * @brief lostUnit
     * @param player
     */
    void lostUnit(qint32 player);
    /**
     * @brief getLostUnits
     * @param player
     * @return
     */
    quint32 getLostUnits(qint32 player);
    /**
     * @brief destroyedUnit
     * @param player
     */
    void destroyedUnit(qint32 player);
    /**
     * @brief getDestroyedUnits
     * @param player
     * @return
     */
    quint32 getDestroyedUnits(qint32 player);
    /**
     * @brief buildUnit
     * @param player
     */
    void buildUnit(qint32 player);
    /**
     * @brief getBuildedUnits
     * @param player
     * @return
     */
    quint32 getBuildedUnits(qint32 player);
    /**
     * @brief attacked
     * @param damage
     */
    void attacked(qint32 player, float damage);
    /**
     * @brief getMapTime
     * @return
     */
    qint32 getMapTime() const;
    /**
     * @brief setMapTime
     * @param mapTime
     */
    void setMapTime(const qint32 &mapTime);
    /**
     * @brief getDeployLimit
     * @return
     */
    quint32 getDeployLimit() const;
    /**
     * @brief setDeployLimit
     * @param deployLimit
     */
    void setDeployLimit(const quint32 &deployLimit);
private:
    QVector<spDayToDayRecord> m_Record;

    QVector<quint32> destroyedUnits;
    QVector<quint32> lostUnits;
    QVector<quint32> damageDealt;
    QVector<quint32> attackNumbers;
    QVector<quint32> deployedUnits;

    qint32 m_mapTime{0};
    quint32 m_deployLimit{0};
};

#endif // GAMERECORDER_H
