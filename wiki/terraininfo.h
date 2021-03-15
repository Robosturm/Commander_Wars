#ifndef TERRAININFO_H
#define TERRAININFO_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "game/building.h"
#include "game/unit.h"
#include "game/terrain.h"
#include "game/player.h"

class TerrainInfo;
typedef oxygine::intrusive_ptr<TerrainInfo> spTerrainInfo;

class TerrainInfo : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit TerrainInfo(Terrain* pTerrain, qint32 width);

signals:
    /**
     * @brief sigShowLink
     * @param pageID
     */
    void sigShowLink(QString pageID);
public slots:
    /**
     * @brief showLink
     * @param pageID
     */
    void showLink(QString pageID);
private:
    void showUnitList(QStringList productionList, qint32& y, qint32 width);

private:
    spPlayer m_pPlayer;
};

#endif // TERRAININFO_H
