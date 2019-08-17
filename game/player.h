#ifndef PLAYER_H
#define PLAYER_H

#include <tuple>
#include <QColor>
#include <QObject>
#include <QVector>
#include <QVector3D>
#include "oxygine-flow.h"
#include "game/smartpointers.h"
#include "game/smartCO.h"

#include "coreengine/mainapp.h"

#include "coreengine/fileserializable.h"

#include "game/GameEnums.h"

#include "gameinput/basegameinputif.h"

class CO;

class Player : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT

public:

    explicit Player();
    void init();
    virtual ~Player() override;
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
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 9;
    }
    /**
     * @brief setBaseGameInput sets the player input
     * @param pBaseGameInput
     */
    void setBaseGameInput(BaseGameInputIF *pBaseGameInput);
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
signals:

public slots:
    /**
     * @brief getBaseGameInput pointer to the ai or human player interface
     * @return
     */
    inline BaseGameInputIF* getBaseGameInput()
    {
        return m_pBaseGameInput.get();
    }
    /**
     * @brief getIsDefeated
     * @return
     */
    bool getIsDefeated() const;
    /**
     * @brief setIsDefeated if true defeats the player but doesn't remove units or buildings
     * @param value
     */
    void setIsDefeated(bool value);
    /**
     * @brief swapCOs swaps co 0 and 1
     */
    void swapCOs();
    /**
     * @brief getColor the color of this player
     * @return
     */
    QColor getColor() const;
    /**
     * @brief setColor sets the color of this player. Note this want update existing sprites
     * @param Color
     */
    void setColor(QColor Color);
    /**
     * @brief getPlayerID player id of this player from 0 to n
     * @return
     */
    qint32 getPlayerID() const;
    /**
     * @brief getArmy the army string id of this player.
     * @return
     */
    QString getArmy();
    /**
     * @brief isEnemy checks the alliance with this player
     * @param pPlayer the player we want to check if he's an enemy
     * @return the alliance of the player with us
     */
    GameEnums::Alliance checkAlliance(Player* pPlayer);
    /**
     * @brief isEnemyUnit checks if the given unit is an enemy
     * @param pUnit
     * @return
     */
    bool isEnemyUnit(Unit* pUnit);
    /**
     * @brief isEnemy
     * @param pOwner
     * @return
     */
    bool isEnemy(Player* pOwner);
    qint32 getFunds() const;
    /**
     * @brief addFunds increases the money of this player by the given value
     * @param value
     */
    void addFunds(const qint32 &value);
    /**
     * @brief setFunds
     * @param value
     */
    void setFunds(const qint32 &value);
    /**
     * @brief earnMoney earns money based on the buildings the player has
     * @param modifier multiplier additionaly modifying the income
     */
    void earnMoney(float modifier = 1.0f);
    /**
     * @brief calcIncome
     * @param modifier
     * @return
     */
    qint32 calcIncome(float modifier = 1.0f);
    /**
     * @brief getBonusIncome
     * @param pBuilding
     * @param income
     * @return
     */
    qint32 getIncomeReduction(Building* pBuilding, qint32 income);
    /**
     * @brief getCO
     * @param id index of the co 0 or 1
     * @return pointer to the co
     */
    CO* getCO(quint8 id);
    /**
     * @brief setCO
     * @param coId the co we want to load
     * @param id the index at which we want this co
     */
    void setCO(QString coId, quint8 idx);
    /**
     * @brief getCostModifier
     * @param baseCost
     * @return
     */
    qint32 getCostModifier(QString id, qint32 baseCost);
    /**
     * @brief getCosts
     * @param id
     * @return
     */
    qint32 getCosts(QString id);
    /**
     * @brief gainPowerstar
     * @param fundsDamage
     */
    void gainPowerstar(qint32 fundsDamage, QPoint position);
    /**
     * @brief startOfTurn
     */
    void startOfTurn();
    /**
     * @brief getUnits
     * @return
     */
    QmlVectorUnit* getUnits();
    /**
     * @brief getEnemyUnits
     * @return
     */
    QmlVectorUnit* getEnemyUnits();
    /**
     * @brief getEnemyBuildings
     * @return
     */
    QmlVectorBuilding* getEnemyBuildings();
    /**
     * @brief getEnemyCount
     * @return
     */
    qint32 getEnemyCount();
    /**
     * @brief getBuildings
     * @return
     */
    QmlVectorBuilding* getBuildings();
    /**
     * @brief updateCORange
     */
    void updateVisualCORange();
    /**
     * @brief getMovementcostModifier
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getMovementcostModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getRockettarget finds the most valuable target for damaging units in a diamond.
     * If multiple targets with the same value are found.
     * One is selected at random
     * @param radius
     * @param damage
     * @param ownUnitValue value of own or allied units compared to enemy ones.
     * @return -1, -1 for no target found
     */
    QPoint getRockettarget(qint32 radius, qint32 damage, float ownUnitValue = 1.2f, GameEnums::RocketTarget targetType = GameEnums::RocketTarget_Money);
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
    qint32 getRocketTargetDamage(qint32 x, qint32 y, QmlVectorPoint* pPoints, qint32 damage, float ownUnitValue, GameEnums::RocketTarget targetType);
    /**
     * @brief defineArmy defines our army sprites based on the current co at position 0
     */
    void defineArmy();
    /**
     * @brief getFundsModifier
     * @return
     */
    float getFundsModifier() const;
    /**
     * @brief setFundsModifier
     * @param value
     */
    void setFundsModifier(float value);
    /**
     * @brief calculatePlayerStrength
     * @return
     */
    qint32 calculatePlayerStrength();
    /**
     * @brief getBuildingCount
     * @return
     */
    qint32 getBuildingCount(QString buildingID = "");
    /**
     * @brief getUnitCount
     * @return
     */
    qint32 getUnitCount(QString unitID = "");
    /**
     * @brief getUnitCount
     * @param pUnit
     * @param unitID
     * @return
     */
    qint32 getUnitCount(Unit* pUnit, QString unitID);
    /**
     * @brief getTeam
     * @return
     */
    qint32 getTeam() const;
    /**
     * @brief setTeam
     * @param value
     */
    void setTeam(const qint32 &value);
    /**
     * @brief defeatPlayer defeats the player and removes him from further playing
     * @param pPLayer none nullptr will give all buildings to the selected player
     * @param units if units is true the player will also get the controll of the given player
     */
    void defeatPlayer(Player* pPLayer, bool units = false);
    /**
     * @brief postBattleActions
     * @param pAttacker
     * @param pDefender
     */
    void postBattleActions(Unit* pAttacker, float atkDamage, Unit* pDefender, bool gotAttacked);
    /**
     * @brief updatePlayerVision updates the current vision of this player.
     * @brief reduces the timer for vision created for several turns
     */
    void updatePlayerVision(bool reduceTimer = false);
    /**
     * @brief addVisionField
     * @param x
     * @param y
     * @param duration
     */
    void addVisionField(qint32 x, qint32 y, qint32 duration = 1, bool directView = false);
    /**
     * @brief getFieldVisible
     * @param x
     * @param y
     */
    bool getFieldVisible(qint32 x, qint32 y);
    /**
     * @brief getFieldDirectVisible
     * @param x
     * @param y
     * @return if true a stealthed unit will be made visible by this players view
     */
    bool getFieldDirectVisible(qint32 x, qint32 y);
    /**
     * @brief loadVisionFields
     */
    void loadVisionFields();
    /**
     * @brief loadCOMusic
     */
    void loadCOMusic();
    /**
     * @brief buildedUnit called after a unit was created do whatever you want with this information
     * @param pUnit
     */
    void buildedUnit(Unit* pUnit);
    /**
     * @brief getBuildList
     * @return
     */
    QStringList getBuildList() const;
    /**
     * @brief getCOUnits
     * @return
     */
    QStringList getCOUnits(Building* pBuilding);
    /**
     * @brief setBuildList
     * @param BuildList
     */
    void setBuildList(const QStringList &BuildList);
    /**
     * @brief changeBuildlist
     * @param unitID
     * @param remove
     */
    void changeBuildlist(const QString& unitID, bool remove = false);
private:
    qint32 funds{0};
    float fundsModifier{1.0f};

    QColor m_Color;
    QString playerArmy{""};
    qint32 team{0};
    spCO playerCOs[2]{nullptr, nullptr};
    /**
     * @brief m_pBaseGameInput pointer to the ai or human player
     */
    spBaseGameInputIF m_pBaseGameInput{nullptr};
    bool isDefeated{false};
    QVector<QVector<std::tuple<bool, qint32, bool>>> m_FogVisionFields;
    /**
     * @brief m_BuildList contains all units we're allowed to build
     */
    QStringList m_BuildList;
};

#endif // PLAYER_H
