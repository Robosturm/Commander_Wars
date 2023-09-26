#ifndef PLAYERRECORD_H
#define PLAYERRECORD_H

#include <QObject>
#include <QVector>

#include "coreengine/fileserializable.h"

class PlayerRecord;
using spPlayerRecord = std::shared_ptr<PlayerRecord>;

class PlayerRecord final : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit PlayerRecord();
    explicit PlayerRecord(qint32 day, qint32 player, qint32 funds, qint32 income,
                          qint32 buildings, qint32 units, qint32 playerStrength);
    virtual ~PlayerRecord() = default;
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
    Q_INVOKABLE qint32 getDay() const;
    Q_INVOKABLE qint32 getOwner() const;
    Q_INVOKABLE qint32 getFunds() const;
    Q_INVOKABLE qint32 getIncome() const;
    Q_INVOKABLE qint32 getBuildings() const;
    Q_INVOKABLE qint32 getUnits() const;
    Q_INVOKABLE qint32 getPlayerStrength() const;
private:
    qint32 m_Day{-1};
    qint32 m_Player{-1};
    qint32 m_Funds{-1};
    qint32 m_Income{-1};
    qint32 m_Buildings{-1};
    qint32 m_Units{-1};
    qint32 m_PlayerStrength{-1};
};

Q_DECLARE_INTERFACE(PlayerRecord, "PlayerRecord");

#endif // PLAYERRECORD_H
