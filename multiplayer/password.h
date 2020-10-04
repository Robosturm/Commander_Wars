#ifndef PASSWORD_H
#define PASSWORD_H

#include <QObject>
#include "coreengine/fileserializable.h"

/**
 * @brief The Password class for transferring a password as cryptic hash and checking if a given password matches it.
 */
class Password : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit Password();
    explicit Password(QString password);

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
    /**
     * @brief setPassword
     * @param password
     * @return
     */
    void setPassword(const Password & password);
public slots:
    void setPassword(QString password);
    bool isValidPassword(QString password) const;
    bool areEqualPassword(const Password & password) const;
private:
    QByteArray m_passwordHash;
};

#endif // PASSWORD_H
