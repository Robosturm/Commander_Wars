#include <QFileInfo>
#include <QDirIterator>

#include "coreengine/vfs.h"

#include "resource_management/movementtablemanager.h"

#include "modding/csvtableimporter.h"

#include "game/unit.h"
#include "game/terrain.h"
#include "game/gamemap.h"

MovementTableManager::MovementTableManager()
    : RessourceManagement<MovementTableManager>("",
                                                "/scripts/movementtables")
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("MovementTableManager");
#endif
}

void MovementTableManager::loadAll()
{
    RessourceManagement<MovementTableManager>::loadAll();
    // import csv tables
    // clean up temp folder
    Interpreter* pInterpreter = Interpreter::getInstance();
    QTemporaryDir tempDir = Settings::getInstance()->newTempDir();
    QStringList data;
    QFile file(Vfs::find("resources/scripts/movementtables/movement_csv_import.txt"));
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString jsHeader = stream.readAll();
    QStringList searchPaths = getSearchPaths();
    for (auto & path : searchPaths)
    {
        QString jsData = CsvTableImporter::ImportCsvTable(path + "/movementtables.csv", jsHeader, "movementpointsTable", data);
        if (!jsData.isEmpty())
        {
            m_loadedRessources.append(data);
            pInterpreter->loadScript(jsData, tempDir.path() + "/movementtable.js");
        }
    }
    m_loadedRessources.sort();
}
qint32 MovementTableManager::getBaseMovementPoints(const QString & movementID, Terrain* pTerrain, Terrain* pCurrentTerrain, Unit* pUnit, bool trapChecking)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementpoints";
    QJSValueList args({JsThis::getJsThis(pTerrain),
                       JsThis::getJsThis(pUnit),
                       JsThis::getJsThis(pCurrentTerrain),
                       trapChecking,
                       GameMap::getMapJsThis(pCurrentTerrain->getMap())});
    QJSValue ret = pInterpreter->doFunction(movementID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return -1;
    }
}

bool MovementTableManager::getSupportsFastPfs(const QString & movementID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getSupportsFastPfs";
    QJSValue ret = pInterpreter->doFunction(movementID, function1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    else
    {
        return true;
    }
}
