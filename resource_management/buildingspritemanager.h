#ifndef BUILDINGSPRITEMANAGER_H
#define BUILDINGSPRITEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class BuildingSpriteManager final : public QObject, public RessourceManagement<BuildingSpriteManager>
{
    Q_OBJECT
public slots:
    void removeRessource(QString id);
    /**
     * @brief getLoadedRessources
     * @return
     */
    QStringList getLoadedBuildings()
    {
        return m_loadedRessources;
    }
    /**
     * @brief getCount
     * @return
     */
    qint32 getBuildingCount()
    {
        return m_loadedRessources.size();
    }
    QStringList getBuildingsSorted();
    /**
     * @brief getBuildingGroup
     * @param id
     * @return
     */
    qint32 getBuildingGroup(QString id);
    /**
     * @brief getTerrainGroup
     * @param i
     * @return
     */
    qint32 getBuildingGroup(qint32 i);
    /**
     * @brief getBuildingGroupName
     * @param group
     * @return
     */
    QString getBuildingGroupName(qint32 group);
protected:
    friend RessourceManagement<BuildingSpriteManager>;
    BuildingSpriteManager();
private:
    ~BuildingSpriteManager() = default;
};

Q_DECLARE_INTERFACE(BuildingSpriteManager, "BuildingSpriteManager");

#endif // BUILDINGSPRITEMANAGER_H
