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

    static constexpr qint32 MAX_VICTORY_INFO_PER_MAP = 5;
    struct MapVictoryInfo
    {
        QString mapPath;
        QStringList co1;
        QStringList co2;
        QVector<qint32> score;
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
        return 5;
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
    /**
     * @brief addVictoryForMap
     * @param mapPath
     * @param co1
     * @param co2
     * @param score
     */
    void addVictoryForMap(QString mapPath, QString co1, QString co2, qint32 score);
    /**
     * @brief getVictoryForMap
     * @param mapPath
     * @return
     */
    const MapVictoryInfo * getVictoryForMap(QString mapPath);
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
    /**
     * @brief deleteAchievement
     * @param id
     */
    void deleteAchievement(QString id);
    /**
     * @brief achieved
     * @param id
     * @return
     */
    bool achieved(QString id);
private:
    void showAchieved();

private:
    explicit Userdata();

    static Userdata* m_pInstance;

    QVector<std::tuple<QString, QString, QImage, QImage, bool>> m_customCOStyles;
    QVector<Achievement> m_achievements;
    QVector<MapVictoryInfo> m_mapVictoryInfo;
};

#endif // USERDATA_H
