#ifndef KO_H
#define KO_H

#include <QObject>
#include <QPoint>

#include "game/GameEnums.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/fileserializable.h"

#include "coreengine/scriptvariables.h"

class Unit;
class Terrain;
class Player;
class Building;
class GameAnimationDialog;
class GameAnimationPower;
class GameAction;

class CO;
typedef oxygine::intrusive_ptr<CO> spCO;

class CO : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT
public:
    static constexpr qint32 MAX_CO_UNIT_VALUE = 10;

    explicit CO(QString coID, Player* owner);
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
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
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 6;
    }
    /**
     * @brief writeCoStyleToStream
     * @param pStream
     */
    void writeCoStyleToStream(QDataStream& pStream) const;
    /**
     * @brief readCoStyleFromStream
     * @param pStream
     */
    void readCoStyleFromStream(QDataStream& pStream);
    /**
     * @brief init
     */
    void init();
    /**
     * @brief isValid
     * @return
     */
    bool isValid();

    /**
     * @brief getResAnim
     * @param id
     * @param ep
     * @return
     */
    oxygine::ResAnim* getResAnim(QString id, oxygine::error_policy ep = oxygine::ep_show_error) const;


signals:

public slots:
    /**
     * @brief getCoRangeEnabled
     * @return
     */
    bool getCoRangeEnabled() const;
    /**
     * @brief setCoRangeEnabled
     * @param coRangeEnabled
     */
    void setCoRangeEnabled(bool coRangeEnabled);
    /**
     * @brief getPowerUsed
     * @return
     */
    qint32 getPowerUsed() const;
    /**
     * @brief setPowerUsed
     * @param value
     */
    void setPowerUsed(const qint32 &value);
    /**
     * @brief getUnitBuildValue
     * @param unitID
     * @return
     */
    float getUnitBuildValue(QString unitID);
    /**
     * @brief getCOName
     * @return
     */
    QString getCOName();
    /**
     * @brief getCOUnits
     * @param pBuilding
     * @return
     */
    QStringList getCOUnits(Building* pBuilding);
    /**
     * @brief getMovementpointModifier the bonus movementpoints of this co
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getMovementpointModifier(Unit* pUnit, QPoint position);
    /**
     * @brief buildedUnit called after a unit was created do whatever you want with this information
     * @param pUnit
     */
    void buildedUnit(Unit* pUnit);
    /**
     * @brief getFirerangeModifier the bonus firerange of this co
     * @param pUnit the unit we want to get the bonus points from
     * @return
     */
    qint32 getFirerangeModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getMinFirerangeModifier
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getMinFirerangeModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getHpHidden
     * @param pUnit
     * @param position
     * @return
     */
    bool getHpHidden(Unit* pUnit, QPoint position);
    /**
     * @brief getPerfectHpView
     * @param pUnit
     * @param position
     * @return
     */
    bool getPerfectHpView(Unit* pUnit, QPoint position);
    /**
     * @brief getMovementcostModifier
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getMovementcostModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getAttackHpBonus
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getAttackHpBonus(Unit* pUnit, QPoint position);
    /**
     * @brief getBonusLuck
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getBonusLuck(Unit* pUnit, QPoint position);
    /**
     * @brief getBonusMisfortune
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getBonusMisfortune(Unit* pUnit, QPoint position);
    /**
     * @brief getTerrainDefenseModifier the bonus defense of this co for a terrain
     * @param pUnit the unit we want to get the bonus points from
     * @return
     */
    qint32 getTerrainDefenseModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getFirstStrike
     * @param pUnit
     * @param position
     * @return
     */
    bool getFirstStrike(Unit* pUnit, QPoint position, Unit* pAttacker);
    /**
     * @brief getVisionrangeModifier the bonus defense of this co for a terrain
     * @param pUnit the unit we want to get the bonus points from
     * @return
     */
    qint32 getVisionrangeModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getEnemyTerrainDefenseModifier
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getEnemyTerrainDefenseModifier(Unit* pUnit, QPoint position);
    /**
     * @brief activatePower called when the power is activated
     */
    void activatePower();
    /**
     * @brief activateSuperpower called when the super power is activated
     */
    void activateSuperpower(GameEnums::PowerMode powerMode);
    /**
     * @brief addUnitShine
     */
    void addUnitShines();
    /**
     * @brief getOffensiveBonus
     * @param pAttacker the attacking unit our unit
     * @param pDefender the defending unit the unit we attack
     * @return
     */
    qint32 getOffensiveBonus(Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender);
    /**
     * @brief getOffensiveReduction
     * @param pAttacker the attacking unit our unit
     * @param pDefender the defending unit the unit we attack
     * @return
     */
    qint32 getOffensiveReduction(Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender);
    /**
     * @brief getDeffensiveBonus the deffensive bonus of our unit
     * @param pAttacker the unit attacking us
     * @param pDefender our unit that gets attacked
     * @return bonus
     */
    qint32 getDeffensiveBonus(Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isDefender);
    /**
     * @brief getDeffensiveBonus the deffensive bonus of our unit
     * @param pAttacker the unit attacking us
     * @param pDefender our unit that gets attacked
     * @return bonus
     */
    qint32 getDeffensiveReduction(Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isDefender);

    /**
     * @brief getDamageReduction
     * @param damage
     * @param pAttacker
     * @param position
     * @param attackerBaseHp
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    float getDamageReduction(float damage, Unit* pAttacker, QPoint atkPosition, qint32 attackerBaseHp,
                             Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode);
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
    float getTrueDamage(float damage, Unit* pAttacker, QPoint atkPosition, qint32 attackerBaseHp,
                        Unit* pDefender, QPoint defPosition, bool isDefender);
    /**
     * @brief getMovementFuelCostModifier
     * @param pUnit
     * @param fuelCost
     * @return
     */
    qint32 getMovementFuelCostModifier(Unit* pUnit, qint32 fuelCost);
    /**
     * @brief getCoID our co id
     * @return
     */
    QString getCoID() const;
    /**
     * @brief getPowerStars stars needed to activate our power
     * @return
     */
    qint32 getPowerStars() const;
    /**
     * @brief setPowerStars stars needed to activate our power
     * @param value
     */
    void setPowerStars(const qint32 &value);
    /**
     * @brief getSuperpowerStars stars needed to activate our super power
     * @return
     */
    qint32 getSuperpowerStars() const;
    /**
     * @brief setSuperpowerStars stars needed to activate our super power
     * @param value
     */
    void setSuperpowerStars(const qint32 &value);
    /**
     * @brief canUseSuperpower
     * @return
     */
    bool canUseSuperpower() const;
    /**
     * @brief canUsePower
     * @return
     */
    bool canUsePower() const;
    /**
     * @brief getPowerFilled
     * @return
     */
    double getPowerFilled() const;
    /**
     * @brief setPowerFilled
     * @param value
     */
    void setPowerFilled(const double &value);
    /**
     * @brief addPowerFilled
     * @param value
     */
    void addPowerFilled(const double &value);
    /**
     * @brief getCanMoveAndFire returns
     * @return
     */
    bool getCanMoveAndFire(Unit* pUnit, QPoint position);
    /**
     * @brief getRepairBonus
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getRepairBonus(Unit* pUnit, QPoint position);
    /**
     * @brief canBeRepaired
     * @param pUnit
     * @param position
     * @return
     */
    bool canBeRepaired(Unit* pUnit, QPoint position);
    /**
     * @brief getCaptureBonus
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getCaptureBonus(Unit* pUnit, QPoint position);
    /**
     * @brief getCostModifier
     * @param baseCost
     * @return
     */
    qint32 getCostModifier(QString id, qint32 baseCost);
    /**
     * @brief getCOArmy
     */
    QString getCOArmy();
    /**
     * @brief getPowerMode
     * @return
     */
    GameEnums::PowerMode getPowerMode() const;
    /**
     * @brief setPowerMode
     * @param PowerMode
     */
    void setPowerMode(const GameEnums::PowerMode &PowerMode);
    /**
     * @brief gainPower
     * @param fundsDamage
     * @param position
     */
    void gainPowerstar(qint32 fundsDamage, QPoint position, qint32 hpDamage, bool defender, bool counterAttack);
    /**
     * @brief startOfTurn called at the start of our turn
     */
    void startOfTurn();
    /**
     * @brief getPlayer
     * @return
     */
    inline Player* getOwner()
    {
        return m_Owner;
    }
    /**
     * @brief getCOUnit
     * @return
     */
    Unit* getCOUnit();
    /**
     * @brief setCOUnit
     * @param pUnit
     * @return
     */
    void setCOUnit(Unit* pUnit);
    /**
     * @brief getActionModifierList
     * @param pUnit
     * @return
     */
    QStringList getActionModifierList(Unit* pUnit);
    /**
     * @brief getCORange
     * @return
     */
    qint32 getCORange();
    /**
     * @brief inCORange
     * @param position
     * @param pUnit
     * @return
     */
    bool inCORange(QPoint position, Unit* pUnit);
    /**
     * @brief getFuelCostModifier
     * @param pUnit
     * @param position
     * @param costs
     * @return
     */
    qint32 getFuelCostModifier(Unit* pUnit, QPoint position, qint32 costs);
    /**
     * @brief getBuildingActions
     * @param pBuilding
     * @return
     */
    QString getAdditionalBuildingActions(Building* pBuilding);
    /**
     * @brief getBonusIncome
     * @param pBuilding
     * @param income
     * @return
     */
    qint32 getBonusIncome(Building* pBuilding, qint32 income);
    /**
     * @brief getIncomeReduction
     * @param pBuilding
     * @param income
     * @return
     */
    qint32 getIncomeReduction(Building* pBuilding, qint32 income);
    /**
     * @brief getPerfectVision
     * @return
     */
    bool getPerfectVision();
    /**
     * @brief getWeatherImmune
     * @return
     */
    bool getWeatherImmune();
    /**
     * @brief postBattleActions
     * @param pAttacker
     * @param atkDamage
     * @param pDefender
     */
    void postBattleActions(Unit* pAttacker, float atkDamage, Unit* pDefender, bool gotAttacked);
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief loadCOMusic
     */
    void loadCOMusic();
    /**
     * @brief createPowerSentence
     * @return
     */
    GameAnimationDialog* createPowerSentence();
    /**
     * @brief getDefeatSentence
     * @return
     */
    QString getDefeatSentence();
    /**
     * @brief getVictorySentence
     * @return
     */
    QString getVictorySentence();
    /**
     * @brief createPowerScreen
     * @return
     */
    GameAnimationPower* createPowerScreen(GameEnums::PowerMode powerMode);
    /**
     * @brief getIsCO0
     * @return
     */
    bool getIsCO0();
    /**
     * @brief getAiUsePower
     * @param powerSurplus
     * @param unitCount
     * @param repairUnits
     * @param indirectUnits
     * @param directUnits
     * @param enemyUnits
     * @param turnMode
     * @return
     */
    GameEnums::PowerMode getAiUsePower(double powerSurplus, qint32 unitCount, qint32 repairUnits,
                                       qint32 indirectUnits, qint32 directUnits, qint32 enemyUnits,
                                       GameEnums::AiTurnMode turnMode);

    /**
     * @brief getCoUnitBonus
     * @param pUnit
     * @param bonus
     * @return
     */
    float getAiCoUnitBonus(Unit* pUnit, bool & valid);
    /**
     * @brief getPerkList
     * @return
     */
    QStringList getPerkList();
    /**
     * @brief setPerkList
     * @param perks
     */
    void setPerkList(QStringList perks);
    /**
     * @brief addPerk
     * @param perk
     */
    void addPerk(QString perk);
    /**
     * @brief addPerk
     * @param perk
     */
    void removePerk(QString perk);
    /**
     * @brief getBio
     * @return
     */
    QString getBio();
    /**
     * @brief getLongBio
     * @return
     */
    QString getLongBio();
    /**
     * @brief getHits
     * @return
     */
    QString getHits();
    /**
     * @brief getMiss
     * @return
     */
    QString getMiss();
    /**
     * @brief getCODescription
     * @return
     */
    QString getCODescription();
    /**
     * @brief getLongCODescription
     * @return
     */
    QString getLongCODescription();
    /**
     * @brief getPowerDescription
     * @return
     */
    QString getPowerDescription();
    /**
     * @brief getPowerName
     * @return
     */
    QString getPowerName();
    /**
     * @brief getSuperPowerDescription
     * @return
     */
    QString getSuperPowerDescription();
    /**
     * @brief getSuperPowerName
     * @return
     */
    QString getSuperPowerName();
    /**
     * @brief postAction
     * @param pAction
     */
    void postAction(GameAction* pAction);
    /**
     * @brief setCoStyleFromUserdata
     */
    void setCoStyleFromUserdata();
    /**
     * @brief setCoStyle
     * @param file path to the co style relative to the commander wars exe
     * @param style index in the related predefined styles
     */
    void setCoStyle(QString file, qint32 style);
    /**
     * @brief postAction
     * @param pAction
     */
    /**
     * @brief getActiveCoStyle
     * @param coid
     */
    QString getActiveCoStyle();
protected:
    void limitPowerbar(float previousValue);

private:
    void loadResAnim(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox);
private:
    Player* m_Owner;
    QString coID;
    qint32 powerStars{0};
    qint32 superpowerStars{0};
    double powerFilled{0.0};
    Unit* m_pCOUnit{nullptr};
    GameEnums::PowerMode m_PowerMode{GameEnums::PowerMode_Off};
    ScriptVariables m_Variables;
    qint32 m_powerUsed{0};
    bool m_powerCharging{false};
    bool m_coRangeEnabled{true};

    QStringList m_perkList;

    QVector<std::tuple<QString, QString, QImage, QImage, bool>> m_customCOStyles;
    QVector<std::tuple<QString, oxygine::spResAnim>> m_Ressources;
};

#endif // KO_H
