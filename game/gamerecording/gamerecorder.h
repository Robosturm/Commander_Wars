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
        return 1;
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
     * @brief destroyedUnit
     * @param player
     */
    void destroyedUnit(qint32 player);
    /**
     * @brief buildUnit
     * @param player
     */
    void buildUnit(qint32 player);
    /**
     * @brief attacked
     * @param damage
     */
    void attacked(qint32 player, float damage);
private:
    QVector<spDayToDayRecord> m_Record;

    QVector<quint64> destroyedUnits;
    QVector<quint64> lostUnits;
    QVector<quint64> damageDealt;
    QVector<quint64> attackNumbers;
    QVector<quint64> deployedUnits;
};

#endif // GAMERECORDER_H
