#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>

#include <qimage.h>

#include <qvector.h>


#include "coreengine/fileserializable.h"

class Userdata : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    static Userdata* getInstance();
    void changeUser();
    void storeUser();
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
    /**
     * @brief addCOStyle
     * @param coid
     * @param file
     * @param colorTable
     * @param maskTable
     */
    void addCOStyle(QString coid, QString file, QImage colorTable, QImage maskTable);
    /**
     * @brief removeCOStyle
     * @param coid
     */
    void removeCOStyle(QString coid);
    /**
     * @brief getCOStyle
     * @param coid
     * @return
     */
    std::tuple<QString, QString, QImage, QImage>* getCOStyle(QString coid);
private:
    explicit Userdata();

    static Userdata* m_pInstance;

    QVector<std::tuple<QString, QString, QImage, QImage>> m_customCOStyles;
};

#endif // USERDATA_H
