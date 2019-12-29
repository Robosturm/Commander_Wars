#ifndef PLAYERRECORD_H
#define PLAYERRECORD_H

#include <QObject>

#include <qvector.h>

#include "coreengine/fileserializable.h"

#include "oxygine-framework.h"

class PlayerRecord;
typedef oxygine::intrusive_ptr<PlayerRecord> spPlayerRecord;


class PlayerRecord : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    /**
     * @brief The AttackReport struct
     */
    struct AttackReport
    {
        /**
         * @brief attackerDamage
         */
        qint32 attackerDamage{0};
        /**
         * @brief attackerX
         */
        qint32  attackerX{-1};
        /**
         * @brief attackerY
         */
        qint32  attackerY{-1};
        /**
         * @brief attackerID
         */
        QString attackerID;
        /**
         * @brief pAttackerOwner
         */
        qint32 attackerOwnerID{-1};
        /**
         * @brief defenderX
         */
        qint32  defenderX{-1};
        /**
         * @brief defenderY
         */
        qint32  defenderY{-1};
        /**
         * @brief defenderDamage
         */
        qint32 defenderDamage{0};
        /**
         * @brief defenderID
         */
        QString defenderID;
    };

    explicit PlayerRecord();
    explicit PlayerRecord(qint32 day, qint32 player, qint32 funds, qint32 income,
                          qint32 buildings, qint32 units, qint32 playerStrength);

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
        return 2;
    }


    qint32 getDay() const;
    qint32 getOwner() const;
    qint32 getFunds() const;
    qint32 getIncome() const;
    qint32 getBuildings() const;
    qint32 getUnits() const;
    qint32 getPlayerStrength() const;
private:
    qint32 m_Day{-1};
    qint32 m_Player{-1};
    qint32 m_Funds{-1};
    qint32 m_Income{-1};
    qint32 m_Buildings{-1};
    qint32 m_Units{-1};
    qint32 m_PlayerStrength{-1};
    QVector<AttackReport> m_Attackreports;
};

#endif // PLAYERRECORD_H
