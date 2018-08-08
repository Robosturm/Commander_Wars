#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QVector>
#include "oxygine-framework.h"

#include "game/smartpointers.h"

class Unit : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Unit(QString unitID, spPlayer pOwner);
    /**
     * @brief setOwner changes the owner ship of this unit
     * @param pOwner
     */
    void setOwner(spPlayer pOwner);
    /**
     * @brief getSpOwner
     * @return  smart pointer to the owner
     */
    spPlayer getSpOwner();
    /**
     * @brief setTerrain
     * @param pTerrain smart pointer to the terrain this building is placed on
     */
    void setTerrain(spTerrain pTerrain);
signals:

public slots:
    /**
     * @brief loadSprites loads the given sprite
     * @param spriteID
     */
    void loadSprite(QString spriteID, bool addPlayerColor);
    /**
     * @brief getOwner
     * @return the pointer to the owner of this unit
     */
    Player* getOwner();
    /**
     * @brief getMovementType the movement type id
     * @return
     */
    QString getMovementType();
    /**
     * @brief getUnitID the unit id
     * @return
     */
    QString getUnitID();
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
    /**
     * @brief updateSprites reloads all sprites
     */
    void updateSprites();

    QVector<oxygine::spSprite> m_pUnitSprites;
    /**
     * @brief m_UnitID the id of this unit
     */
    QString m_UnitID;
    /**
     * @brief m_Owner our owner a nullptr means we're a neutral building
     */
    spPlayer m_Owner;
    /**
     * @brief m_Terrain the terrain at which we are placed
     */
    spTerrain m_Terrain;
};

#endif // UNIT_H
