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
    qint32 getTerrainGroup(qint32 i);
    /**
     * @brief getTerrainGroup
     * @param id
     * @return
     */
    qint32 getTerrainGroup(QString id);
    /**
     * @brief getTerrainGroupName
     * @param group
     * @return
     */
    QString getTerrainGroupName(qint32 group);
    /**
     * @brief getName
     * @param id
     * @return
     */
    QString getName(const QString & id);
public slots:
    void removeRessource(QString id);
    /**
     * @brief getTerrainsSorted
     * @return
     */
    QStringList getTerrainsSorted();
    /**
     * @brief getCount
     * @return
     */
    qint32 getTerrainCount()
    {
        return m_loadedRessources.size();
    }
    /**
     * @brief getFittingResAnim
     * @param spriteIdStart
     * @param spriteIdEnd
     * @return
     */
    QString getFittingResAnim(const QString & spriteIdStart, const QString & spriteIdEnd) const;
protected:
    friend RessourceManagement<TerrainManager>;
    TerrainManager();
private:
    ~TerrainManager() = default;
};

Q_DECLARE_INTERFACE(TerrainManager, "TerrainManager");

#endif // TERRAINMANAGER_H
