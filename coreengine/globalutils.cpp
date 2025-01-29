#include <QCoreApplication>
#include <QFileInfo>
#include <QDirIterator>
#include <QDateTime>
#include <QSet>

#include <cmath>

#include "coreengine/qmlvector.h"
#include "coreengine/globalutils.h"
#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"
#include "coreengine/settings.h"
#include "coreengine/vfs.h"

#include "game/gamemap.h"

static constexpr double DOUBLE_EPSILON = 0.5;
static constexpr float FLOAT_EPSILON = 0.5f;
static constexpr qint32 HEXBASE = 16;
static constexpr qint32 MAGIC_HEADER_SIZE = 18;
static const quint8 MAP_MAGIC_DATA[MAGIC_HEADER_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                         0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
                                                         0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const QByteArray GlobalUtils::MAP_MAGIC(reinterpret_cast<const char*>(MAP_MAGIC_DATA), MAGIC_HEADER_SIZE);

spGlobalUtils GlobalUtils::m_pInstace;

GlobalUtils::GlobalUtils()
{
#ifdef GRAPHICSUPPORT
    setObjectName("GlobalUtils");
#endif
    Interpreter::setCppOwnerShip(this);
}

void GlobalUtils::setup()
{
    m_pInstace = MemoryManagement::create<GlobalUtils>();
    srand(static_cast<unsigned>(time(nullptr)));
    Interpreter::setCppOwnerShip(m_pInstace.get());
    quint32 seedValue = QRandomGenerator::global()->bounded(0u, std::numeric_limits<quint32>::max());
    m_pInstace->m_randGenerator.seed(seedValue);
    m_pInstace->m_useSeed = false;
    m_pInstace->m_seed = 0;
}

QString GlobalUtils::getByteArrayString(const QByteArray & bytes)
{
    QString data;
    for (qint32 i = 0; i < bytes.size(); i++)
    {
        if (i > 0)
        {
            data += ",";
        }
        data += QString::number(bytes[i], HEXBASE);
    }
    return data;
}

QByteArray GlobalUtils::getStringByteArray(const QString & bytes)
{
    QByteArray data;
    QStringList items = bytes.split(",");
    for (auto & item : items)
    {
        bool ok;
        data += item.toInt(&ok, HEXBASE);
    }
    return data;
}

void GlobalUtils::seed(quint32 seed)
{
    m_pInstace->m_seed = seed;
    m_pInstace->m_randGenerator.seed(seed);
    CONSOLE_PRINT("Seeding with " + QString::number(m_pInstace->m_seed), GameConsole::eDEBUG);
}

quint32 GlobalUtils::getSeed()
{
    return m_pInstace->m_seed;
}

qint32 GlobalUtils::randInt(qint32 low, qint32 high, bool forceUnseeded)
{
    if (high <= low)
    {
        return low;
    }
    if (m_pInstace->m_useSeed && !forceUnseeded)
    {
        return m_pInstace->m_randGenerator.bounded(low, high + 1);
    }
    else
    {
        return randIntBase(low, high);
    }
}

float GlobalUtils::randFloat(float low, float high, bool forceUnseeded)
{
    if (high <= low)
    {
        return low;
    }
    if (m_pInstace->m_useSeed && !forceUnseeded)
    {
        return m_pInstace->m_randGenerator.bounded(high - low + 0.00001f) + low;
    }
    else
    {
        return randFloatBase(low, high);
    }
}

qreal GlobalUtils::randDouble(qreal low, qreal high, bool forceUnseeded)
{
    if (high <= low)
    {
        return low;
    }
    if (m_pInstace->m_useSeed && !forceUnseeded)
    {
        return m_pInstace->m_randGenerator.bounded(high - low + 0.00001) + low;
    }
    else
    {
        return randDoubleBase(low, high);
    }
}

qint32 GlobalUtils::randIntBase(qint32 low, qint32 high)
{
    if (high <= low)
    {
        return low;
    }
    return QRandomGenerator::global()->bounded(low, high + 1);
}

float GlobalUtils::randFloatBase(float low, float high)
{
    if (high <= low)
    {
        return low;
    }
    return QRandomGenerator::global()->bounded(high - low + 0.00001f) + low;
}

qreal GlobalUtils::randDoubleBase(qreal low, qreal high)
{
    if (high <= low)
    {
        return low;
    }
    return QRandomGenerator::global()->bounded(high - low + 0.00001) + low;
}

qint32 GlobalUtils::roundUp(qreal value)
{
    qint32 roundDown = static_cast<qint32>(value);
    // little cheat
    if (static_cast<float>(roundDown) == value)
    {
        return roundDown;
    }
    else
    {
        if (value > 0)
        {
            return roundDown + 1;
        }
        else
        {
            return roundDown;
        }
    }
}

qint32 GlobalUtils::roundUpInt(qint32 numToRound, qint32 multiple)
{
    if (multiple == 0)
    {
        return numToRound;
    }
    qint32 remainder = numToRound % multiple;
    if (remainder == 0)
    {
        return numToRound;
    }
    return numToRound + multiple - remainder;
}

qint32 GlobalUtils::roundDown(qreal value)
{
    qint32 roundDown = static_cast<qint32>(value);
    return roundDown;
}

spQmlVectorPoint GlobalUtils::getSpCircle(qint32 min, qint32 max)
{
    spQmlVectorPoint ret = MemoryManagement::create<QmlVectorPoint>();
    for (qint32 currentRadius = min; currentRadius <= max; currentRadius++)
    {
        if (currentRadius == 0)
        {
            ret->append(QPoint(0, 0));
        }
        else
        {
            qint32 x2 = -currentRadius;
            qint32 y2 = 0;
            for (qint32 i = 0; i < currentRadius; ++i)
            {
                x2 += 1;
                y2 += 1;
                ret->append(QPoint(x2, y2));
            }
            for (qint32 i = 0; i < currentRadius; ++i)
            {
                x2 += 1;
                y2 -= 1;
                ret->append(QPoint(x2, y2));
            }
            for (qint32 i = 0; i < currentRadius; ++i)
            {
                x2 -= 1;
                y2 -= 1;
                ret->append(QPoint(x2, y2));
            }
            for (qint32 i = 0; i < currentRadius; ++i)
            {
                x2 -= 1;
                y2 += 1;
                ret->append(QPoint(x2, y2));
            }
        }
    }
    return ret;
}

QmlVectorPoint* GlobalUtils::getCircle(qint32 min, qint32 max)
{
    auto ret = getSpCircle(min, max);
    Interpreter* pInterpreter = Interpreter::getInstance();
    Q_ASSERT(pInterpreter->getInJsCall());
    pInterpreter->trackJsObject(ret);
    return ret.get();
}

QmlVectorPoint* GlobalUtils::getShotFields(qint32 min, qint32 max, qint32 xDirection, qint32 yDirection)
{
    QmlVectorPoint* ret = MemoryManagement::createAndTrackJsObject<QmlVectorPoint>();
    for (qint32 i = min; i <= max; i++)
    {
        if (xDirection > 0)
        {
            for (qint32 y = -i + 1; y < i; y++)
            {
                ret->append(QPoint(i, y));
            }
        }
        else if (xDirection < 0)
        {
            for (qint32 y = -i + 1; y < i; y++)
            {
                ret->append(QPoint(-i, y));
            }
        }

        if (yDirection > 0)
        {
            for (qint32 x = -i + 1; x < i; x++)
            {
                ret->append(QPoint(x, i));
            }
        }
        else if (yDirection < 0)
        {
            for (qint32 x = -i + 1; x < i; x++)
            {
                ret->append(QPoint(x, -i));
            }
        }
    }
    return ret;
}

void GlobalUtils::setImageSize(qint32 value)
{
    GameMap::setImagesize(value);
}

qint32 GlobalUtils::getDistance(const QPoint p1, const QPoint p2)
{
    return qAbs(p1.x() - p2.x()) + qAbs(p1.y() - p2.y());
}

qint32 GlobalUtils::getDistance(qint32 x1, qint32 y1, qint32 x2, qint32 y2)
{
    return qAbs(x1 - x2) + qAbs(y1 - y2);
}

QmlVectorPoint* GlobalUtils::getEmptyPointArray()
{
    return MemoryManagement::createAndTrackJsObject<QmlVectorPoint>();
}

bool GlobalUtils::isEven(qint32 value)
{
    return (value % 2 == 0);
}

bool GlobalUtils::getUseSeed()
{
    return m_pInstace->m_useSeed;
}

void GlobalUtils::setUseSeed(bool useSeed)
{
    m_pInstace->m_useSeed = useSeed;
    if (m_pInstace->m_useSeed)
    {
        CONSOLE_PRINT("Running with seed " + QString::number(m_pInstace->m_seed), GameConsole::eDEBUG);
    }
    else
    {
        CONSOLE_PRINT("Stop running with seed", GameConsole::eDEBUG);
    }
}

QStringList GlobalUtils::getFiles(const QString folder, const QStringList filter)
{
    QStringList ret;

    QStringList paths = Vfs::createSearchPath("", false);
    for (const auto & path : std::as_const(paths))
    {
        QDirIterator dirIter(path + folder, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString file = dirIter.fileInfo().canonicalFilePath();
            file = GlobalUtils::makePathRelative(file);
            file.replace(folder + "/", "");
            file.replace(folder, "");
            ret.append(file);
        }
    }
    return ret;
}

void GlobalUtils::importFilesFromDirectory(QString folder, QString targetDirectory, QStringList filter, bool replace, QStringList excludeFolders)
{
    CONSOLE_PRINT("GlobalUtils::importFilesFromDirectory", GameConsole::eDEBUG);
    QDirIterator dirIter(folder, filter, QDir::Files, QDirIterator::Subdirectories);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().canonicalFilePath();
        bool exclude = false;
        for (const auto & exlucePath : excludeFolders)
        {
            if (file.contains("/" + exlucePath + "/"))
            {
                exclude = true;
                break;
            }
        }
        if (!exclude)
        {
            file.replace(folder + "/", "");
            file.replace(folder, "");
            bool exists = QFile::exists(targetDirectory + "/" + file);
            if (replace || !exists)
            {
                QFile::remove(targetDirectory + "/" + file);
                QFile::copy(dirIter.fileInfo().canonicalFilePath(), targetDirectory + "/" + file);
            }
        }
    }
}

