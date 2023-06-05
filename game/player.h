#ifndef PLAYER_H
#define PLAYER_H

#include <tuple>
#include <QColor>
#include <QObject>
#include <QVector>
#include <QVector3D>

#include "coreengine/qmlvector.h"
#include "coreengine/fileserializable.h"

#include "game/GameEnums.h"
#include "game/co.h"

#include "gameinput/basegameinputif.h"

class SimpleProductionSystem;
class GameMap;
class Player;
using spPlayer = oxygine::intrusive_ptr<Player>;

class Player : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT
public:
    /**
     * @brief Player
     */
    explicit Player(GameMap* pMap);
    virtual ~Player() = default;
    /**
     * @brief init
     */
    void init();
    /**
     * @brief releaseStaticData
     */
    static void releaseStaticData();
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief serializeObject
     * @param pStream
     * @param forHash
     */
    void serializeObject(QDataStream& pStream, bool forHash) const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief deserializeObject
     * @param pStream
     * @param fast
     */
    void deserializer(QDataStream& pStream, bool fast);
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 19;
    }
    /**
     * @brief setBaseGameInput sets the player input
     * @param pBaseGameInput
     */
    void setBaseGameInput(spBaseGameInputIF pBaseGameInput);
    /**
     * @brief getCO
     * @param id index of the co 0 or 1
     * @return pointer to the co
     */
    spCO getspCO(quint8 id);
    /**
     * @brief getSpEnemyUnits
     * @return
     */
    QVector<spUnit> getSpEnemyUnits();

    /**
     * @brief getBuildlistChanged
     * @return
     */
    bool getBuildlistChanged() const;
    /**
     * @brief setBuildlistChanged
     * @param BuildlistChanged
     */
    void setBuildlistChanged(bool BuildlistChanged);
    /**
     * @brief getSocketId
     * @return
     */
    quint64 getSocketId() const;
    /**
     * @brief setSocketId
     * @param socketId
     */
    void setSocketId(const quint64 &socketId);
    /**
     * @brief getColorTableAnim
     * @return
     */
    virtual oxygine::spResAnim getColorTableAnim() const;
    /**
     * @brief getColorTableAnim
     * @return
     */
    static oxygine::spResAnim getNeutralTableAnim();
#ifdef GRAPHICSUPPORT
    /**
     * @brief getColorTable
     * @return
     */
    const QImage &getColorTable() const;
    /**
     * @brief getNeutralTableImage
     * @return
     */
    static const QImage &getNeutralTableImage();
