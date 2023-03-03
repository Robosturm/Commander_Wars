#include "resource_management/buildingspritemanager.h"

BuildingSpriteManager::BuildingSpriteManager()
    : RessourceManagement<BuildingSpriteManager>("/images/building/res.xml",
                                                  "/scripts/building")
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("BuildingSpriteManager");
#endif
}

void BuildingSpriteManager::removeRessource(QString id)
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

qint32 BuildingSpriteManager::getBuildingGroup(qint32 i)
{
    if ((i >= 0) && (i < m_loadedRessources.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getBuildingGroup";
        QJSValue ret = pInterpreter->doFunction(m_loadedRessources[i], function1);
        if (ret.isNumber())
        {
            return ret.toInt();
        }
    }
    return 0;
}

qint32 BuildingSpriteManager::getBuildingGroup(QString id)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBuildingGroup";
    QJSValue ret = pInterpreter->doFunction(id, function1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    return 0;
}

QString BuildingSpriteManager::getBuildingGroupName(qint32 group)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBuildingGroupName";
    QJSValueList args({QJSValue(group)});
    QJSValue ret = pInterpreter->doFunction("BUILDING", function1, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    return tr("Unknown");
}

QStringList BuildingSpriteManager::getBuildingsSorted()
{
    QStringList sortedBuildings;
    QVector<qint32> buildingGroups;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBuildingGroupSort";
    QJSValue ret = pInterpreter->doFunction("BUILDING", function1);
    auto list = ret.toVariant().toList();
    for (const auto & buildingType : qAsConst(list))
    {
        qint32 value = buildingType.toInt();
        if (!buildingGroups.contains(value))
        {
            buildingGroups.append(value);
        }
    }

    for (qint32 i = 0; i < m_loadedRessources.size(); i++)
    {
        qint32 buildingGroup = getBuildingGroup(i);
        if (!buildingGroups.contains(buildingGroup))
        {
            buildingGroups.append(buildingGroup);
        }
    }
    for (qint32 i2 = 0; i2 < buildingGroups.size(); i2++)
    {
        for (qint32 i = 0; i < m_loadedRessources.size(); i++)
        {
            if (getBuildingGroup(i) == buildingGroups[i2])
            {
                sortedBuildings.append(m_loadedRessources[i]);
            }
        }
    }
    return sortedBuildings;
}