double GlobalUtils::sigmoid(double x)
{
    return 1.0 / (1.0 + qExp(-x));
}

double GlobalUtils::relu(double x)
{
    if (x > 0)
    {
        return 1;
    }
    return 0;
}

double GlobalUtils::distanceVector(const QVector<double> v1, const QVector<double> v2)
{
    double d = 0;
    for (qint32 i = 0; i < v1.size(); i++)
    {
        d += (v1[i] - v2[i])*(v1[i] - v2[i]);
    }
    return d;
}

QVector<qint32> GlobalUtils::getRandomizedArray(qint32 min, qint32 max)
{
    QVector<qint32> ret;
    if (min < max)
    {
        QVector<qint32> temp;
        for (qint32 i = min; i <= max; ++i)
        {
            temp.append(i);
        }
        while (temp.size() > 0)
        {
            qint32 value = GlobalUtils::randInt(0, temp.size() - 1);
            ret.push_back(temp[value]);
            temp.removeAt(value);
        }
    }
    else
    {
        CONSOLE_PRINT("getRandomizedArray(min, max) min " + QString::number(min) + " is not smaller than max " + QString::number(max), GameConsole::eERROR);
    }
    return ret;
}

QString GlobalUtils::getNextAutosavePath(const QString path, const QString ending, qint32 max)
{
    CONSOLE_PRINT("GlobalUtils::getNextAutosavePath", GameConsole::eDEBUG);
    QString finalPath = path + QString::number(1) + ending;
    QDateTime oldestDate = QFileInfo(finalPath).lastModified();
    for (qint32 i = 2; i <= max; ++i)
    {
        if (oldestDate.isValid())
        {
            QString test = path + QString::number(i) + ending;
            QFileInfo info(test);
            if (info.exists())
            {
                if (info.lastModified() < oldestDate)
                {
                    finalPath = test;
                    oldestDate = info.lastModified();
                }
            }
            else
            {
                finalPath = test;
                break;
            }
        }
        else
        {
            break;
        }
    }
    return finalPath;
}

