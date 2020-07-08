#ifndef NETWORKGAMEDATA_H
#define NETWORKGAMEDATA_H

#include <QObject>

#include "coreengine/fileserializable.h"
#include "oxygine-framework.h"

class NetworkGameData;
typedef oxygine::intrusive_ptr<NetworkGameData> spNetworkGameData;

class NetworkGameData : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit NetworkGameData();
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
        return 0;
    }
    quint16 getGamePort() const;
    void setGamePort(const quint16 &gamePort);

    QString getMapName() const;
    void setMapName(const QString &mapName);

    QString getDescription() const;
    void setDescription(const QString &value);

    QVector<QString> getMods() const;
    void setMods(const QVector<QString> &Mods);

    qint32 getMaxPlayers() const;
    void setMaxPlayers(const qint32 &maxPlayers);

    qint32 getPlayers() const;
    void setPlayers(const qint32 &players);

signals:

private:
    qint32 _players{0};
    qint32 _maxPlayers{0};
    QVector<QString> _Mods;
    QString description;
    QString _mapName;
    quint16 _gamePort{0};
};

#endif // NETWORKGAMEDATA_H