#endif
    void setMenu(GameMenue *newMenu);

    void setIsOnline(bool newIsOnline);
    /**
     * @brief setUniqueIdentifier
     * @param newUniqueIdentifier
     */
    Q_INVOKABLE void setUniqueIdentifier(const QString newUniqueIdentifier);
    /**
     * @brief setPlayerNameId
     * @param newDisplayName
     */
    Q_INVOKABLE void setPlayerNameId(const QString newDisplayName);
    /**
     * @brief setControlType
     * @param newControlType
     */
    Q_INVOKABLE void setControlType(const GameEnums::AiTypes newControlType);
    /**
     * @brief getOnline
     * @return
     */
    Q_INVOKABLE bool getIsOnline() const;
    /**
     * @brief getCoGroupModifier
     * @param unitIds
     * @return
     */
    Q_INVOKABLE qreal getCoGroupModifier(QStringList unitIds, SimpleProductionSystem* system);
    /**
     * @brief getControlType
     * @return this is the ai type set during game creation this may differ to the instanciated ai type during multiplayer matches
     */
    Q_INVOKABLE GameEnums::AiTypes getControlType() const;
    /**
     * @brief getDisplayName
     * @return
     */
    Q_INVOKABLE QString getPlayerNameId() const;
    /**
     * @brief getUniqueIdentifier
     * @return
     */
    Q_INVOKABLE  QString getUniqueIdentifier() const;
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap *getMap() const;
    /**
     * @brief onUnitDeath
     * @param pUnit
     */
    Q_INVOKABLE void onUnitDeath(Unit* pUnit);
    /**
     * @brief postAction
     * @param pAction
     */
    Q_INVOKABLE void postAction(GameAction* pAction);
    /**
     * @brief setPlayerArmy
     * @param value
     */
    Q_INVOKABLE void setPlayerArmy(const QString value);
    /**
     * @brief getPlayerArmy
     * @return
     */
    Q_INVOKABLE QString getPlayerArmy() const;
    /**
     * @brief getPlayerArmySelected
     * @return
     */
    Q_INVOKABLE bool getPlayerArmySelected() const;
    /**
     * @brief setPlayerArmySelected
     * @param playerArmySelected
     */
    Q_INVOKABLE void setPlayerArmySelected(bool playerArmySelected);
    /**
     * @brief getFlipUnitSprites
     * @return
     */
    Q_INVOKABLE bool getFlipUnitSprites() const;
    /**
     * @brief getUnitBuildValue
     * @param unitID
     * @return
     */
    Q_INVOKABLE qreal getUnitBuildValue(const QString unitID);
    /**
     * @brief getBaseGameInput pointer to the ai or human player interface
     * @return
     */
    Q_INVOKABLE BaseGameInputIF* getBaseGameInput();
    /**
     * @brief getIsDefeated
     * @return
     */
    Q_INVOKABLE bool getIsDefeated() const;
    /**
     * @brief setIsDefeated if true defeats the player but doesn't remove units or buildings
     * @param value
     */
    Q_INVOKABLE void setIsDefeated(bool value);
    /**
     * @brief swapCOs swaps co 0 and 1
     */
    Q_INVOKABLE void swapCOs();
    /**
     * @brief getColor the color of this player
     * @return
     */
    Q_INVOKABLE QColor getColor() const;
    /**
     * @brief setColor sets the color of this player. Note this want update existing sprites
     * @param Color
     */
    Q_INVOKABLE void setColor(QColor Color, qint32 table = -1);
    /**
     * @brief getPlayerID player id of this player from 0 to n
     * @return
     */
    Q_INVOKABLE qint32 getPlayerID() const;
    /**
     * @brief getArmy the army string id of this player.
     * @return
     */
    Q_INVOKABLE QString getArmy();
    /**
     * @brief isEnemy checks the alliance with this player
     * @param pPlayer the player we want to check if he's an enemy
     * @return the alliance of the player with us
     */
    Q_INVOKABLE GameEnums::Alliance checkAlliance(Player* pPlayer);
    /**
     * @brief isEnemyUnit checks if the given unit is an enemy
     * @param pUnit
     * @return
     */
    Q_INVOKABLE bool isEnemyUnit(Unit* pUnit);
    /**
     * @brief isEnemy
     * @param pOwner
     * @return
     */
    Q_INVOKABLE bool isEnemy(Player* pOwner);
    /**
     * @brief isPlayerIdEnemy
     * @param playerId
     */
    Q_INVOKABLE bool isPlayerIdEnemy(qint32 playerId);
    /**
     * @brief Player::isPlayerIdAlly
     * @param playerId
     * @return
     */
    Q_INVOKABLE bool isPlayerIdAlly(qint32 playerId);
    /**
     * @brief isAlly
     * @param pOwner
     * @return
     */
    Q_INVOKABLE bool isAlly(Player* pOwner);
    /**
     * @brief getFunds
     * @return
     */
    Q_INVOKABLE qint32 getFunds() const;
    /**
     * @brief addFunds increases the money of this player by the given value
     * @param value
     */
    Q_INVOKABLE void addFunds(const qint32 value);
    /**
     * @brief setFunds
     * @param value
     */
    Q_INVOKABLE void setFunds(const qint32 value);
    /**
     * @brief earnMoney earns money based on the buildings the player has
     * @param modifier multiplier additionaly modifying the income
     */
    Q_INVOKABLE void earnMoney(qreal modifier = 1.0f);
    /**
     * @brief calcIncome
     * @param modifier
     * @return
     */
    Q_INVOKABLE qint32 calcIncome(qreal modifier = 1.0f) const;
    /**
     * @brief calcArmyValue
     * @return
     */
    Q_INVOKABLE qint32 calcArmyValue();
    /**
     * @brief getBonusIncome
     * @param pBuilding
     * @param income
     * @return
     */
    Q_INVOKABLE qint32 getIncomeReduction(Building* pBuilding, qint32 income);
    /**
     * @brief getCO
     * @param id index of the co 0 or 1
     * @return pointer to the co
     */
    Q_INVOKABLE CO* getCO(quint8 id);
    /**
     * @brief getMaxCoCount
     * @return
     */
    Q_INVOKABLE qint32 getMaxCoCount() const;
    /**
     * @brief setCO
     * @param coId the co we want to load
     * @param id the index at which we want this co
     */
    Q_INVOKABLE void setCO(QString coId, quint8 idx);
    /**
     * @brief getCoCount
     * @return
     */
    Q_INVOKABLE qint32 getCoCount() const;
    /**
     * @brief getCostModifier
     * @param baseCost
     * @return
     */
    Q_INVOKABLE qint32 getCostModifier(const QString id, qint32 baseCost, QPoint position);
    /**
     * @brief getCosts
     * @param id
     * @return
     */
    Q_INVOKABLE qint32 getCosts(const QString id, QPoint position);
    /**
     * @brief gainPowerstar
     * @param fundsDamage
     */
    Q_INVOKABLE void gainPowerstar(qint32 fundsDamage, QPoint position, qint32 hpDamage, bool defender, bool counterAttack);
    /**
     * @brief endOfTurn
     */
    Q_INVOKABLE void endOfTurn();
    /**
     * @brief startOfTurn
     */
    Q_INVOKABLE void startOfTurn();
    /**
     * @brief getUnits
     * @return
     */
    Q_INVOKABLE QmlVectorUnit* getUnits();
    /**
     * @brief getEnemyUnits
     * @return
     */
    Q_INVOKABLE QmlVectorUnit* getEnemyUnits();
    /**
     * @brief getEnemyBuildings
     * @return
     */
    Q_INVOKABLE QmlVectorBuilding* getEnemyBuildings();
    /**
     * @brief getEnemyCount
     * @return
     */
    Q_INVOKABLE qint32 getEnemyCount();
    /**
     * @brief getBuildings
     * @return
     */
    Q_INVOKABLE QmlVectorBuilding* getBuildings(const QString id = "");
    /**
     * @brief getBuildingsListCount
     * @param ids
     * @return
     */
    Q_INVOKABLE QmlVectorBuilding* getBuildingsListCount(const QStringList ids);
    /**
     * @brief updateCORange
     */
    Q_INVOKABLE void updateVisualCORange();
    /**
     * @brief getMovementcostModifier
     * @param pUnit
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getMovementcostModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getEnemyBonusLuck
     * @param pUnit
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getBonusLuck(Unit* pUnit, QPoint position);
    /**
     * @brief getEnemyBonusLuck
     * @param pUnit
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getEnemyBonusLuck(Unit* pUnit, QPoint position);
    /**
     * @brief getEnemyBonusLuck
     * @param pUnit
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getBonusMisfortune(Unit* pUnit, QPoint position);
    /**
     * @brief getEnemyBonusLuck
     * @param pUnit
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getEnemyBonusMisfortune(Unit* pUnit, QPoint position);
    /**
     * @brief getWeatherMovementCostModifier
     * @param pUnit
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getWeatherMovementCostModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getBonusMovementpoints
     * @param pUnit
     * @param position
     * @return
     */
    Q_INVOKABLE qint32 getBonusMovementpoints(Unit* pUnit, QPoint position);
    /**
     * @brief getRockettarget finds the most valuable target for damaging units in a diamond.
     * If multiple targets with the same value are found.
     * One is selected at random
     * @param radius
     * @param damage
     * @param ownUnitValue value of own or allied units compared to enemy ones.
     * @return -1, -1 for no target found
     */
    Q_INVOKABLE QPoint getRockettarget(qint32 radius, qint32 damage, qreal ownUnitValue = 1.2, GameEnums::RocketTarget targetType = GameEnums::RocketTarget_Money, QmlVectorPoint* pSearchArea = nullptr);
    /**
     * @brief getSiloRockettarget
     * @param radius
     * @param damage
     * @param highestDamage
     * @param ownUnitValue
     * @param targetType
     * @return
     */
    Q_INVOKABLE QPoint getSiloRockettarget(qint32 radius, qint32 damage, qint32 highestDamage, qreal ownUnitValue = 1.2, GameEnums::RocketTarget targetType = GameEnums::RocketTarget_Money, QmlVectorPoint* pSearchArea = nullptr);
    /**
     * @brief getRocketTargetDamage
     * @param x
     * @param y
     * @param pPoints
     * @param damage
     * @param ownUnitValue
     * @param targetType
     * @return
     */
    Q_INVOKABLE qint32 getRocketTargetDamage(qint32 x, qint32 y, QmlVectorPoint* pPoints, qint32 damage, qreal ownUnitValue, GameEnums::RocketTarget targetType, bool ignoreStealthed);
    /**
     * @brief defineArmy defines our army sprites based on the current co at position 0
     */
    Q_INVOKABLE void defineArmy();
    /**
     * @brief getFundsModifier
     * @return
     */
    Q_INVOKABLE qreal getFundsModifier() const;
    /**
     * @brief getPowerChargeBonus
     * @return
     */
    Q_INVOKABLE qint32 getPowerChargeBonus() const;
    /**
     * @brief setFundsModifier
     * @param value
     */
    Q_INVOKABLE void setFundsModifier(qreal value);
    /**
     * @brief calculatePlayerStrength
     * @return
     */
    Q_INVOKABLE qint32 calculatePlayerStrength() const;
    /**
     * @brief getBuildingCount
     * @return
     */
    Q_INVOKABLE qint32 getBuildingCount(const QString buildingID = "");
    /**
     * @brief getBuildingListCount
     * @param list
     * @param whitelist
     * @return
     */
    Q_INVOKABLE qint32 getBuildingListCount(const QStringList list, bool whitelist = true);
    /**
     * @brief getUnitCount
     * @return
     */
    Q_INVOKABLE qint32 getUnitCount(const QString unitID = "") const;
    /**
     * @brief getUnitCount
     * @param pUnit
     * @param unitID
     * @return
     */
    Q_INVOKABLE qint32 getUnitCount(Unit* pUnit, const QString unitID) const;
    /**
     * @brief getEnemyBonus
     * @param position
     * @param pUnit
     * @return
     */
    Q_INVOKABLE qint32 getCoBonus(QPoint position, Unit* pUnit, const QString function);
    /**
     * @brief getTeam
     * @return
     */
    Q_INVOKABLE qint32 getTeam() const;
    /**
     * @brief setTeam
     * @param value
     */
    Q_INVOKABLE void setTeam(const qint32 value);
    /**
     * @brief defeatPlayer defeats the player and removes him from further playing
     * @param pPLayer none nullptr will give all buildings to the selected player
     * @param units if units is true the player will also get the controll of the given player
     */
    Q_INVOKABLE void defeatPlayer(Player* pPlayer, bool units = false);
    /**
     * @brief postBattleActions
     * @param pAttacker
     * @param pDefender
     */
    Q_INVOKABLE void postBattleActions(Unit* pAttacker, qreal atkDamage, Unit* pDefender, bool gotAttacked, qint32 weapon, GameAction* pAction);
    /**
     * @brief updatePlayerVision updates the current vision of this player.
     * @brief reduces the timer for vision created for several turns
     */
    Q_INVOKABLE void updatePlayerVision(bool reduceTimer = false);
    /**
     * @brief addVisionField
     * @param x
     * @param y
     * @param duration
     */
    Q_INVOKABLE void addVisionField(qint32 x, qint32 y, qint32 duration = 1, bool directView = false);
    /**
     * @brief getFieldVisible
     * @param x
     * @param y
     */
    Q_INVOKABLE virtual bool getFieldVisible(qint32 x, qint32 y);
    /**
     * @brief getFieldVisibleLoaded
     * @return
     */
    Q_INVOKABLE inline bool getFieldVisibleLoaded()
    {
        return m_FogVisionFields.size() > 0;
    }
    /**
     * @brief getFieldVisibleType
     * @param x
     * @param y
     * @return
     */
    Q_INVOKABLE virtual GameEnums::VisionType getFieldVisibleType(qint32 x, qint32 y) const;
    /**
     * @brief getFieldDirectVisible
     * @param x
     * @param y
     * @return if true a stealthed unit will be made visible by this players view
     */
    Q_INVOKABLE virtual bool getFieldDirectVisible(qint32 x, qint32 y);
    /**
     * @brief loadVisionFields
     */
    Q_INVOKABLE void loadVisionFields();
    /**
     * @brief loadCOMusic
     */
    Q_INVOKABLE void loadCOMusic();
    /**
     * @brief buildedUnit called after a unit was created do whatever you want with this information
     * @param pUnit
     */
    Q_INVOKABLE void buildedUnit(Unit* pUnit);
    /**
     * @brief getWeatherImmune
     * @return
     */
    Q_INVOKABLE bool getWeatherImmune();
    /**
     * @brief getBuildList
     * @return
     */
    Q_INVOKABLE QStringList getBuildList() const;
    /**
     * @brief getCOUnits
     * @return
     */
    Q_INVOKABLE QStringList getCOUnits(Building* pBuilding);
    /**
     * @brief getTransportUnits
     * @param pUnit
     * @return
     */
    Q_INVOKABLE QStringList getTransportUnits(Unit* pUnit);
    /**
     * @brief setBuildList
     * @param BuildList
     */
    Q_INVOKABLE void setBuildList(const QStringList BuildList);
    /**
     * @brief changeBuildlist
     * @param unitID
     * @param remove
     */
    Q_INVOKABLE void changeBuildlist(const QString unitID, bool remove = false);
    /**
     * @brief getVariables
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief getEnemyRepairCostModifier
     * @param pUnit
     * @return
     */
    Q_INVOKABLE qreal getEnemyRepairCostModifier(Unit* pUnit);
    /**
     * @brief getBonusRepairCostModifier
     * @param pUnit
     * @return
     */
    Q_INVOKABLE qreal getRepairCostModifier(Unit* pUnit);
