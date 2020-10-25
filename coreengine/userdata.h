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
    struct Achievement
    {
        QString id;
        qint32 progress{0};
        qint32 targetValue{0};
        QString name;
        QString description;
        QString icon;
        bool hide{false};
        bool loaded{false};
    };

    static Userdata* getInstance();
    void changeUser();
    void storeUser();
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
        return 4;
    }
    /**
     * @brief addCOStyle
     * @param coid
     * @param file
     * @param colorTable
     * @param maskTable
     */
    void addCOStyle(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox);
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
    std::tuple<QString, QString, QImage, QImage, bool>* getCOStyle(QString coid);
    /**
     * @brief getAchievements
     * @return
     */
    QVector<Achievement>* getAchievements();
signals:

public slots:
    /**
     * @brief addAchievement
     * @param id
     * @param value
     */
    void increaseAchievement(QString id, qint32 value);
    /**
     * @brief addAchievement
     * @param id
     * @param targetValue
     * @param name
     * @param description
     * @param icon
     * @param hide
     */
    void addAchievement(QString id, qint32 targetValue, QString name, QString description, QString icon, bool hide = false);

private:
    void showAchieved();

private:
    explicit Userdata();

    static Userdata* m_pInstance;

    QVector<std::tuple<QString, QString, QImage, QImage, bool>> m_customCOStyles;
    QVector<Achievement> m_achievements;
};

#endif // USERDATA_H
