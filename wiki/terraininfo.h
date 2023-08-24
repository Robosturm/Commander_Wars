#ifndef TERRAININFO_H
#define TERRAININFO_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#include "game/terrain.h"
#include "game/player.h"

class GameMap;
class TerrainInfo;
using spTerrainInfo = std::shared_ptr<TerrainInfo>;

class TerrainInfo final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit TerrainInfo(GameMap* pMap, Terrain* pTerrain, qint32 width);
    ~TerrainInfo() = default;
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
    GameMap* m_pMap{nullptr};
};

#endif // TERRAININFO_H
