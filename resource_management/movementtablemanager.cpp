#include <QFileInfo>
#include <QDirIterator>
#include "resource_management/movementtablemanager.h"
#include "coreengine/mainapp.h"

#include "modding/csvtableimporter.h"

#include "game/unit.h"
#include "game/terrain.h"

MovementTableManager* MovementTableManager::m_pInstance = nullptr;

MovementTableManager::MovementTableManager()
{
}

MovementTableManager* MovementTableManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new MovementTableManager();
    }
    return m_pInstance;
}

QString MovementTableManager::getMovementName(QString movementID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getName";
    QJSValue ret = pInterpreter->doFunction(movementID, function1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

void MovementTableManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/movementtables");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/movementtables");
    }
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path =  QCoreApplication::applicationDirPath() + "/" + searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator* dirIter = new QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter->hasNext())
        {
            dirIter->next();
            QString file = dirIter->fileInfo().fileName().split(".").at(0);
            if (!m_loadedTables.contains(file.toUpper()))
            {
                loadTable(file.toUpper());
            }
        }
    }

    // import csv tables
    // clean up temp folder
    Interpreter* pInterpreter = Interpreter::getInstance();
    QDir dir("temp/");
    dir.removeRecursively();
    dir.mkpath(".");
    QStringList data;
    QFile file("resources/scripts/movementtables/movement_csv_import.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString jsHeader = stream.readAll();
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString jsData = CsvTableImporter::ImportCsvTable(searchPaths[i] + "/movementtables.csv", jsHeader, "movementpointsTable", data);
        if (!jsData.isEmpty())
        {
            m_loadedTables.append(data);
            QFile tmp("temp/table.js");
            tmp.open(QIODevice::WriteOnly);
            QTextStream stream2(&tmp);
            stream2 << jsData;
            tmp.close();
            pInterpreter->openScript("temp/table.js");
            QFile::remove("temp/table.js");
        }
    }

    m_loadedTables.sort();


}


bool MovementTableManager::loadTable(QString movementtableID)
{
    Mainapp* pMainapp = Mainapp::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/movementtables");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/movementtables");
    }
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + movementtableID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pInterpreter->openScript(file);
            if (!bRet)
            {
                m_loadedTables.append(movementtableID);
            }
            bRet = true;
        }
    }
    return bRet;
}

void MovementTableManager::reset()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (qint32 i = 0; i < m_loadedTables.size(); i++)
    {
        pInterpreter->deleteObject(m_loadedTables[i]);
    }
    m_loadedTables.clear();
}

qint32 MovementTableManager::getBaseMovementPoints(QString movementID, Terrain* pTerrain, Terrain* pCurrentTerrain, Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementpoints";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(pTerrain);
    args1 << obj3;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj4 = pInterpreter->newQObject(pCurrentTerrain);
    args1 << obj4;
    QJSValue ret = pInterpreter->doFunction(movementID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return -1;
    }
}
