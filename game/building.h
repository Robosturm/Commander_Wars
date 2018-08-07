#ifndef BUILDING_H
#define BUILDING_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"
#include "game/smartpointers.h"

class Building : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Building(const QString& BuildingID, qint32 x, qint32 y);

    void setOwner(spPlayer pOwner);
    void updateBuildingSprites();
    spPlayer getSpOwner();
    inline void setTerrain(spTerrain pTerrain)
    {
        m_Terrain = pTerrain;
    }
    bool canBuildingBePlaced(QString terrainID);
signals:

public slots:
    void loadSprite(QString sprite, bool addPlayerColor);
    qint32 getOwnerID();
    Player* getOwner();
    inline QString getBuildingID()
    {
        return m_BuildingID;
    }
private:
    QVector<oxygine::spSprite> m_pBuildingSprites;
    /**
     * @brief m_BuildingID the id of this terrain
     */
    QString m_BuildingID;
    /**
     * @brief m_Owner our owner a nullptr means we're a neutral building
     */
    spPlayer m_Owner;
    /**
     * @brief m_Terrain the terrain at which we are placed
     */
    spTerrain m_Terrain;
};

#endif // BUILDING_H