private:
    /**
     * @brief loadTable
     * @param table
     * @return
     */
    bool loadTable(qint32 table);
    /**
     * @brief loadTableFromFile
     * @param tablename
     * @return
     */
    bool loadTableFromFile(const QString tablename);
    /**
     * @brief colorToTable
     * @param baseColor
     * @return
     */
    bool colorToTable(QColor baseColor);
    /**
     * @brief colorToTableInTable
     * @param baseColor
     * @return
     */
    bool colorToTableInTable(QColor baseColor);
    /**
     * @brief createTable
     * @param baseColor
     */
    void createTable(QColor baseColor);
    /**
     * @brief getAverageCost
     * @return
     */
    qint32 getAverageCost();
    /**
     * @brief addVisionFieldInternal
     * @param x
     * @param y
     * @param duration
     * @param directView
     */
    void addVisionFieldInternal(qint32 x, qint32 y, qint32 duration, bool directView);
    /**
     * @brief calculatePlayerStrength
     * @param pUnit
     */
    qint32 calculatePlayerStrength(Unit* pUnit) const;
protected:
    GameMap* m_pMap{nullptr};

private:
    qint32 m_funds{0};
    qreal m_fundsModifier{1.0};

    QColor m_Color;
#ifdef GRAPHICSUPPORT
    QImage m_colorTable;
    oxygine::spResAnim m_ColorTableAnim;
