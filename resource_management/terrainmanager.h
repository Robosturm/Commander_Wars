#ifndef TERRAINMANAGER_H
#define TERRAINMANAGER_H

#include <QObject>

#include "oxygine-framework.h"

class TerrainManager : public QObject, public oxygine::Resources
{
    Q_OBJECT
public:
    static TerrainManager* getInstance();

signals:

public slots:
    void loadAll();
    /**
     * @brief loadTerrain
     * @param TerrainID the terrain that should be loaded.
     * @return true if all is fine
     */
    bool loadTerrain(const QString& TerrainID);
    /**
     * @brief getTerrainID
     * @param position the position in the loaded terrain list
     * @return the id of the terrain at the given index
     */
    inline QString getTerrainID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedTerrains.size()))
        {
            return m_loadedTerrains.at(position);
        }
        return "";
    }
    /**
     * @brief getTerrainCount
     * @return the amount of terrains loaded at the current time
     */
    inline qint32 getTerrainCount()
    {
        return m_loadedTerrains.size();
    }
    void reset();
private:
    QStringList m_loadedTerrains;
    explicit TerrainManager();
    virtual ~TerrainManager() = default;
    static TerrainManager* m_pInstance;
};



#endif // TERRAINMANAGER_H
