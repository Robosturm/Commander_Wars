#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <QObject>

#include "coreengine/fileserializable.h"

class GameData : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit GameData();

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
        return 1;
    }
signals:

public slots:

private:
    QString m_Host;
    QString m_MapName;
    qint32 m_Player{0};
    qint32 m_MaxPlayer{0};
    QString m_password;
    QString m_Version;
    QStringList m_modData;
};

#endif // GAMEDATA_H
