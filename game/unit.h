#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QVector>
#include "oxygine-framework.h"

#include "game/smartpointers.h"
#include "coreengine/fileserializable.h"

class GameAction;

class Unit : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT
    Q_PROPERTY(float hp READ getHp WRITE setHp)
    Q_PROPERTY(qint32 ammo1 READ getAmmo1 WRITE setAmmo1)
    Q_PROPERTY(qint32 maxAmmo1 READ getMaxAmmo1 WRITE setMaxAmmo1)
    Q_PROPERTY(qint32 ammo2 READ getAmmo2 WRITE setAmmo2)
    Q_PROPERTY(qint32 maxAmmo2 READ getMaxAmmo2 WRITE setMaxAmmo2)
    Q_PROPERTY(qint32 fuel READ getFuel WRITE setFuel)
    Q_PROPERTY(qint32 maxFuel READ getMaxFuel WRITE setMaxFuel)
    Q_PROPERTY(qint32 baseMovementPoints READ getBaseMovementPoints WRITE setBaseMovementPoints)
    Q_PROPERTY(qint32 capturePoints READ getCapturePoints WRITE setCapturePoints)
public:
    /**
     * @brief Unit only for deserialization
     */
    explicit Unit() = default;

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
        return 3;
    }
    qint32 getHp() const;
    void setHp(const qint32 &value);

    qint32 getAmmo1() const;
    void setAmmo1(const qint32 &value);

    qint32 getMaxAmmo1() const;
    void setMaxAmmo1(const qint32 &value);

    qint32 getAmmo2() const;
    void setAmmo2(const qint32 &value);

    qint32 getMaxAmmo2() const;
    void setMaxAmmo2(const qint32 &value);

    qint32 getFuel() const;
    void setFuel(const qint32 &value);

    qint32 getMaxFuel() const;
    void setMaxFuel(const qint32 &value);

    qint32 getCapturePoints() const;
    void setCapturePoints(const qint32 &value);

    /**
     * @brief initUnit loads all default unit value
     */
    void initUnit();
    /**
     * @brief getMovementPoints the movement points this unit can move
     * @return
     */
    qint32 getMovementPoints();
    /**
     * @brief getBaseMovementPoints the base movement points of this unit
     * @return
     */
    qint32 getBaseMovementPoints() const;
    /**
     * @brief setBaseMovementPoints the base movement points of this unit
     * @param value
     */
    void setBaseMovementPoints(const qint32 &value);
    /**
     * @brief getActionList
     * @return the string id list of actions this units can perform
     */
    QStringList getActionList();
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
    /**
     * @brief refill fills up all ammo and fuel to max
     */
    void refill();
    /**
     * @brief setHasMoved  changes if the unit has been moved or not
     * @param value
     */
    void setHasMoved(bool value);
    /**
     * @brief getHasMoveed return if this unit has moved or not
     * @return
     */
    bool getHasMoved();
    /**
     * @brief moveUnit moves the unit to the target position of this unit
     * @param pAction
     */
    void moveUnitAction(GameAction* pAction);
    /**
     * @brief moveUnit moves the unit to the target path
     * @param movePath
     */
    void moveUnit(QVector<QPoint> movePath);
    /**
     * @brief increaseCapturePoints increases the capture points of this unit based on units hp and ko owner
     */
    void increaseCapturePoints();
private:
    /**
     * @brief updateSprites reloads all sprites
     */
    void updateSprites();

    QVector<oxygine::spSprite> m_pUnitWaitSprites;
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
    float hp{10};
    qint32 ammo1{-1};
    qint32 maxAmmo1{-1};
    qint32 ammo2{-1};
    qint32 maxAmmo2{-1};
    qint32 fuel{-1};
    qint32 maxFuel{-1};
    qint32 m_Rank{0};
    qint32 baseMovementPoints{0};
    bool m_Moved{false};
    QVector<spUnit> m_TransportUnits;

    qint32 capturePoints{0};
};

#endif // UNIT_H

