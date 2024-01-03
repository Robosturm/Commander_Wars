#pragma once

#include <QByteArray>
#include <QString>
#include <QList>

class AwbwDataParser
{
public:
    static QString readString(const QByteArray & input);
    static qint64 readLong(const QByteArray & input);
    static qint32 readNullableInteger(const QByteArray & input);
    static float readFloat(const QByteArray & input);
    static bool readBool(const QByteArray & input);
    static QList<QByteArray> readList(const QByteArray & input);
private:
    static QString getStringValue(const QByteArray & stringPart, qint32 size);
};

