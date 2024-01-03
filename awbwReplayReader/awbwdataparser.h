#pragma once

#include <QByteArray>
#include <QString>

class AwbwDataParser
{
public:
    static QString readString(const QByteArray & input);
    static qint64 readLong(const QByteArray & input);
    static qint32 readNullableInteger(const QByteArray & input);
    static float readFloat(const QByteArray & input);
    static bool readBool(const QByteArray & input);
};

