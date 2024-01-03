#include "awbwReplayReader/awbwdataparser.h"
#include <QList>

QString AwbwDataParser::readString(const QByteArray & input)
{
    auto data = input.split(':');
    if (data[0] == "s")
    {
        QString ret = data[2].split('\"')[1];
        if (ret.size() == data[1].toInt())
        {
            return ret;
        }
        else
        {
            return "";
        }
    }
    else if (data[0] =="N")
    {
        return "";
    }
    return "";
}

qint64 AwbwDataParser::readLong(const QByteArray & input)
{
    auto data = input.split(':');
    if (data[0] == "i")
    {
        return data[1].toInt();
    }
    return 0;
}

qint32 AwbwDataParser::readNullableInteger(const QByteArray & input)
{
    auto data = input.split(':');
    if (data[0] == "N")
    {
        return 0;
    }
    return readLong(input);
}

float AwbwDataParser::readFloat(const QByteArray & input)
{
    auto data = input.split(':');
    if (data[0] == "d")
    {
        return data[1].toFloat();
    }
    return 0;
}

bool AwbwDataParser::readBool(const QByteArray & input)
{
    auto data = input.split(':');
    if (data[0] == "s" && data[1].toInt() == 1)
    {
        if (data[2] == "Y" || data[2] == "y")
        {
            return true;
        }
        else if (data[2] == "N" || data[2] == "n")
        {
            return false;
        }
    }
    return false;
}
