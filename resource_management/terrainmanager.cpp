#include "resource_management/terrainmanager.h"
#include "game/terrain.h"

TerrainManager::TerrainManager()
    : RessourceManagement<TerrainManager>("/images/terrain/res.xml",
                                          "/scripts/terrain", false)
{
#ifdef GRAPHICSUPPORT
    setObjectName("TerrainManager");
#endif
    Interpreter::setCppOwnerShip(this);
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

QString TerrainManager::getName(const QString & id)
{
    spTerrain pTerrain = Terrain::createTerrain(id, -1, -1, "", nullptr);
    return pTerrain->getTerrainName();
}

QString TerrainManager::getFittingResAnim(const QString & spriteIdStart, const QString & spriteIdEnd) const
{
    QString ret;
    const auto & keys = m_resourcesMap.keys();
    const auto spriteIdStartLower = spriteIdStart.toLower();
    const auto spriteIdEndLower = spriteIdEnd.toLower();
    for (const auto & key : qAsConst(keys))
    {
        if (key.startsWith(spriteIdStartLower) &&
            key.endsWith(spriteIdEndLower))
        {
            ret = key;
            break;
        }
    }
    return ret;
}

qint32 TerrainManager::getTerrainGroup(QString id)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainGroup";
    QJSValue ret = pInterpreter->doFunction(id, function1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    return 0;
}

QString TerrainManager::getTerrainGroupName(qint32 group)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainGroupName";
    QJSValueList args({QJSValue(group)});
    QJSValue ret = pInterpreter->doFunction("TERRAIN", function1, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "Unknown";
}

QStringList TerrainManager::getTerrainsSorted()
{
    QStringList sortedTerrains;
    QVector<qint32> terrainGroups;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainGroupSort";
    QJSValue ret = pInterpreter->doFunction("TERRAIN", function1);
    auto list = ret.toVariant().toList();
    for (const auto & terrainType : qAsConst(list))
    {
        qint32 value = terrainType.toInt();
        if (!terrainGroups.contains(value))
        {
            terrainGroups.append(value);
        }
    }

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

void TerrainManager::removeRessource(QString id)
{
    for (qint32 i = 0; i < m_loadedRessources.size(); ++i)
    {
        if (m_loadedRessources[i] == id)
        {
            m_loadedRessources.removeAt(i);
            break;
        }
    }
}
