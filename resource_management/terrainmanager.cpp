#include "resource_management/terrainmanager.h"

#include <QFileInfo>

#include <QDirIterator>

#include "coreengine/mainapp.h"

TerrainManager* TerrainManager::m_pInstance = nullptr;

TerrainManager::TerrainManager() : QObject()
{
    // load terrain
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        oxygine::Resources::loadXML(QString(pMainapp->getSettings()->getMods().at(i) + "/images/terrain/res.xml").toStdString());
    }
    oxygine::Resources::loadXML("resources/images/terrain/res.xml");
}

void TerrainManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedTerrains.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedTerrains[i]);
    }
    m_loadedTerrains.clear();
}

void TerrainManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/terrain");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/terrain");
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
            loadTerrain(file.toUpper());
        }
    }
}

bool TerrainManager::loadTerrain(const QString& TerrainID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/terrain");
    }
    searchPaths.append("resources/scripts/terrain");
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + TerrainID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            m_loadedTerrains.append(TerrainID);
            return true;
        }
    }
    return false;
}

TerrainManager* TerrainManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new TerrainManager();
    }
    return m_pInstance;
}
