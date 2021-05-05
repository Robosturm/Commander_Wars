#include "coreengine/filesupport.h"
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
    for (const auto & folder : qAsConst(folders))
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

QByteArray Filesupport::getRuntimeHash(QStringList mods)
{
    QStringList folders = mods;
    folders.append("/resources");
    QStringList filter = {"*.js", "*.txt", "*.csv"};
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

void Filesupport::writeBytes(QDataStream& stream, const QByteArray& array)
{
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

void Filesupport::storeList(QString file, QStringList items, QString folder)
{
    QDir dir(folder);
    dir.mkpath(".");
    QFile dataFile(folder + file + ".bl");
    dataFile.open(QIODevice::WriteOnly);
    QDataStream stream(&dataFile);
    stream << file;
    stream << static_cast<qint32>(items.size());
    for (qint32 i = 0; i < items.size(); i++)
    {
        stream << items[i];
    }
}

std::tuple<QString, QStringList> Filesupport::readList(QString file, QString folder)
{
    return readList(folder + file);
}

std::tuple<QString, QStringList> Filesupport::readList(QString file)
{
    QFile dataFile(file);
    dataFile.open(QIODevice::ReadOnly);
    QDataStream stream(&dataFile);
    std::tuple<QString, QStringList> ret;
    stream >> std::get<0>(ret);
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        QString name;
        stream >> name;
        std::get<1>(ret).append(name);
    }
    return ret;
}
