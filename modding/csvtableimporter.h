#ifndef CSVTABLEIMPORTER_H
#define CSVTABLEIMPORTER_H

#include <qstring.h>

class CsvTableImporter
{
public:

    static QString ImportCsvTable(QString csvTable, QString jsHeaderString, QString tableName, QStringList& newData);
private:
    explicit CsvTableImporter() = default;
    virtual ~CsvTableImporter() = default;
    static void MergeCsvEntry(QStringList & item);
};

#endif // CSVTABLEIMPORTER_H
