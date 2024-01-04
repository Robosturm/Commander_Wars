#pragma once

#include <QString>
#include <QTextStream>

class AwbwDataParser
{
public:
    static QString readString(QTextStream & input, bool & success);
    static qint64 readLong(QTextStream & input, bool & success);
    static qint32 readNullableInteger(QTextStream & input, bool & success, const QString & type = "i");
    static float readFloat(QTextStream & input, bool & success);
    static bool readBool(QTextStream & input, bool & success);
    static QString readObjectStart(QTextStream & input, bool & success, qint32 & itemCount);
    static qint32 readArrayStart(QTextStream & input, bool & success);
    static bool readArrayObjectEnd(QTextStream & input);
private:
    static bool readSeperator(QTextStream & input, const QString & seperator);
    static QString readTillSign(QTextStream & input, const QString & seperator);
    static QString readStringPart(QTextStream & input, bool & success, const QString & seperator);
};

