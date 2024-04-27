#pragma once

#include <QString>
#include "coreengine/fileserializable.h"

class GameVersion : public FileSerializable
{
public:
    GameVersion();
    GameVersion(qint32 major, qint32 minor, qint32 revision, QString sufix);
    ~GameVersion() = default;


    QString toString() const;

    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& stream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& stream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
    qint32 getMajor() const;
    void setMajor(qint32 newMajor);

    qint32 getMinor() const;
    void setMinor(qint32 newMinor);

    qint32 getRevision() const;
    void setRevision(qint32 newRevision);

    void setSufix(const QString & newSufix);
    QString getSufix() const;

    bool operator>=(const GameVersion& other) const;
    bool operator==(const GameVersion& other) const;
    bool operator!=(const GameVersion& other) const;
private:

    qint32 m_major = VERSION_MAJOR;
    qint32 m_minor = VERSION_MINOR;
    qint32 m_revision = VERSION_REVISION;
    QString m_sufix;
};

