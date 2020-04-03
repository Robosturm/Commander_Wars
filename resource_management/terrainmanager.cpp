#include "resource_management/terrainmanager.h"

TerrainManager::TerrainManager()
    : RessourceManagement<TerrainManager>("/images/terrain/res.xml",
                                             "/scripts/terrain")
{
}

qint32 TerrainManager::getTerrainGroup(qint32 i)
{
    if ((i >= 0) && (i < m_loadedRessources.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getTerrainGroup";
        QJSValue ret = pInterpreter->doFunction(m_loadedRessources[i], function1);
        if (ret.isNumber())
        {
            return ret.toInt();
        }
    }
    return 0;
}

QStringList TerrainManager::getTerrainsSorted()
{
    QStringList sortedTerrains;
    QVector<qint32> terrainGroups;
    for (qint32 i = 0; i < m_loadedRessources.size(); i++)
    {
        qint32 terrainGroup = getTerrainGroup(i);
        if (!terrainGroups.contains(terrainGroup))
        {
            terrainGroups.append(terrainGroup);
        }
    }
    for (qint32 i2 = 0; i2 < terrainGroups.size(); i2++)
    {
        for (qint32 i = 0; i < m_loadedRessources.size(); i++)
        {
            if (getTerrainGroup(i) == terrainGroups[i2])
            {
                sortedTerrains.append(m_loadedRessources[i]);
            }
        }
    }
    return sortedTerrains;
}
