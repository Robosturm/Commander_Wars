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
    bool loadTerrain(const QString& TerrainID);
    void reset();
private:
    QStringList m_loadedTerrains;
    explicit TerrainManager();
    virtual ~TerrainManager() = default;
    static TerrainManager* m_pInstance;
};



#endif // TERRAINMANAGER_H
