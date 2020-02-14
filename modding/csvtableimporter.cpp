#include "csvtableimporter.h"

#include <qfile.h>
#include <qtextstream.h>
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
        if (header.size() > 1)
        {
            while (!stream.atEnd())
            {
                QStringList item = stream.readLine().split(";");
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
                    for (qint32 i = 3; i < item.size(); i++)
                    {
                        if (!item.isEmpty())
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
