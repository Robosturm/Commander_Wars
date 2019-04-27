#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QVector>
#include "oxygine-framework.h"

#include "game/smartpointers.h"
#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"

#include "game/GameEnums.h"

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
    explicit Unit();

    explicit Unit(QString unitID, Player* pOwner);

    virtual ~Unit();
    /**
     * @brief setOwner changes the owner ship of this unit
     * @param pOwner
     */
    void setOwner(Player* pOwner);
    /**
     * @brief setTerrain
     * @param pTerrain smart pointer to the terrain this building is placed on
     */
    void setTerrain(Terrain* pTerrain);
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 6;
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
    /**
     * @brief updateIcons updates icons that can be modified by the co.
     * E.g Transport sign and health point sign
     */
    void updateIcons(Player* pPlayer);


signals:

public slots:
    /**
     * @brief getHpHidden
     * @param pPlayer
     * @return
     */
    bool getHpHidden(Player* pPlayer);
    /**
     * @brief getIgnoreUnitCollision
     * @return
     */
    bool getIgnoreUnitCollision() const;
    /**
     * @brief setIgnoreUnitCollision
     * @param IgnoreUnitCollision
     */
    void setIgnoreUnitCollision(bool IgnoreUnitCollision);
    /**
     * @brief getName
     * @return
     */
    QString getName();
    /**
     * @brief getTerrainDefenseModifier the bonus defense for this unit
     * @return
     */
    qint32 getTerrainDefenseModifier(QPoint position);
    /**
     * @brief getFirstStrike
     * @param position
     * @return
     */
    bool getFirstStrike(QPoint position);
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
    void reduceAmmo1(qint32 value);

    qint32 getMaxAmmo1() const;
    void setMaxAmmo1(const qint32 &value);

    QString getWeapon1ID() const;
    void setWeapon1ID(const QString &value);

    qint32 getAmmo2() const;
    void setAmmo2(const qint32 &value);
    bool hasAmmo2() const;
    void reduceAmmo2(qint32 value);

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

    bool getHidden() const;
    void setHidden(bool Hidden);
    /**
     * @brief isStealthed checks if the unit is invisible for the given player. Either by terrain, Out of Vision or Stealthmode
     * @param pPlayer
     * @return
     */
    bool isStealthed(Player* pPlayer, bool ignoreOutOfVisionRange = false, qint32 testX = -1, qint32 testY = -1);

    GameEnums::UnitRanks getUnitRank() const;
    void setUnitRank(const GameEnums::UnitRanks &UnitRank);

    qint32 getVision(QPoint position);
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
     * @brief getLoadingPlace
     * @return
     */
    qint32 getLoadingPlace();
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
     * @brief moveUnitToField
     * @param x
     * @param y
     */
    void moveUnitToField(qint32 x, qint32 y);
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
    void increaseCapturePoints(QPoint position);
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
    qint32 getBonusOffensive(QPoint position, Unit* pDefender, QPoint defPosition, bool isDefender);
    /**
     * @brief getDamageReduction
     * @param pAttacker
     * @param position
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    float getDamageReduction(float damage, Unit* pAttacker, QPoint position, qint32 attackerBaseHp,
                              QPoint defPosition, bool isDefender);
    /**
     * @brief getTrueDamage
     * @param damage
     * @param pAttacker
     * @param position
     * @param attackerBaseHp
     * @param defPosition
     * @param isDefender
     * @return
     */
    float getTrueDamage(float damage, Unit* pAttacker, QPoint position, qint32 attackerBaseHp,
                              QPoint defPosition, bool isDefender);
    /**
     * @brief getTerrainDefense
     * @return
     */
    qint32 getTerrainDefense();
    /**
     * @brief getBonusDeffensive
     * @param position
     * @return defense bonus at this position
     */
    qint32 getBonusDefensive(QPoint position, Unit* pAttacker, QPoint atkPosition, bool isDefender);
    /**
     * @brief useTerrainDefense
     * @return
     */
    bool useTerrainDefense();
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
    /**
     * @brief getUnitCosts
     * @return
     */
    qint32 getUnitCosts();
    /**
     * @brief getRepairBonus
     * @return
     */
    qint32 getRepairBonus(QPoint position);
    /**
     * @brief setUnitVisible
     * @param value
     */
    void setUnitVisible(bool value);
    /**
     * @brief makeCOUnit
     * @param co
     */
    void makeCOUnit(quint8 co);
    /**
     * @brief createCORange
     */
    void createCORange(qint32 coRange);
    /**
     * @brief updateSprites reloads all sprites
     */
    void updateSprites();
    /**
     * @brief getFuelCostModifier
     * @param pUnit
     * @param costs
     * @return
     */
    qint32 getFuelCostModifier(QPoint position, qint32 costs);
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief getTerrainAnimationBase
     * @return
     */
    QString getTerrainAnimationBase();
    /**
     * @brief getTerrainAnimationForeground
     * @return
     */
    QString getTerrainAnimationForeground();
    /**
     * @brief getTerrainAnimationBackground
     * @return
     */
    QString getTerrainAnimationBackground();
    /**
     * @brief isAttackable
     * @param pDefender
     * @return
     */
    bool isAttackable(Unit* pDefender, bool ignoreOutOfVisionRange = false);
    /**
     * @brief canMoveOver
     * @param x
     * @param y
     * @return
     */
    bool canMoveOver(qint32 x, qint32 y);
    /**
     * @brief getUnitValue
     * @return
     */
    qint32 getUnitValue();

protected:
    /**
     * @brief updateIconTweens creates the visibility toogle tweens for all icons
     */
    void updateIconTweens();
private:
    QVector<oxygine::spSprite> m_pUnitWaitSprites;
    QVector<oxygine::spSprite> m_pUnitSprites;
    /**
     * @brief m_pIconSprites vector containg all icon sprites
     */
    QVector<oxygine::spSprite> m_pIconSprites;
    /**
     * @brief m_CORange
     */
    oxygine::spActor m_CORange;
    /**
     * @brief m_UnitID the id of this unit
     */
    QString m_UnitID;
    /**
     * @brief m_Owner our owner a nullptr means we're a neutral building
     */
    Player* m_pOwner{nullptr};
    /**
     * @brief m_Terrain the terrain at which we are placed
     */
    Terrain* m_pTerrain{nullptr};
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
    qint32 baseMovementPoints{0};
    bool m_Moved{false};
    QVector<spUnit> m_TransportUnits;
    qint32 capturePoints{0};
    GameEnums::UnitRanks m_UnitRank{GameEnums::UnitRank_None};

    bool m_Hidden{false};

    bool m_IgnoreUnitCollision{false};

    qint32 minRange{1};
    qint32 maxRange{-1};
    qint32 vision{1};

    ScriptVariables m_Variables;
};

#endif // UNIT_H

