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
    explicit Building(const QString& BuildingID);
    /**
     * @brief setOwner changes the owner ship of this building
     * @param pOwner
     */
    void setOwner(spPlayer pOwner);
    /**
     * @brief updateBuildingSprites updates all sprites of this building
     */
    void updateBuildingSprites();
    /**
     * @brief getSpOwner
     * @return  smart pointer to the owner
     */
    spPlayer getSpOwner();
    /**
     * @brief setTerrain
     * @param pTerrain smart pointer to the terrain this building is placed on
     */
    inline void setTerrain(spTerrain pTerrain)
    {
        m_Terrain = pTerrain;
    }
    /**
     * @brief canBuildingBePlaced
     * @param terrainID
     * @return if the building can be placed on the given terrain
     */
    bool canBuildingBePlaced(QString terrainID);
signals:

public slots:
    /**
     * @brief loadSprite loads a sprite for this building
     * @param sprite the sprite id
     * @param addPlayerColor true for adding player color to the sprite
     */
    void loadSprite(QString sprite, bool addPlayerColor);
    /**
     * @brief getOwnerID
     * @return the player owner index of this building
     */
    qint32 getOwnerID();
    /**
     * @brief getOwner
     * @return the pointer to the owner of this building
     */
    Player* getOwner();
    /**
     * @brief getBuildingID
     * @return the string if of this building
     */
    inline QString getBuildingID()
    {
        return m_BuildingID;
    }
    /**
     * @brief getX
     * @return  x coordinates of this unit
     */
    qint32 getX() const;
    /**
     * @brief getY
     * @return  y coordinates of this unit
     */
    qint32 getY() const;
private:
    QVector<oxygine::spSprite> m_pBuildingSprites;
    /**
     * @brief m_BuildingID the id of this building
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
