#include <QFileInfo>
#include <QDirIterator>
#include "resource_management/movementtablemanager.h"
#include "coreengine/mainapp.h"

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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getName";
    QJSValue ret = pApp->getInterpreter()->doFunction(movementID, function1);
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
            loadTable(file.toUpper());
        }
    }
}


bool MovementTableManager::loadTable(const QString& movementtableID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/movementtables");
    }
    searchPaths.append("resources/scripts/movementtables");
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + movementtableID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            m_loadedTables.append(movementtableID);
            return true;
        }
    }
    return false;
}

void MovementTableManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedTables.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedTables[i]);
    }
    m_loadedTables.clear();
}

qint8 MovementTableManager::getMovementPoints(Unit* pUnit, Terrain* pTerrain)
{
    qint8 basePoints = getBaseMovementPoints(pUnit->getMovementType(), pTerrain->getID());
    return basePoints;
}

qint8 MovementTableManager::getBaseMovementPoints(QString movementID, QString terrainID)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementpoints";
    QJSValueList args1;
    args1 << terrainID;
    QJSValue ret = pApp->getInterpreter()->doFunction(movementID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return -1;
    }
}
