#ifndef CSVTABLEIMPORTER_H
#define CSVTABLEIMPORTER_H

#include <QString>

class CsvTableImporter final
{
public:

    static QString ImportCsvTable(QString csvTable, QString jsHeaderString, QString tableName, QStringList& newData);
private:
    explicit CsvTableImporter() = delete;
   virtual ~CsvTableImporter() = delete;
    static void MergeCsvEntry(QStringList & item);
};

#endif // CSVTABLEIMPORTER_H
