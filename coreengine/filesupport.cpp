#include "filesupport.h"

#include "coreengine/settings.h"

#include <QCryptographicHash>
#include <QDirIterator>
#include <QCoreApplication>

Filesupport::Filesupport() : QObject()
{

}

QByteArray Filesupport::getHash(QStringList filter, QStringList folders)
{
    QCryptographicHash myHash(QCryptographicHash::Sha3_512);
    for (auto folder : folders)
    {
        QString path =  QCoreApplication::applicationDirPath() + "/" + folder;
        QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QFile file(dirIter.filePath());
            file.open(QIODevice::ReadOnly | QIODevice::Truncate);
            myHash.addData(&file);
            file.close();
        }
    }
    return myHash.result();
}

QByteArray Filesupport::getRuntimeHash()
{
    QStringList folders = Settings::getMods();
    folders.append("/resources");
    QStringList filter = {"*.js", "*.txt"};
    return getHash(filter, folders);
}

void Filesupport::writeByteArray(QDataStream& stream, const QByteArray& array)
{
    stream << static_cast<qint32>(array.size());
    for (qint32 i = 0; i < array.size(); i++)
    {
        stream << static_cast<qint8>(array[i]);
    }
}

QByteArray Filesupport::readByteArray(QDataStream& stream)
{
    QByteArray array;
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        qint8 value = 0;
        stream >> value;
        array.append(value);
    }
    return array;
}
