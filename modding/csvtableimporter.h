#ifndef CSVTABLEIMPORTER_H
#define CSVTABLEIMPORTER_H

#include <qstring.h>

class CsvTableImporter
{
public:

    static QString ImportCsvTable(QString csvTable, QString jsHeaderString, QString tableName, QStringList& newData);
private:
    CsvTableImporter() = default;
};

#endif // CSVTABLEIMPORTER_H