#endif
    QString m_playerArmy{""};
    qint32 m_team{0};
    std::array<spCO, 2> m_playerCOs;
    /**
     * @brief m_pBaseGameInput pointer to the ai or human player
     */
    spBaseGameInputIF m_pBaseGameInput{nullptr};
    GameEnums::AiTypes m_controlType{GameEnums::AiTypes::AiTypes_Open};
    GameMenue* m_pMenu{nullptr};
    bool m_isDefeated{false};
    struct VisionFieldInfo
    {
        VisionFieldInfo()
        {
        }
        VisionFieldInfo(GameEnums::VisionType visionType, qint32 duration, bool directView)
            : m_visionType(visionType),
              m_duration(duration),
              m_directView(directView)
        {
        }
        GameEnums::VisionType m_visionType = GameEnums::VisionType_Shrouded;
        qint32 m_duration = 0;
        bool m_directView = false;
    };

    std::vector<std::vector<VisionFieldInfo>> m_FogVisionFields;
    /**
     * @brief m_BuildList contains all units we're allowed to build
     */
    QStringList m_BuildList;
    bool m_BuildlistChanged{false};
    ScriptVariables m_Variables;
    quint64 m_socketId{0};
    bool m_playerArmySelected{false};
    qint32 m_averageCosts{-1};
    /**
     * @brief m_uniqueIdentifier
     */
    QString m_uniqueIdentifier;
    QString m_playerNameId;
    bool m_isOnline{true};
    static oxygine::spResAnim m_neutralTableAnim;
    static QImage m_neutralTableImage;
};

Q_DECLARE_INTERFACE(Player, "Player");

#endif // PLAYER_H
