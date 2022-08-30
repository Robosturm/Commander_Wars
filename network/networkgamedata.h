#ifndef NETWORKGAMEDATA_H
#define NETWORKGAMEDATA_H

#include <QObject>
#include <QJsonObject>

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

#include "coreengine/fileserializable.h"

class NetworkGameData;
using spNetworkGameData = oxygine::intrusive_ptr<NetworkGameData>;

class NetworkGameData : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit NetworkGameData() = default;
    virtual ~NetworkGameData() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    QJsonObject toJson() const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    void fromJson(const QJsonObject & obj);
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline qint32 getVersion() const
    {
        return 0;
    }

    QString getMapName() const;
    void setMapName(const QString &mapName);

    QString getDescription() const;
    void setDescription(const QString &value);

    const QStringList & getMods() const;
    void setMods(const QStringList &Mods);

    qint32 getMaxPlayers() const;
    void setMaxPlayers(const qint32 &maxPlayers);

    qint32 getPlayers() const;
    void setPlayers(const qint32 &players);

    QString getSlaveName() const;
    void setSlaveName(const QString &slaveName);

    bool getLaunched() const;
    void setLaunched(bool launched);

    bool getLocked() const;
    void setLocked(bool locked);

    bool hasOpenPlayers();

    const QString &getSlaveAddress() const;
    void setSlaveAddress(const QString &newAddress);

    const QString &getSlaveSecondaryAddress() const;
    void setSlaveSecondaryAddress(const QString &newSlaveSecondaryAddress);

    quint16 getSlavePort() const;
    void setSlavePort(quint16 newPort);

    const QStringList & getPlayerNames() const;
    void setPlayerNames(const QStringList &playerNames);

private:
    qint32 m_players{0};
    qint32 m_maxPlayers{0};
    QStringList m_Mods;
    QStringList m_playerNames;
    QString m_description;
    QString m_mapName;
    QString m_slaveName;
    bool m_locked{false};
    bool m_launched{false};
    QString m_slaveAddress;
    QString m_slaveSecondaryAddress;
    quint16 m_slavePort{0};
};

#endif // NETWORKGAMEDATA_H
