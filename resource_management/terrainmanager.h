#ifndef TERRAINMANAGER_H
#define TERRAINMANAGER_H

#include "resource_management/ressourcemanagement.h"

class TerrainManager final : public QObject, public RessourceManagement<TerrainManager>
{
    Q_OBJECT
public:
    /**
     * @brief getTerrainGroup
     * @param i
     * @return
     */
    Q_INVOKABLE qint32 getTerrainGroup(qint32 i);
    /**
     * @brief getTerrainGroup
     * @param id
     * @return
     */
    Q_INVOKABLE qint32 getTerrainGroup(QString id);
    /**
     * @brief getTerrainGroupName
     * @param group
     * @return
     */
    Q_INVOKABLE QString getTerrainGroupName(qint32 group);
    /**
     * @brief getName
     * @param id
     * @return
     */
    Q_INVOKABLE QString getName(const QString id);
    Q_INVOKABLE void removeRessource(QString id);
    /**
     * @brief getTerrainsSorted
     * @return
     */
    Q_INVOKABLE QStringList getTerrainsSorted();
    /**
     * @brief getCount
     * @return
     */
    Q_INVOKABLE qint32 getTerrainCount()
    {
        return m_loadedRessources.size();
    }
    /**
     * @brief getFittingResAnim
     * @param spriteIdStart
     * @param spriteIdEnd
     * @return
     */
    Q_INVOKABLE QString getFittingResAnim(const QString spriteIdStart, const QString spriteIdEnd) const;
protected:
    friend MemoryManagement;
    TerrainManager();
private:
    ~TerrainManager() = default;
};

Q_DECLARE_INTERFACE(TerrainManager, "TerrainManager");

#endif // TERRAINMANAGER_H
