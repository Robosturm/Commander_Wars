#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QVector>

#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/LUPDATE_MACROS.h"

#include "game/GameEnums.h"

#include "objects/base/tooltip.h"

class Player;
class Terrain;
class Unit;
class GameAction;
class GameAnimation;
class GameMap;
class UnitPathFindingSystem;
using spGameAction = oxygine::intrusive_ptr<GameAction>;
using spUnit = oxygine::intrusive_ptr<Unit>;

class Unit final : public Tooltip, public FileSerializable
{
    Q_OBJECT
public:
    static constexpr qreal MAX_UNIT_HP = 10.0f;
    static constexpr qreal DAMAGE_100 = 100.0f;

    struct IconDuration
    {
        QString icon;
        qint32 x;
        qint32 y;
        qint32 duration;
        qint32 player;
    };

    ENUM_CLASS Priorities
    {
        Colored = 0,
        Outline,
        Waiting,
        OutlineWaiting,
        Icons
    };

    enum ModdingFlags
    {
        None = 0x0,
        FlagAmmo1 = 0x1,
        FlagAmmo2 = 0x2,
        FlagFuel = 0x4,
    };

    /**
     * @brief Unit only for deserialization
     */
    explicit Unit(GameMap* pMap);
    explicit Unit(const QString & unitID, Player* pOwner, bool aquireId, GameMap* pMap);
    ~Unit();
    /**
     * @brief setTerrain
     * @param pTerrain smart pointer to the terrain this building is placed on
     */
    void setTerrain(Terrain* pTerrain);
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    void serializeObject(QDataStream& pStream, bool forHash) const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief deserializer
     * @param pStream
     * @param fast
     */
    void deserializer(QDataStream& pStream, bool fast);
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 23;
    }


    /**
     * @brief initUnit loads all default unit value
     */
    void initUnit();
    /**
     * @brief hasAction
     * @param action
     * @return
     */
    bool hasAction(const QString & action);
    /**
     * @brief updateIcons updates icons that can be modified by the co.
     * E.g Transport sign and health point sign
     */
    void updateIcons(Player* pPlayer);
    /**
     * @brief updateRankInfoVisibility
     * @param pPlayer
     */
    void updateRankInfoVisibility(Player* pPlayer);
    /**
     * @brief setModdingFlags
     * @param value
     */
    void setModdingFlags(ModdingFlags value);
    /**
     * @brief getModdingFlags
     * @return
     */
    Unit::ModdingFlags getModdingFlags() const;
    /**
     * @brief applyMod
     */
    void applyMod();

    void setVirtuellX(qint32 value)
    {
        m_virtuellX = value;
    }
    void setVirtuellY(qint32 value)
    {
        m_virtuellY = value;
    }
    QVector<QPoint> getMultiTurnPath() const;
    void setMultiTurnPath(const QVector<QPoint> &MultiTurnPath);
    /**
     * @brief isValid
     * @return
     */
    bool isValid();
    /**
     * @brief syncAnimation
     * @param syncTime
     */
    void syncAnimation(oxygine::timeMS syncTime);
    /**
     * @brief Unit::getSortValues
     * @return
     */
    inline const QVector<qint32> &getSortValues() const
    {
        return m_sortValues;
    }
    /**
     * @brief Unit::setSortValues
     * @param newSortValues
     */
    inline void setSortValues(const QVector<qint32> &newSortValues)
    {
        m_sortValues = newSortValues;
    }
    /**
     * @brief Unit::getLoadedUnits
     * @return
     */
    QVector<spUnit> & getLoadedUnits()
    {
        return m_TransportUnits;
    }
    /**
     * @brief setMapForExtending
     * @param newMap
     */
    bool setMapForExtending(GameMap * newMap);
    /**
     * @brief isStatusStealthedAndInvisible
     * @param pOwner
     * @return
     */
    bool isStatusStealthedAndInvisible(Player* pPlayer, bool & terrainHide);

    // js invokable functions
    /**
     * @brief getX
     * @return  x coordinates of this unit
     */
    Q_INVOKABLE qint32 getX() const;
    /**
     * @brief getY
     * @return  y coordinates of this unit
     */
    Q_INVOKABLE qint32 getY() const;
    /**
     * @brief createUnitPathFindingSystem Note: the path finding system needs to be deleted by the caller using remove()
     * @return a path finding system that is explored for this unit using the given player information
     */
    Q_INVOKABLE UnitPathFindingSystem* createUnitPathFindingSystem(Player* pPlayer);
    /**
     * @brief getActionList
     * @return the string id list of actions this units can perform
     */
    Q_INVOKABLE QStringList getActionList();
    /**
     * @brief getBaseActionList
     * @return
     */
    Q_INVOKABLE QStringList getBaseActionList();
    /**
     * @brief getShowInEditor
     * @param unitId
     * @return if the given unit should be shown in the editor
     */
    Q_INVOKABLE static bool getShowInEditor(QString unitId);
    /**
     * @brief getWeatherImmune
     * @return
     */
    Q_INVOKABLE bool getWeatherImmune();
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap *getMap() const;
    /**
     * @brief getCOSpecificUnit
     * @return
     */
    Q_INVOKABLE bool getCOSpecificUnit();
    /**
     * @brief setVirtualHpValue
     * @param value
     */
    Q_INVOKABLE void setVirtualHpValue(const qreal value);
    /**
     * @brief getVirtualHp
     * @return
     */
    Q_INVOKABLE qreal getVirtualHp() const;
    /**
     * @brief getVirtualHpValue
     * @return
     */
    Q_INVOKABLE qreal getVirtualHpValue() const;
    /**
     * @brief transformUnit
     * @param unitID
     */
    Q_INVOKABLE void transformUnit(const QString unitID);
    /**
     * @brief getCursorInfoRange
     * @return
     */
    Q_INVOKABLE qint32 getCursorInfoRange() const;
    /**
     * @brief setCursorInfoRange
     * @param newCursorInfoRange
     */
    Q_INVOKABLE void setCursorInfoRange(qint32 newCursorInfoRange);
    /**
     * @brief getAiMovePath
     * @return
     */
    Q_INVOKABLE QVector<QPoint> getAiMovePath() const;
    /**
     * @brief setAiMovePath
     * @param AiMovePath
     */
    Q_INVOKABLE void setAiMovePath(const QVector<QPoint> AiMovePath);
    /**
     * @brief addAiMovePathPoint
     * @param point
     */
    Q_INVOKABLE void addAiMovePathPoint(const QPoint point);
    /**
     * @brief setAiMovePathPoint
     * @param index
     * @param point
     */
    Q_INVOKABLE void setAiMovePathPoint(qint32 index, const QPoint point);
    /**
     * @brief removeLastAiMovePathPoint
     */
    Q_INVOKABLE void removeLastAiMovePathPoint();
    /**
     * @brief removeFirstAiMovePathPoint
     */
    Q_INVOKABLE void removeFirstAiMovePathPoint();
    /**
     * @brief onMap
     * @return true if the unit is on the map and not loaded by another unit
     */
    Q_INVOKABLE bool onMap();
    /**
     * @brief getCustomName
     * @return
     */
    Q_INVOKABLE QString getCustomName() const;
    /**
     * @brief setCustomName
     * @param customName
     */
    Q_INVOKABLE void setCustomName(const QString customName);
    /**
     * @brief getVisionHigh
     * @return
     */
    Q_INVOKABLE qint32 getVisionHigh() const;
    /**
     * @brief setVisionHigh
     * @param VisionHigh
     */
    Q_INVOKABLE void setVisionHigh(const qint32 VisionHigh);
    /**
     * @brief getTotalVisionHigh
     * @return
     */
    Q_INVOKABLE qint32 getTotalVisionHigh();
    /**
     * @brief postBattleActions
     * @param damage
     * @param pUnit
     * @param gotAttacked
     */
    Q_INVOKABLE void postBattleActions(qreal damage, Unit* pUnit, bool gotAttacked, qint32 weapon, GameAction* pAction);
    /**
     * @brief getDescription
     * @return
     */
    Q_INVOKABLE QString getDescription();
    /**
     * @brief hasWeapons
     * @return
     */
    Q_INVOKABLE bool hasWeapons();
    /**
     * @brief getUniqueID
     * @return returns the map wide unique id of this unit.
     */
    Q_INVOKABLE qint32 getUniqueID() const;
    /**
     * @brief spawnUnit
     * @param unitID
     * @return
     */
    Q_INVOKABLE Unit* spawnUnit(const QString unitID);
    /**
     * @brief getAiMode
     * @return
     */
    Q_INVOKABLE GameEnums::GameAi getAiMode() const;
    /**
     * @brief setAiMode
     * @param AiMode
     */
    Q_INVOKABLE void setAiMode(const GameEnums::GameAi AiMode);
    /**
     * @brief getTransportHidden
     * @param pPlayer
     * @return if the transported units of this unit are hidden from the given player
     */
    Q_INVOKABLE bool getTransportHidden(Player* pPlayer);
    /**
     * @brief getHpHidden
     * @param pPlayer
     * @return
     */
    Q_INVOKABLE bool getHpHidden(Player* pPlayer);
    /**
     * @brief getRankInfoHidden
     * @param pPlayer
     * @return
     */
    Q_INVOKABLE bool getRankInfoHidden(Player* pPlayer);
    /**
     * @brief getPerfectHpView
     * @param pPlayer
     * @return
     */
    Q_INVOKABLE bool getPerfectHpView(Player* pPlayer);
    /**
     * @brief getIgnoreUnitCollision
     * @return
     */
    Q_INVOKABLE bool getIgnoreUnitCollision() const;
    /**
     * @brief setIgnoreUnitCollision
     * @param IgnoreUnitCollision
     */
    Q_INVOKABLE void setIgnoreUnitCollision(bool IgnoreUnitCollision);
    /**
     * @brief getName
     * @return
     */
    Q_INVOKABLE QString getName();
    /**
     * @brief Unit::getUnitType
     * @return
     */
    Q_INVOKABLE qint32 getUnitType();
    /**
     * @brief getTerrainDefenseModifier the bonus defense for this unit
     * @return
     */
    Q_INVOKABLE qint32 getTerrainDefenseModifier(QPoint position);
    /**
     * @brief getFirstStrike
     * @param position
     * @return
     */
    Q_INVOKABLE bool getFirstStrike(QPoint position, Unit* pAttacker, bool isDefender, QPoint attackerPosition = QPoint(-1, -1));
    /**
     * @brief getMovementpoints the movement points this unit can move
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getMovementpoints(QPoint position);
    /**
     * @brief getBonusMovementpoints
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getBonusMovementpoints(QPoint position);
    /**
     * @brief getBaseMovementPoints the base movement points of this unit
     * @return
     */
    Q_INVOKABLE qint32 getBaseMovementPoints() const;
    /**
     * @brief setBaseMovementPoints the base movement points of this unit
     * @param value
     */
    Q_INVOKABLE void setBaseMovementPoints(const qint32 value);
    /**
     * @brief getBaseMovementCosts
     * @param x
     * @param y
     */
    Q_INVOKABLE qint32 getBaseMovementCosts(qint32 x, qint32 y, qint32 curX = -1, qint32 curY = -1, bool trapChecking = false);
    /**
     * @brief getMovementCosts
     * @param x
     * @param y
     */
    Q_INVOKABLE qint32 getMovementCosts(qint32 x, qint32 y, qint32 curX = -1, qint32 curY = -1, bool trapChecking = false);
    Q_INVOKABLE qreal getHp() const;
    Q_INVOKABLE void setHp(const qreal value);
    Q_INVOKABLE qint32 getHpRounded() const;
    Q_INVOKABLE bool hasDirectWeapon();
    Q_INVOKABLE qint32 getAmmo1() const;
    Q_INVOKABLE void setAmmo1(const qint32 value);
    Q_INVOKABLE bool hasAmmo1() const;
    Q_INVOKABLE void reduceAmmo1(qint32 value);
    Q_INVOKABLE GameEnums::WeaponType getTypeOfWeapon1();
    Q_INVOKABLE qint32 getMaxAmmo1() const;
    Q_INVOKABLE void setMaxAmmo1(const qint32 value);
    Q_INVOKABLE QString getWeapon1ID() const;
    Q_INVOKABLE void setWeapon1ID(const QString value);
    Q_INVOKABLE qint32 getAmmo2() const;
    Q_INVOKABLE void setAmmo2(const qint32 value);
    Q_INVOKABLE bool hasAmmo2() const;
    Q_INVOKABLE void reduceAmmo2(qint32 value);
    Q_INVOKABLE GameEnums::WeaponType getTypeOfWeapon2();
    Q_INVOKABLE qint32 getMaxAmmo2() const;
    Q_INVOKABLE void setMaxAmmo2(const qint32 value);
    Q_INVOKABLE QString getWeapon2ID() const;
    Q_INVOKABLE void setWeapon2ID(const QString value);
    Q_INVOKABLE qint32 getFuel() const;
    Q_INVOKABLE void setFuel(const qint32 value);
    Q_INVOKABLE qint32 getMaxFuel() const;
    Q_INVOKABLE void setMaxFuel(const qint32 value);
    Q_INVOKABLE qint32 getCapturePoints() const;
    Q_INVOKABLE void setCapturePoints(const qint32 value);
    Q_INVOKABLE bool canCapture();
    Q_INVOKABLE qint32 getCosts() const;
    Q_INVOKABLE qint32 getMinRange(QPoint position);
    Q_INVOKABLE qint32 getBonusMinRange(QPoint position);
    Q_INVOKABLE qint32 getBaseMinRange() const;
    Q_INVOKABLE void setMinRange(const qint32 value);
    Q_INVOKABLE qint32 getMaxRange(QPoint position);
    Q_INVOKABLE qint32 getBaseMaxRange();
    Q_INVOKABLE qint32 getMaxRangeAtPosition(QPoint position);
    Q_INVOKABLE qint32 getBonusMaxRange(QPoint position);
    Q_INVOKABLE void setMaxRange(const qint32 value);
    /**
     * @brief isStealthed
     * @return combination of cloak and hidden
     */
    Q_INVOKABLE bool isStatusStealthed() const;
    Q_INVOKABLE bool getHidden() const;
    Q_INVOKABLE void setHidden(bool Hidden);
    /**
     * @brief getCloaked cloaked units are like stealthed units, but cloak will always be removed at the start of a players turn
     * @return
     */
    Q_INVOKABLE bool getCloaked() const;
    /**
     * @brief setCloaked cloaked units are like stealthed units, but cloak will always be removed at the start of a players turn
     * @param cloaked
     */
    Q_INVOKABLE void setCloaked(qint32 cloaked, qint32 byPlayer = -1);
    /**
     * @brief removeCloaked
     * @param byPlayer
     */
    Q_INVOKABLE void removeCloaked(qint32 byPlayer = -1);
    /**
     * @brief updateStealthIcon
     */
    Q_INVOKABLE void updateStealthIcon();
    /**
     * @brief isStealthed checks if the unit is invisible for the given player. Either by terrain, Out of Vision or Stealthmode
     * @param pPlayer
     * @return
     */
    Q_INVOKABLE bool isStealthed(Player* pPlayer, bool ignoreOutOfVisionRange = false, qint32 testX = -1, qint32 testY = -1);
    /**
     * @brief hasTerrainHide
     * @param pPlayer
     * @return
     */
    Q_INVOKABLE bool hasTerrainHide(Player* pPlayer);
    /**
     * @brief getBaseDamage
     * @param pEnemyUnit
     * @return
     */
    Q_INVOKABLE qreal getBaseDamage(Unit* pEnemyUnit);
    Q_INVOKABLE qint32 getUnitRank() const;
    Q_INVOKABLE void setUnitRank(const qint32 UnitRank, bool force = false);
    Q_INVOKABLE qint32 getVision(QPoint position);
    Q_INVOKABLE void setVision(const qint32 value);
    Q_INVOKABLE qint32 getBaseVision();
    /**
     * @brief loadSprites loads the given sprite
     * @param spriteID
     */
    Q_INVOKABLE void loadSprite(const QString spriteID, bool addPlayerColor, bool flipSprite = true, qint32 frameTime = 150);
    /**
     * @brief loadSpriteV2
     * @param spriteID
     * @param mode
     */
    Q_INVOKABLE void loadSpriteV2(const QString spriteID, GameEnums::Recoloring mode, bool flipSprite = true, qint32 frameTime = 150);
    /**
     * @brief addShineTween
     */
    Q_INVOKABLE void addShineTween();
    /**
     * @brief removeShineTween
     */
    Q_INVOKABLE void removeShineTween();
    /**
     * @brief setOwner changes the owner ship of this unit
     * @param pOwner
     */
    Q_INVOKABLE void setOwner(Player* pOwner);
    /**
     * @brief getOwner
     * @return the pointer to the owner of this unit
     */
    Q_INVOKABLE Player* getOwner();
    /**
     * @brief getMovementType the movement type id
     * @return
     */
    Q_INVOKABLE QString getMovementType() const;
    /**
     * @brief setMovementType
     * @param Movementtype
     */
    Q_INVOKABLE void setMovementType(const QString movementType);;
    /**
     * @brief getLoadingPlace
     * @return
     */
    Q_INVOKABLE qint32 getLoadingPlace();
    /**
     * @brief getUnitID the unit id
     * @return
     */
    Q_INVOKABLE QString getUnitID();
    /**
     * @brief getUnitDamageID
     * @return
     */
    Q_INVOKABLE QString getUnitDamageID();
    /**
     * @brief getUnitDamage
     * @param weaponID
     * @return
     */
    Q_INVOKABLE qreal getUnitDamage(const QString weaponID);
    /**
     * @brief getMapPosition
     * @return
     */
    Q_INVOKABLE inline QPoint getMapPosition() const
    {
        return  QPoint(Unit::getX(), Unit::getY());
    }
    /**
     * @brief getPosition the unit position as qpoint
     * @return
     */
    Q_INVOKABLE QPoint getPosition() const
    {
        return QPoint(Unit::getX(), Unit::getY());
    }
    /**
     * @brief refill fills up all ammo and fuel to max
     * @param noMaterial if true material will not be refilled.
     */
    Q_INVOKABLE void refill(bool noMaterial = false, qreal fuelAmount = 1, qreal ammo1Amount = 1, qreal ammo2Amount = 1);

    /**
     * @brief setHasMoved  changes if the unit has been moved or not
     * @param value
     */
    Q_INVOKABLE void setHasMoved(bool value);
    /**
     * @brief getHasMoveed return if this unit has moved or not
     * @return
     */
    Q_INVOKABLE bool getHasMoved() const;
    /**
     * @brief getMovementFuelCostModifier fuel modification when moving this unit from a to b
     * @param fuelCost
     * @return
     */
    Q_INVOKABLE qint32 getMovementFuelCostModifier(qint32 fuelCost);
    /**
     * @brief moveUnit moves the unit to the target position of this unit
     * @param pAction
     */
    Q_INVOKABLE void moveUnitAction(GameAction* pAction);
    /**
     * @brief moveUnit moves the unit to the target path
     * @param movePath
     */
    Q_INVOKABLE void moveUnit(QVector<QPoint> movePath);
    /**
     * @brief createMoveVisionAction
     * @param pAction
     */
    Q_INVOKABLE void createMoveVisionAction(GameAction* pAction);
    /**
     * @brief moveUnit moves the unit to the target path
     * @param movePath
     */
    Q_INVOKABLE void createMoveVision(QVector<QPoint> movePath);
    /**
     * @brief createMoveVisionFromAction
     * @param pAction
     */
    Q_INVOKABLE void createMoveVisionFromAction(GameAction* pAction);
    /**
     * @brief createMoveVision
     * @param x
     * @param y
     */
    Q_INVOKABLE void moveUnitToField(qint32 x, qint32 y);
    /**
     * @brief showCORange
     */
    Q_INVOKABLE void showRanges();
    /**
     * @brief getVisionFields
     * @param position
     * @return x = x, y = y z = direct vision
     */
    Q_INVOKABLE QVector<QVector3D> getVisionFields(QPoint position);
    /**
     * @brief removeUnit removes this from game
     */
    Q_INVOKABLE void removeUnit(bool killed = true);
    /**
     * @brief killUnit removes the unit from the game and let it explode
     */
    Q_INVOKABLE GameAnimation* killUnit();
    /**
     * @brief increaseCapturePoints increases the capture points of this unit based on units hp and ko owner
     */
    Q_INVOKABLE void increaseCapturePoints(QPoint position);
    /**
     * @brief getCaptureRate
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getCaptureRate(QPoint position);
    /**
     * @brief loadIcon
     * @param iconID icon we want to load
     * @param x position of the icon
     * @param y position of the icon
     * @param duration number of days the icon will be shown on the unit
     * @param player the player on which the duration gets decreased
     */
    Q_INVOKABLE void loadIcon(const QString iconID, qint32 x, qint32 y, qint32 duration = -1, qint32 player = -1);
    /**
     * @brief unloadIcon removes the given icon from this unit
     * @param iconID
     */
    Q_INVOKABLE void unloadIcon(const QString iconID);
    /**
     * @brief unloadIconAndDuration
     * @param iconID
     */
    Q_INVOKABLE void unloadIconAndDuration(const QString iconID);
    /**
     * @brief endOfTurn
     */
    Q_INVOKABLE void endOfTurn();
    /**
     * @brief startOfTurn
     */
    Q_INVOKABLE void startOfTurn();
    /**
     * @brief updateIconDuration
     * @param player
     */
    Q_INVOKABLE void updateStatusDurations(qint32 player);
    /**
     * @brief updateUnitStatus updates unit buffs
     */
    Q_INVOKABLE void updateUnitStatus();
    /**
     * @brief getTerrain
     * @return
     */
    Q_INVOKABLE Terrain* getTerrain();
    /**
     * @brief canMoveAndFire
     * @return if this unit can move and fire
     */
    Q_INVOKABLE bool canMoveAndFire(QPoint position);
    /**
     * @brief loadUnit adds a unit to the transport list
     * @param pUnit
     */
    Q_INVOKABLE void loadUnit(Unit* pUnit, qint32 index = -1);
    /**
     * @brief loadUnit adds a unit to the transport list
     * @param pUnit
     */
    Q_INVOKABLE void loadSpawnedUnit(const QString unitId);
    /**
     * @brief getLoadedUnit
     * @param index
     */
    Q_INVOKABLE Unit* getLoadedUnit(qint32 index) const;
    /**
     * @brief unloadUnit removes a unit from the transport list
     * @param index
     */
    Q_INVOKABLE void unloadUnit(Unit* pUnit, QPoint position);
    /**
     * @brief unloadUnit
     * @param index
     * @param position
     */
    Q_INVOKABLE void unloadUnitAtIndex(qint32 index, QPoint position);
    /**
     * @brief getLoadedUnitCount
     * @return number of units loaded by this unit
     */
    Q_INVOKABLE qint32 getLoadedUnitCount() const;
    /**
     * @brief getTransportUnits
     * @return
     */
    Q_INVOKABLE QStringList getTransportUnits();
    /**
     * @brief canTransportUnit
     * @return checks if we have space and if the unit can be transported by this unit.
     */
    Q_INVOKABLE bool canTransportUnit(Unit* pUnit, bool ignoreLoadingPlace = false);
    /**
     * @brief isTransporter
     * @return
     */
    Q_INVOKABLE bool isTransporter();
    /**
     * @brief postAction
     * @param pAction
     */
    Q_INVOKABLE void postAction(GameAction * pAction);
    /**
     * @brief getBonusOffensive
     * @param position
     * @return offensive bonus at this position
     */
    Q_INVOKABLE qint32 getBonusOffensive(GameAction* pAction, QPoint position, Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief getUnitBonusOffensive
     * @param position
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    Q_INVOKABLE qint32 getUnitBonusOffensive(GameAction* pAction, QPoint position, Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief getDamageReduction
     * @param pAttacker
     * @param position
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    Q_INVOKABLE qreal getDamageReduction(GameAction* pAction, qreal damage, Unit* pAttacker, QPoint position, qint32 attackerBaseHp,
                                         QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief getTrueDamage
     * @param damage
     * @param position
     * @param attackerBaseHp
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    Q_INVOKABLE qreal getTrueDamage(GameAction* pAction, qreal damage, QPoint position, qint32 attackerBaseHp,
                                    Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief canCounterAttack
     * @param pAction
     * @param position
     * @param pDefender
     * @param defPosition
     * @param luckMode
     * @return
     */
    Q_INVOKABLE bool canCounterAttack(GameAction* pAction, QPoint position, Unit* pDefender, QPoint defPosition, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief getTerrainDefense
     * @return
     */
    Q_INVOKABLE qint32 getTerrainDefense(qint32 x = -1, qint32 y = -1);
    /**
     * @brief getBonusDeffensive
     * @param position
     * @return defense bonus at this position
     */
    Q_INVOKABLE qint32 getBonusDefensive(GameAction* pAction, QPoint position, Unit* pAttacker, QPoint atkPosition, bool isAttacker, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief getUnitBonusDefensive
     * @param position
     * @param pAttacker
     * @param atkPosition
     * @param isDefender
     * @return
     */
    Q_INVOKABLE qint32 getUnitBonusDefensive(GameAction* pAction, QPoint position, Unit* pAttacker, QPoint atkPosition, bool isAttacker, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief useTerrainDefense
     * @return
     */
    Q_INVOKABLE bool useTerrainDefense();
    /**
     * @brief useVisionHide
     * @return
     */
    Q_INVOKABLE bool useTerrainHide();
    /**
     * @brief getAttackHpBonus
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getAttackHpBonus(QPoint position);
    /**
     * @brief getMisfortune
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getBonusMisfortune(QPoint position);
    /**
     * @brief getLuck
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getBonusLuck(QPoint position);
    /**
     * @brief getUnitCosts
     * @return
     */
    Q_INVOKABLE qint32 getUnitCosts() const;
    /**
     * @brief getRepairBonus
     * @return
     */
    Q_INVOKABLE qint32 getRepairBonus(QPoint position);
    /**
     * @brief getRepairCostModifier
     * @return
     */
    Q_INVOKABLE qreal getRepairCostModifier();
    /**
     * @brief setUnitVisible
     * @param value
     */
    Q_INVOKABLE void setUnitVisible(bool value, Player* pPlayer);
    /**
     * @brief makeCOUnit
     * @param co
     */
    Q_INVOKABLE void makeCOUnit(quint8 co, bool force = false);
    /**
     * @brief createCORange
     */
    Q_INVOKABLE void createCORange(qint32 coRange);
    /**
     * @brief showCustomRange
     * @param id
     * @param range
     * @param color
     */
    Q_INVOKABLE void showCustomRange(const QString id, qint32 range, QColor color = Qt::white);
    /**
     * @brief removeCustomRange
     * @param id
     */
    Q_INVOKABLE void removeCustomRange(const QString id);
    /**
     * @brief updateSprites reloads all sprites
     */
    Q_INVOKABLE void updateSprites(bool editor);
    /**
     * @brief resetSprites removes all sprites from the unit which makes it invisible
     */
    Q_INVOKABLE void resetSprites();
    /**
     * @brief getFuelCostModifier
     * @param pUnit
     * @param costs
     * @return
     */
    Q_INVOKABLE qint32 getFuelCostModifier(QPoint position, qint32 costs);
    /**
     * @brief getVariables
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief getTerrainAnimationBase
     * @return
     */
    Q_INVOKABLE QString getTerrainAnimationBase(Unit* pDefender = nullptr);
    /**
     * @brief getTerrainAnimationForeground
     * @return
     */
    Q_INVOKABLE QString getTerrainAnimationForeground(Unit* pDefender = nullptr);
    /**
     * @brief getTerrainAnimationBackground
     * @return
     */
    Q_INVOKABLE QString getTerrainAnimationBackground(Unit* pDefender = nullptr);
    /**
     * @brief getTerrainAnimationMoveSpeed
     * @return
     */
    Q_INVOKABLE qreal getTerrainAnimationMoveSpeed();
    /**
     * @brief isAttackableFromPosition
     * @param pDefender
     * @param unitPos
     * @return
     */
    Q_INVOKABLE bool isAttackableFromPosition(Unit* pDefender, QPoint unitPos);
    /**
     * @brief isAttackable
     * @param pDefender
     * @return
     */
    Q_INVOKABLE bool isAttackable(Unit* pDefender, bool ignoreOutOfVisionRange = false, QPoint unitPos = QPoint(-1, -1), bool isDefenderPos = false);
    /**
     * @brief canAttackWithWeapon
     * @param weaponIndex
     * @param unitX
     * @param unitY
     * @param targetX
     * @param targetY
     * @return
     */
    Q_INVOKABLE bool canAttackWithWeapon(qint32 weaponIndex, qint32 unitX, qint32 unitY, qint32 targetX, qint32 targetY, GameEnums::AttackRangeCheck rangeCheck = GameEnums::AttackRangeCheck_All);
    /**
     * @brief canAttackStealthedUnit
     * @param pDefender
     * @return
     */
    Q_INVOKABLE bool canAttackStealthedUnit(Unit* pDefender);
    /**
     * @brief isEnvironmentAttackable
     * @param terrainID
     * @return
     */
    Q_INVOKABLE bool isEnvironmentAttackable(QString terrainID);
    /**
     * @brief Unit::getEnvironmentDamage
     * @param terrainID
     * @return
     */
    Q_INVOKABLE qreal getEnvironmentDamage(QString terrainID);
    /**
     * @brief canMoveOver
     * @param x
     * @param y
     * @return
     */
    Q_INVOKABLE bool canMoveOver(qint32 x, qint32 y);
    /**
     * @brief getUnitValue the value of the unit -> which is unit cost * hp / 10
     * @return
     */
    Q_INVOKABLE qint32 getUnitValue();
    /**
     * @brief getCoUnitValue
     * @return
     */
    Q_INVOKABLE qint32 getCoUnitValue();
    /**
     * @brief canBeRepaired
     * @return
     */
    Q_INVOKABLE bool canBeRepaired(QPoint position);
    /**
     * @brief addOffensiveBonus
     * @param value
     * @param duration
     */
    Q_INVOKABLE void addOffensiveBonus(qint32 value, qint32 duration = 1);
    /**
     * @brief addDefensiveBonus
     * @param value
     * @param duration
     */
    Q_INVOKABLE void addDefensiveBonus(qint32 value, qint32 duration = 1);
    /**
     * @brief addVisionBonus
     * @param value
     * @param duration
     */
    Q_INVOKABLE void addVisionBonus(qint32 value, qint32 duration = 1);
    /**
     * @brief addMovementBonus
     * @param value
     * @param duration
     */
    Q_INVOKABLE void addMovementBonus(qint32 value, qint32 duration = 1);
    /**
     * @brief addFirerangeBonus
     * @param value
     * @param duration
     */
    Q_INVOKABLE void addFirerangeBonus(qint32 value, qint32 duration = 1);
    /**
     * @brief modifyUnit
     * @param hpChange
     * @param ammo1Change
     * @param ammo2Change
     * @param fuelChange
     */
    Q_INVOKABLE void modifyUnit(qint32 hpChange, qint32 ammo1Change, qint32 ammo2Change, qint32 fuelChange);
    /**
     * @brief getMaxUnitRang
     * @return
     */
    Q_INVOKABLE qint32 getMaxUnitRang();
    /**
     * @brief getUnitRangIcon
     * @return
     */
    Q_INVOKABLE QString getUnitRangIcon();
    /**
     * @brief getUnitRangName
     * @param rang
     * @return
     */
    Q_INVOKABLE static QString getUnitRangName(qint32 rang);
protected:
    /**
     * @brief updateIconTweens creates the visibility toogle tweens for all icons
     */
    void updateIconTweens();
    /**
     * @brief updateBonus
     * @param data
     */
    void updateBonus(QVector<QPoint>& data);
    /**
     * @brief getBonus
     * @param data
     * @return
     */
    qint32 getBonus(QVector<QPoint>& data);
    /**
     * @brief updateRangeActor
     * @param pActor
     * @param range
     * @param resAnim
     * @param color
     */
    void updateRangeActor(oxygine::spActor & pActor, qint32 range, QString resAnim, QColor color);
    /**
     * @brief updateCustomRangeActors
     */
    void updateCustomRangeActors();
    /**
     * @brief Unit::getEnemyBonus
     * @param position
     * @return
     */
    qint32 getCoBonus(QPoint position, const QString & function, qint32(Player::*pBonusFunction)(QPoint, Unit*, const QString));
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
    struct CustomRangeInfo
    {
        oxygine::spActor pActor;
        QString id;
        qint32 range;
        QColor color;
    };
    /**
     * @brief m_customRangeInfo
     */
    QVector<CustomRangeInfo> m_customRangeInfo;
    /**
     * @brief m_UnitID the id of this unit
     */
    QString m_UnitID;
    /**
     * @brief m_customName
     */
    QString m_customName;
    /**
     * @brief m_Owner our owner a nullptr means we're a neutral building
     */
    Player* m_pOwner{nullptr};
    /**
     * @brief m_Terrain the terrain at which we are placed
     */
    Terrain* m_pTerrain{nullptr};
    // basic data of this unit
    qreal m_hp{Unit::MAX_UNIT_HP};
    qint32 m_ammo1{-1};
    qint32 m_maxAmmo1{-1};
    QString m_weapon1ID;
    qint32 m_ammo2{-1};
    qint32 m_maxAmmo2{-1};
    QString m_weapon2ID;
    qint32 m_fuel{-1};
    qint32 m_maxFuel{-1};
    qint32 m_baseMovementPoints{0};
    bool m_Moved{false};
    QVector<spUnit> m_TransportUnits;
    qint32 m_capturePoints{0};
    qint32 m_UnitRank{GameEnums::UnitRank_None};
    qreal m_virtualHp{0.0f};

    QVector<QPoint> m_cloaked;
    bool m_Hidden{false};

    bool m_IgnoreUnitCollision{false};

    qint32 m_minRange{1};
    qint32 m_maxRange{-1};
    qint32 m_cursorInfoRange{-1};
    qint32 m_vision{1};
    qint32 m_UniqueID{0};
    GameEnums::GameAi m_AiMode{GameEnums::GameAi::GameAi_Normal};
    QVector<QPoint> m_AiMovePath;
    ScriptVariables m_Variables;
    ModdingFlags m_ModdingFlags{ModdingFlags::None};

    qint32 m_VisionHigh{-1};

    qint32 m_virtuellX{-1};
    qint32 m_virtuellY{-1};

    QVector<QPoint> m_MultiTurnPath;

    QVector<oxygine::spTween> m_ShineTweens;
    QString m_MovementType;

    QVector<QPoint> m_OffensiveBonus;
    QVector<QPoint> m_DefensiveBonus;
    QVector<QPoint> m_VisionBonus;
    QVector<QPoint> m_MovementBonus;
    QVector<QPoint> m_FirerangeBonus;

    QVector<IconDuration> m_IconDurations;
    GameMap* m_pMap{nullptr};
    /**
     * @brief m_sortValues values sto
     */
    QVector<qint32> m_sortValues;
};

Q_DECLARE_INTERFACE(Unit, "Unit");

#endif // UNIT_H

