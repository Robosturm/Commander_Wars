#include <qfile.h>
#include <qtextstream.h>

#include "modding/csvtableimporter.h"

#include "coreengine/interpreter.h"

QString CsvTableImporter::ImportCsvTable(QString csvTable, QString jsHeaderString, QString tableName, QStringList& newData)
{
    QString jsTable;
    QFile file(csvTable);
    if (file.exists())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        QStringList header = stream.readLine().split(";");
        MergeCsvEntry(header);
        if (header.size() > 1)
        {
            while (!stream.atEnd())
            {
                QStringList item = stream.readLine().split(";");
                MergeCsvEntry(item);
                if (item.size() == header.size())
                {
                    QString object = item[0].toUpper();
                    QJSValue pObj = pInterpreter->getGlobal(object);
                    if (pObj.isNull() || pObj.isUndefined())
                    {
                        QString data = jsHeaderString;
                        data = data.replace("@obj", object).replace("@name", item[1]).replace("@custom", item[2]);
                        jsTable += data;
                        newData.append(object);
                    }
                    else
                    {
                        jsTable += item[2];
                    }
                    for (qint32 i = 3; i < item.size(); i++)
                    {
                        if (!item[i].isEmpty())
                        {
                            jsTable += "var idx = getIndexOf1(" + object + "." + tableName + ", \"" + header[i].toUpper() + "\");\n";
                            jsTable += "if (idx >= 0)\n{\n";
                            jsTable += object + "." + tableName + "[idx][1] = " + item[i] + ";\n}\nelse\n{\n";
                            jsTable += object + "." + tableName + ".push([\"" + header[i].toUpper() + "\", " + item[i] + "]);\n";
                            jsTable += "\n}\n";
                        }
                    }
                }
            }
        }
    }
    return jsTable;
}

void CsvTableImporter::MergeCsvEntry(QStringList & item)
{
    qint32 i = 0;
    while (i < item.size())
    {
        if (item[i].startsWith("\""))
        {
            QString newItem = item[i];
            qint32 i2 = i + 1;
            bool exit = false;
            while (i2 < item.size() && !exit)
            {
                if (item[i2].endsWith("\""))
                {
                    exit = true;
                }
                newItem += item[i2] + ";";
                item.removeAt(i2);
            }
            item[i] = newItem.replace("\"", "");
        }
        else
        {
            i++;
        }
    }
}
