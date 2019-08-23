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
            if (!m_loadedTables.contains(file.toUpper()))
            {
                loadTable(file.toUpper());
            }
        }
    }
    m_loadedTables.sort();
}


bool MovementTableManager::loadTable(const QString& movementtableID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

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
            pMainapp->getInterpreter()->openScript(file);
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
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedTables.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedTables[i]);
    }
    m_loadedTables.clear();
}

qint32 MovementTableManager::getBaseMovementPoints(QString movementID, Terrain* pTerrain, Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementpoints";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(pTerrain);
    args1 << obj3;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj2;
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
