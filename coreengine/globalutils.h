#ifndef GLOBALUTILS_H
#define GLOBALUTILS_H

#include <memory>

#include <QObject>
#include <QDir>
#include <QRandomGenerator>

class QmlVectorPoint;
using spQmlVectorPoint = std::shared_ptr<QmlVectorPoint>;

class GlobalUtils;
using spGlobalUtils = std::shared_ptr<GlobalUtils>;

class GlobalUtils final : public QObject
{
    Q_OBJECT
public:
    virtual ~GlobalUtils() = default;
    static GlobalUtils* getInstance()
    {
        return m_pInstace.get();
    }
    static void setup();
    static bool getUseSeed();
    static void setUseSeed(bool useSeed);
    static qint32 randIntBase(qint32 low, qint32 high);
    static float randFloatBase(float low, float high);
    static qreal randDoubleBase(qreal low, qreal high);
    static QFileInfoList getInfoList(const QString & folder, const QStringList & list = QStringList());
    static QUrl getUrlForFile(const QString & file);
    static QString getByteArrayString(const QByteArray & bytes);
    static QVector<qint32> calcWidths(const QVector<qint32> & maxWidths, const QVector<float> & distribution, qint32 totalWidth);
    template<typename TData>
    static bool contains(const std::vector<TData> & vec, const TData & data)
    {
        return std::find(vec.cbegin(), vec.cend(), data) != vec.cend();
    }
    static spQmlVectorPoint getSpCircle(qint32 min, qint32 max);
public slots:
    static QString makePathRelative(QString file, bool full = true);
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
    static qint32 randInt(qint32 low, qint32 high, bool forceUnseeded = false);
    static float randFloat(float low, float high, bool forceUnseeded = false);
    static qreal randDouble(qreal low, qreal high, bool forceUnseeded = false);
    /**
     * @brief roundUp rounds all numbers up. 9.1 -> 10
     * @param value
     * @return the rounded up integer value
     */
    static qint32 roundUp(qreal value);
    static qint32 roundUpInt(qint32 numToRound, qint32 multiple);
    static qint32 roundDown(qreal value);
    static float roundFloor(float value, float precision);
    static double roundFloor(double value, double precision);
    static float roundCeil(float value, float precision);
    static double roundCeil(double value, double precision);
    static qint32 roundFloorToInt(float value, float precision);
    static qint32 roundFloorToInt(qreal value, qreal precision);

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
    static qint32 getDistance(const QPoint p1, const QPoint p2);
    /**
     * @brief getDistance
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @return
     */
    static qint32 getDistance(qint32 x1, qint32 y1, qint32 x2, qint32 y2);
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
    static QStringList getFiles(const QString folder, const QStringList filter);
    /**
     * @brief importFilesFromDirectory
     * @param folder
     * @param targetDirectory
     * @param filter
     * @param replace
     */
    static void importFilesFromDirectory(QString folder, QString targetDirectory, QStringList filter, bool replace, QStringList excludeFolders = QStringList());
    /**
     * @brief sigmoid
     * @param x
     * @return
     */
    static double sigmoid(double x);
    /**
     * @brief relu
     * @param x
     * @return
     */
    static double relu(double x);
    /**
     * @brief distanceVector
     * @param v1
     * @param v2
     * @return
     */
    static double distanceVector(const QVector<double> v1, const QVector<double> v2);
    /**
     * @brief getRandomizedArray
     * @param min
     * @param max
     * @return
     */
    static QVector<qint32> getRandomizedArray(qint32 min, qint32 max);
    /**
     * @brief getNextAutosavePath
     * @param path
     * @param ending
     * @param max
     * @return
     */
    static QString getNextAutosavePath(const QString path, const QString ending, qint32 max);
private:
    explicit GlobalUtils();
private:
    friend class MemoryManagement;
    static spGlobalUtils m_pInstace;
    QRandomGenerator m_randGenerator;
    bool m_useSeed;
    quint32 m_seed;
};

#endif // GLOBALUTILS_H
