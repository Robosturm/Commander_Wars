#include "resource_management/terrainmanager.h"
#include <QFileInfo>
#include "coreengine/mainapp.h"

TerrainManager* TerrainManager::m_pInstance = nullptr;

TerrainManager::TerrainManager() : QObject()
{
    // load terrain
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


bool TerrainManager::loadTerrain(const QString& TerrainID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths = pMainapp->getSettings()->getMods();
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
