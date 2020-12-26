#ifndef GLOBALUTILS_H
#define GLOBALUTILS_H

#include <QObject>
#include "coreengine/qmlvector.h"
#include <QRandomGenerator>
#include "coreengine/qmlvector.h"

class GlobalUtils : public QObject
{
    Q_OBJECT
public:
    static GlobalUtils* getInstance()
    {
        return &m_pInstace;
    };
    static bool getUseSeed();
    static void setUseSeed(bool useSeed);
    static qint32 randIntBase(qint32 low, qint32 high);
public slots:
    /**
     * @brief seed
     * @param seed
     */
    static void seed(quint32 seed);
    /**
     * @brief getSeed
     * @return
     */
    static quint32 getSeed();
    static qint32 randInt(qint32 low, qint32 high);
    /**
     * @brief roundUp rounds all numbers up. 9.1 -> 10
     * @param value
     * @return the rounded up integer value
     */
    static qint32 roundUp(float value);
    static qint32 roundDown(float value);
    static bool isEven(qint32 value);
    /**
     * @brief getCircle
     * @param min radius
     * @param max radius
     * @return vector containing all fields in the given radius. Note: the Object needs to be deleted by the reciever
     */
    static QmlVectorPoint* getCircle(qint32 min, qint32 max);
    /**
     * @brief getShotFields
     * @param min
     * @param max
     * @param xDirection
     * @param yDirection
     * @return
     */
    static QmlVectorPoint* getShotFields(qint32 min, qint32 max, qint32 xDirection = 0, qint32 yDirection = 0);
    /**
     * @brief getDistance
     * @param p1
     * @param p2
     * @return
     */
    static qint32 getDistance(QPoint p1, QPoint p2);
    /**
     * @brief getEmptyPointArray
     * @return
     */
    static QmlVectorPoint* getEmptyPointArray();
    /**
     * @brief setImageSize
     * @return
     */
    static void setImageSize(qint32 value);
    /**
     * @brief getFiles
     * @param folder
     * @param filter
     * @return
     */
    static QStringList getFiles(QString folder, QStringList filter);
private:
    explicit GlobalUtils();
    static GlobalUtils m_pInstace;
    static QRandomGenerator randGenerator;
    static bool m_useSeed;
    static quint32 m_seed;
};

#endif // GLOBALUTILS_H
