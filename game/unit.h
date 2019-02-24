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
public:
    enum class Priorities
    {
        Waiting = 10,
        Icons = 20
    };

    /**
     * @brief Unit only for deserialization
     */
    explicit Unit() = default;

    explicit Unit(QString unitID, spPlayer pOwner);

    virtual ~Unit();
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


    /**
     * @brief initUnit loads all default unit value
     */
    void initUnit();

    /**
     * @brief getActionList
     * @return the string id list of actions this units can perform
     */
    QStringList getActionList();


signals:

public slots:
    /**
     * @brief getTerrainDefenseModifier the bonus defense for this unit
     * @return
     */
    qint32 getTerrainDefenseModifier(QPoint position);
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
    float getHp() const;
    void setHp(const float &value);
    qint32 getHpRounded() const;

    qint32 getAmmo1() const;
    void setAmmo1(const qint32 &value);
    bool hasAmmo1() const;

    qint32 getMaxAmmo1() const;
    void setMaxAmmo1(const qint32 &value);

    QString getWeapon1ID() const;
    void setWeapon1ID(const QString &value);

    qint32 getAmmo2() const;
    void setAmmo2(const qint32 &value);
    bool hasAmmo2() const;

    qint32 getMaxAmmo2() const;
    void setMaxAmmo2(const qint32 &value);

    QString getWeapon2ID() const;
    void setWeapon2ID(const QString &value);

    qint32 getFuel() const;
    void setFuel(const qint32 &value);

    qint32 getMaxFuel() const;
    void setMaxFuel(const qint32 &value);

    qint32 getCapturePoints() const;
    void setCapturePoints(const qint32 &value);

    qint32 getCosts() const;

    qint32 getMinRange() const;
    void setMinRange(const qint32 &value);

    qint32 getMaxRange();
    qint32 getMaxRange(QPoint position);
    void setMaxRange(const qint32 &value);

    qint32 getVision() const;
    void setVision(const qint32 &value);
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
     * @brief getPosition
     * @return
     */
    QPoint getPosition() const;
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
     * @brief removeUnit removes this from game
     */
    void removeUnit();
    /**
     * @brief killUnit removes the unit from the game and let it explode
     */
    void killUnit();
    /**
     * @brief increaseCapturePoints increases the capture points of this unit based on units hp and ko owner
     */
    void increaseCapturePoints();
    /**
     * @brief loadIcon
     * @param iconID icon we want to load
     * @param x position of the icon
     * @param y position of the icon
     */
    void loadIcon(QString iconID, qint32 x, qint32 y);
    /**
     * @brief unloadIcon removes the given icon from this unit
     * @param iconID
     */
    void unloadIcon(QString iconID);
    /**
     * @brief startOfTurn
     */
    void startOfTurn();
    /**
     * @brief getTerrain
     * @return
     */
    Terrain* getTerrain();
    /**
     * @brief canMoveAndFire
     * @return if this unit can move and fire
     */
    bool canMoveAndFire(QPoint position);
    /**
     * @brief loadUnit adds a unit to the transport list
     * @param pUnit
     */
    void loadUnit(Unit* pUnit);
    /**
     * @brief getLoadedUnit
     * @param index
     */
    Unit* getLoadedUnit(qint32 index);
    /**
     * @brief unloadUnit removes a unit from the transport list
     * @param index
     */
    void unloadUnit(Unit* pUnit, QPoint position);
    /**
     * @brief getLoadedUnitCount
     * @return number of units loaded by this unit
     */
    qint32 getLoadedUnitCount();
    /**
     * @brief getBonusOffensive
     * @param position
     * @return offensive bonus at this position
     */
    qint32 getBonusOffensive(QPoint position, Unit* pDefender, QPoint defPosition);
    /**
     * @brief getBonusDeffensive
     * @param position
     * @return defense bonus at this position
     */
    qint32 getBonusDefensive(QPoint position, Unit* pAttacker, QPoint atkPosition);
    /**
     * @brief getAttackHpBonus
     * @param position
     * @return
     */
    qint32 getAttackHpBonus(QPoint position);
    /**
     * @brief getMisfortune
     * @param position
     * @return
     */
    qint32 getBonusMisfortune(QPoint position);
    /**
     * @brief getLuck
     * @param position
     * @return
     */
    qint32 getBonusLuck(QPoint position);
protected:
    /**
     * @brief updateIconTweens creates the visibility toogle tweens for all icons
     */
    void updateIconTweens();
private:
    /**
     * @brief updateSprites reloads all sprites
     */
    void updateSprites();

    QVector<oxygine::spSprite> m_pUnitWaitSprites;
    QVector<oxygine::spSprite> m_pUnitSprites;
    /**
     * @brief m_pIconSprites vector containg all icon sprites
     */
    QVector<oxygine::spSprite> m_pIconSprites;
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
    QString weapon1ID;
    qint32 ammo2{-1};
    qint32 maxAmmo2{-1};
    QString weapon2ID;
    qint32 fuel{-1};
    qint32 maxFuel{-1};
    qint32 m_Rank{0};
    qint32 baseMovementPoints{0};
    bool m_Moved{false};
    QVector<spUnit> m_TransportUnits;

    qint32 capturePoints{0};
    qint32 minRange{1};
    qint32 maxRange{-1};
    qint32 vision{1};
};

#endif // UNIT_H

