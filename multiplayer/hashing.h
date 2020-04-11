#ifndef HASHING_H
#define HASHING_H

#include <QObject>
#include <QDataStream>

class Hashing : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getRuntimeHash
     * @return
     */
    static QByteArray getRuntimeHash();
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
     * @brief readByteArray
     * @param stream
     * @param array
     */
    static void readByteArray(QDataStream& stream, QByteArray& array);
signals:

private:
    explicit Hashing();
};

#endif // HASHING_H
