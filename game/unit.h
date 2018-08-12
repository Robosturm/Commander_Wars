#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QVector>
#include "oxygine-framework.h"

#include "game/smartpointers.h"
#include "coreengine/fileserializable.h"

class Unit : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT
public:
    /**
     * @brief Unit only for deserialization
     */
    explicit Unit();

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
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serialize(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserialize(QDataStream& pStream) override;
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 1;
    }
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
    // basic data of this unit
    qint32 m_Hp{10};
    qint32 m_Ammo1{-1};
    qint32 m_Ammo2{-1};
    qint32 m_Fuel{-1};
    qint32 m_Rank{0};

};

#endif // UNIT_H
