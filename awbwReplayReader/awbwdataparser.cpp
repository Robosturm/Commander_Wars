#include "awbwReplayReader/awbwdataparser.h"

QString AwbwDataParser::readString(QTextStream & input, bool & success)
{
    QString ret;
    QString type = input.read(1);
    if (type == "s")
    {
        success = readSeperator(input, ":");
        if (success)
        {
            ret = readStringPart(input, success, ";");
        }
    }
    else if (type == "N")
    {
        success = readSeperator(input, ";");
    }
    else
    {
        success = false;
    }
    return ret;
}

qint64 AwbwDataParser::readLong(QTextStream & input, bool & success)
{
    qint64 ret = 0;
    success = input.read(1) == "i";
    success = success && readSeperator(input, ":");
    if (success)
    {
        ret = readTillSign(input, ";").toLong(&success);
    }
    return ret;
}

qint32 AwbwDataParser::readNullableInteger(QTextStream & input, bool & success, const QString & type)
{
    qint64 ret = 0;
    QString value = input.read(1);
    if (value == type)
    {
        success = readSeperator(input, ":");
        if (success)
        {
            ret = readTillSign(input, ";").toLong(&success);
        }
    }
    else if (value == "N")
    {
        success = readSeperator(input, ";");
    }
    else
    {
        success = false;
    }
    return ret;
}

float AwbwDataParser::readFloat(QTextStream & input, bool & success)
{
    float ret = 0;
    success = input.read(1) == "d";
    if (success)
    {
        success = readSeperator(input, ":");
        if (success)
        {
            ret = readTillSign(input, ";").toFloat(&success);
        }
    }
    return ret;
}

bool AwbwDataParser::readBool(QTextStream & input, bool & success)
{
    bool ret = false;
    QString value = readString(input, success);
    if (value == "Y" || value == "y")
    {
        ret = true;
    }
    else if (value == "N" || value == "n")
    {
        ret = false;
    }
    else
    {
        success = false;
    }
    return ret;
}

QString AwbwDataParser::readObjectStart(QTextStream & input, bool & success, qint32 & itemCount)
{
    QString name;
    itemCount = 0;
    if (input.read(1) == "O")
    {
        success = readSeperator(input, ":");
        if (success)
        {
            name = readStringPart(input, success, ":");
            if (success)
            {
                itemCount = readTillSign(input, ":").toLong(&success);
                success = success && readSeperator(input, "{");
            }
        }
    }
    else
    {
        success = false;
    }
    return name;
}

qint32 AwbwDataParser::readArrayStart(QTextStream & input, bool & success)
{
    qint32 itemCount = 0;
    if (input.read(1) == "a")
    {
        success = readSeperator(input, ":");
        if (success)
        {
            if (success)
            {
                itemCount = readTillSign(input, ":").toLong(&success);
                success = success && readSeperator(input, "{");
            }
        }
    }
    else
    {
        success = false;
    }
    return itemCount;

}

bool AwbwDataParser::readSeperator(QTextStream & input, const QString & seperator)
{
    return input.read(seperator.size()) == seperator;
}

QString AwbwDataParser::readTillSign(QTextStream & input, const QString & seperator)
{
    QString ret;
    QString sign = input.read(1);
    while (sign != seperator)
    {
        ret += sign;
        sign = input.read(1);
    }
    return ret;
}

QString AwbwDataParser::readStringPart(QTextStream & input, bool & success, const QString & seperator)
{
    QString ret;
    qint32 size = readTillSign(input, ":").toInt(&success);
    if (success && input.read(1) == "\"")
    {
        ret = input.read(size);
        success = input.read(1) == "\"";
        success = success && readSeperator(input, seperator);
    }
    else
    {
        success = false;
    }
    return ret;
}

bool AwbwDataParser::readArrayObjectEnd(QTextStream & input)
{
    return input.read(1) == "}";
}