QString GlobalUtils::makePathRelative(QString file, bool full)
{
    QDir dir (QCoreApplication::applicationDirPath());
    QString path = dir.absolutePath();
    file = file.replace(path + "/", "");
    file = file.replace(path, "");
    QString userPath = Settings::getInstance()->getUserPath();
    if (!userPath.isEmpty())
    {
        file = file.replace(userPath + "/", "");
        file = file.replace(userPath, "");
    }
    if (file.startsWith(oxygine::Resource::RCC_PREFIX_PATH) && full)
    {
        file.remove(0, QString(oxygine::Resource::RCC_PREFIX_PATH).length());
    }
    return file;
}

QUrl GlobalUtils::getUrlForFile(const QString & file)
{
    QUrl url;
    if (file.startsWith(oxygine::Resource::RCC_PREFIX_PATH))
    {
        url = QUrl(oxygine::Resource::QRC_PREFIX + file);
    }
    else
    {
        url = QUrl::fromLocalFile(file);
    }
    return url;
}

QVector<qint32> GlobalUtils::calcWidths(const QVector<qint32> & maxWidths, const QVector<float> & distribution, qint32 totalWidth)
{
    QVector<qint32> ret;
    for (auto & item : distribution)
    {
        ret.append(totalWidth * item);
    }
    for (qint32 i = 0; i < maxWidths.size(); ++i)
    {
        qint32 count = ret[i] - maxWidths[i];
        if (count > 0)
        {
            ret[i] = maxWidths[i];
            qint32 item = 0;
            while (count > 0)
            {
                if (ret[item] < maxWidths[item])
                {
                    ++ret[item];
                    --count;
                }
                ++item;
                if (item >= maxWidths.size())
                {
                    item = 0;
                }
            }
        }
    }
    return ret;
}

float GlobalUtils::roundFloor(float value, float precision)
{
    return std::floor(value * precision) / precision;
}

double GlobalUtils::roundFloor(double value, double precision)
{
    return std::floor(value * precision) / precision;
}

float GlobalUtils::roundCeil(float value, float precision)
{
    return std::ceil(value * precision) / precision;
}

double GlobalUtils::roundCeil(double value, double precision)
{
    return std::ceil(value * precision) / precision;
}

qint32 GlobalUtils::roundFloorToInt(float value, float precision)
{
    return std::floor(value * precision);
}

qint32 GlobalUtils::roundFloorToInt(qreal value, qreal precision)
{
    return std::floor(value * precision);
}

QByteArray GlobalUtils::toByteArray(const QJsonArray & jsonArray)
{
    QByteArray array;
    for (auto & item : jsonArray)
    {
        array.append(static_cast<char>(item.toInt()));
    }
    return array;
}

QJsonArray GlobalUtils::toJsonArray(const QByteArray & byteArray)
{
    QJsonArray array;
    for (qint32 i = 0; i < byteArray.size(); ++i)
    {
        array.append(byteArray[i]);
    }
    return array;
}
