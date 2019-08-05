#ifndef TERRAININFO_H
#define TERRAININFO_H

#include <QObject>

#include "oxygine-framework.h"

#include "game/building.h"
#include "game/unit.h"
#include "game/terrain.h"

class TerrainInfo;
typedef oxygine::intrusive_ptr<TerrainInfo> spTerrainInfo;

class TerrainInfo : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit TerrainInfo(Terrain* pTerrain, qint32 width);

signals:

public slots:

private:
    spPlayer m_pPlayer;
};

#endif // TERRAININFO_H
