#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>


#include "coreengine/fileserializable.h"

class Userdata : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    Userdata* getInstance();

signals:

public slots:
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream);
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream);
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion()
    {
        return 1;
    }
private:
    explicit Userdata();

    static Userdata* m_pInstance;
};

#endif // USERDATA_H
