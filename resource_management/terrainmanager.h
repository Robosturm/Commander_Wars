#ifndef TERRAINMANAGER_H
#define TERRAINMANAGER_H

#include "resource_management/RessourceManagement.h"

class TerrainManager : public QObject, public RessourceManagement<TerrainManager>
{
    Q_OBJECT
public:
    /**
     * @brief getTerrainGroup
     * @param i
     * @return
     */
    qint32 getTerrainGroup(qint32 i);
    /**
     * @brief getTerrainsSorted
     * @return
     */
    QStringList getTerrainsSorted();
protected:
    friend RessourceManagement<TerrainManager>;
    TerrainManager();
private:
    virtual ~TerrainManager() = default;
};

#endif // TERRAINMANAGER_H
