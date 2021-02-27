#ifndef FILESUPPORT_H
#define FILESUPPORT_H

#include <QObject>
#include <QDataStream>

class Filesupport : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getRuntimeHash
     * @return
     */
    static QByteArray getRuntimeHash(QStringList mods);
    /**
     * @brief getHash
     * @param filter
     * @param folders
     * @return
     */
    static QByteArray getHash(QStringList filter, QStringList folders);
    /**
     * @brief writeByteArray
     * @param stream
     * @param array
     */
    static void writeByteArray(QDataStream& stream, const QByteArray& array);
    /**
     * @brief writeBytes
     * @param stream
     * @param array
     */
    static void writeBytes(QDataStream& stream, const QByteArray& array);
    /**
     * @brief readByteArray
     * @param stream
     * @param array
     */
    static QByteArray readByteArray(QDataStream& stream);

    template<typename _TVectorList>
    static void writeVectorList(QDataStream& stream, _TVectorList array)
    {
        stream << static_cast<qint32>(array.size());
        for (qint32 i = 0; i < array.size(); i++)
        {
            stream << array[i];
        }
    }

    template<typename _TType, template<typename T> class _TVectorList>
    static _TVectorList<_TType> readVectorList(QDataStream& stream)
    {
        _TVectorList<_TType> array;
        qint32 size = 0;
        stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            _TType value = 0;
            stream >> value;
            array.append(value);
        }
        return array;
    }
    /**
     * @brief storeList
     * @param file
     * @param items
     * @param folder
     */
    static void storeList(QString file, QStringList items, QString folder);
    /**
     * @brief readList
     * @param file
     * @return
     */
    static std::tuple<QString, QStringList> readList(QString file);
    /**
     * @brief readList
     * @param file
     * @param folder
     * @return
     */
    static std::tuple<QString, QStringList> readList(QString file, QString folder);

signals:

private:
    explicit Filesupport();
};

#endif // HASHING_H
