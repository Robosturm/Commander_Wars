#ifndef NETWORKGAMEDATA_H
#define NETWORKGAMEDATA_H

#include <QObject>
#include <QJsonObject>

/**
 * @brief The NetworkGameData class Data describing this game
 */
class NetworkGameData final
{
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

    qint64 getUuid() const;
    void setUuid(qint64 newUuid);

    bool getRunningGame() const;
    void setRunningGame(bool newRunningGame);

    QString getCurrentPlayer() const;
    void setCurrentPlayer(const QString & newCurrentPlayer);

    QVector<bool> getOnlineData() const;
    void setOnlineData(const QVector<bool> & newOnlineData);

    QByteArray & getMinimapData();
    void setMinimapData(const QByteArray & newMinimapData);

    qint32 getObservers() const;
    void setObservers(qint32 newObservers);

    qint32 getMaxObservers() const;
    void setMaxObservers(qint32 newMaxObservers);

private:
    qint32 m_players{0};
    qint32 m_maxPlayers{0};
    QStringList m_Mods;
    QStringList m_playerNames;
    QString m_description;
    QString m_mapName;
    QString m_slaveName;
    QString m_currentPlayer;
    bool m_locked{false};
    bool m_launched{false};
    bool m_runningGame{false};
    QString m_slaveAddress;
    QString m_slaveSecondaryAddress;
    quint16 m_slavePort{0};
    qint64 m_uuid{0};
    QVector<bool> m_onlineData;
    QByteArray m_minimapData;
    qint32 m_observers{0};
    qint32 m_maxObservers{0};
};

#endif // NETWORKGAMEDATA_H
