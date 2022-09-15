#ifndef NORMALAI_H
#define NORMALAI_H

#include <QObject>

#include "ai/coreai.h"

#include "ai/influencefrontmap.h"

class QmlVectorUnit;
class QmlVectorBuilding;
class QmlVectorPoint;
class Building;

class NormalAi;
using spNormalAi = oxygine::intrusive_ptr<NormalAi>;

class NormalAi final : public CoreAI
{
    Q_OBJECT
    enum FundsMode
    {
        Normal,
        Expensive,
        VeryExpensive,
        NoSpendMalus
    };

    static constexpr float NO_BUILD_SCORE = std::numeric_limits<float>::lowest();
    struct UnitBuildData
    {
        QString unitId;
        bool isTransporter{false};
        bool canMove{false};
        bool indirectUnit{false};
        bool infantryUnit{false};
        bool isSupplyUnit{false};
        qint32 cost{0};
        qint32 baseRange{0};
        float damage{0.0f};
        float counterDamage{0.0f};
        qint32 notAttackableCount{0};
        float coBonus{0.0f};
        float closestTarget{0.0f};
        qint32 movePoints{0};
        qint32 attackCount{0};
        float sameFundsMatchUpScore{0.0f};
        float turnOneDamage{0.0f};

        qint32 smallTransporterCount{0};
        qint32 loadingPlace{0};
        qint32 noTransporterBonus{0};
        qint32 transportCount{0};
        qint32 loadingCount{0};
        qint32 transporterCount{0};
        bool utilityTransporter{false};
        bool isSmallTransporter{false};
    };
    struct ProductionData
    {
        qint32 m_x = -1;
        qint32 m_y = -1;
        QStringList buildList;
        QVector<UnitBuildData> m_buildData;
    };
    struct ExpectedFundsData
    {
        float damage{0.0f};
        float counterDamage{0.0f};
        float notAttackableCount{0};
        qint32 attackCount{0};
        float turnOneDamage{0.0f};
    };

    struct NotAttackableData
    {
        qint32 sameIslandNotAttackableCount{0};
        qint32 notAttackableCount{0};
        qint32 midDamageCount{0};
        qint32 highDamageCount{0};
    };


public:
    enum BuildItems
    {
        DirectUnitRatio = 0,
        InfantryUnitRatio = 1,
        IndirectUnit = 2,
        DirectUnit = 3,
        InfantryUnit = 4,
        FundsFactoryRatio = 5,
        BuildingEnemyRatio = 6,
        NotAttackableCount = 7,
        DamageData = 8,
        InfantryCount = 9,
        COBonus = 10,
        Movementpoints = 11,
        UnitEnemyRatio = 12,
        UnitCount = 13,
        ReachDistance = 14,
        UnitCost = 15,
        UseHighTechUnits = 16,
        SupplyRatio = 17,
        RequiredSupplyRatio = 18,
        LowFunds = 19,
        AttackCount = 20,
        SameFundsMatchUpScore = 21,
        CounterDamage = 22,
        EnemyUnitCount = 23,
        TurnOneDamageMalus = 24,
        Max,
    };

    explicit NormalAi(GameMap* pMap, QString configurationFile, GameEnums::AiTypes aiType, QString jsName);
    ~NormalAi() = default;
signals:

public slots:
    virtual void process() override;
    /*******************************************************************/
    // debugging section
    /*******************************************************************/
    /**
     * @brief toggleAiPause for debugging
     */
    void toggleAiPause();
    /**
     * @brief showIslandMap
     * @param unitId
     */
    void showIslandMap(QString unitId);
    /**
     * @brief showIslandMap
     * @param unitId
     */
    void hideIslandMap(QString unitId);
    /**
     * @brief showFrontMap for debugging visualization
     */
    void showFrontMap();
    /**
     * @brief showFrontLines for debugging visualization
     */
    void showFrontLines();
    /**
     * @brief hideFrontMap  for debugging visualization
     */
    void hideFrontMap();
protected:
    static constexpr float maxDayDistance = 6.0f;

    bool performActionSteps(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits,
                            spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief finishTurn
     */
    virtual void finishTurn() override;
    /**
     * @brief isUsingUnit
     * @param pUnit
     * @return
     */
    bool isUsingUnit(Unit* pUnit);
    /**
     * @brief moveUnits
     * @param pUnits
     * @param pBuildings
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool moveUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings,
                   spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                   qint32 minfireRange, qint32 maxfireRange, bool supportUnits = false);
    /**
     * @brief refillUnits
     * @param pUnits
     * @param pBuildings
     * @param pEnemyBuildings
     * @return
     */
    bool refillUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief getBestRefillTarget
     * @param pfs
     * @param maxRefillCount
     * @param moveTarget
     * @param refillTarget
     * @return
     */
    bool getBestRefillTarget(UnitPathFindingSystem & pfs, qint32 maxRefillCount, QPoint & moveTarget, QPoint & refillTarget, qint32 movepoints)const;
    /**
     * @brief appendRefillTargets
     * @param actions
     * @param pUnit
     * @param pUnits
     * @param targets
     */
    void appendRefillTargets(const QStringList & actions, Unit* pUnit, spQmlVectorUnit & pUnits, std::vector<QVector3D>& targets) const;
    /**
     * @brief moveUnit
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @param transporterTargets
     * @return
     */
    bool moveUnit(spGameAction & pAction, MoveUnitData* pUnitData, spQmlVectorUnit & pUnits, QStringList& actions,
                  std::vector<QVector3D>& targets, std::vector<QVector3D>& transporterTargets,
                  bool shortenPathForTarget,
                  spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief loadUnits
     * @param pUnits
     * @return
     */
    bool loadUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief NormalAi::moveTransporters
     * @param pUnits
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool moveTransporters(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief moveToUnloadArea
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @return
     */
    bool moveToUnloadArea(spGameAction & pAction, MoveUnitData* pUnitData, spQmlVectorUnit & pUnits, QStringList& actions,
                          std::vector<QVector3D>& targets,
                          spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                          spQmlVectorUnit & pEnemyUnits);
    /**
     * @brief unloadUnits
     * @param pAction
     * @param pUnit
     * @return
     */
    bool unloadUnits(spGameAction & pAction, Unit* pUnit, spQmlVectorUnit & pEnemyUnits);
    /**
     * @brief repairUnits
     * @param pUnits
     * @param pBuildings
     * @return
     */
    bool repairUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief getMoveTargetField shortens the movepath so we take no damage
     * @param pUnit
     * @param movePath
     * @return
     */
    qint32 getMoveTargetField(MoveUnitData & unitData, UnitPathFindingSystem& turnPfs,
                              std::vector<QPoint>& movePath, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                              qint32 movePoints) const;
    /**
     * @brief moveToSafety
     * @param pUnit
     * @param turnPfs
     * @return
     */
    std::tuple<QPoint, float, bool> moveToSafety(MoveUnitData & unitData, UnitPathFindingSystem& turnPfs, QPoint target,
                                                 spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                                                 qint32 movePoints);
    /**
     * @brief captureBuildings
     * @param pUnits
     * @return
     */
    bool captureBuildings(spQmlVectorUnit & pUnits);
    /**
     * @brief joinCaptureBuildings
     * @param pUnits
     * @return
     */
    bool joinCaptureBuildings(spQmlVectorUnit & pUnits);
    /**
     * @brief fireWithIndirectUnits
     * @param pUnits
     * @return
     */
    bool fireWithUnits(spQmlVectorUnit & pUnits, qint32 minfireRange, qint32 maxfireRange,
                       spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief suicide
     * @param pAction
     * @param pUnit
     * @param turnPfs
     * @return
     */
    bool suicide(spGameAction & pAction, Unit* pUnit, UnitPathFindingSystem& turnPfs, qint32 movepoints);
    /**
     * @brief getIndirectTarget
     * @param pUnit
     * @param ret
     * @param moveTargetFields
     * @return
     */
    qint32 getIndirectTarget(Unit* pUnit, std::vector<QVector3D>& ret, std::vector<QVector3D>& moveTargetFields);
    /**
     * @brief getBestAttackTarget
     * @param pUnit
     * @param ret
     * @param moveTargetFields
     * @return
     */
    qint32 getBestAttackTarget(MoveUnitData & unitData, std::vector<CoreAI::DamageData>& ret,
                               std::vector<QVector3D>& moveTargetFields,
                               spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings) const;
    /**
     * @brief calcVirtualDamage
     */
    void calcVirtualDamage();
    /**
     * @brief calculateCaptureBonus
     * @param pUnit
     * @param newLife
     * @return
     */
    float calculateCaptureBonus(Unit* pUnit, float newLife) const;
    /**
     * @brief calculateCounterDamage
     * @param pUnit
     * @param newPosition
     * @param pEnemyUnit
     * @param enemyNewLife
     */
    float calculateCounterDamage(MoveUnitData & curUnitData, QPoint newPosition,
                                 Unit* pEnemyUnit, float enemyTakenDamage,
                                 spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                                 bool ignoreOutOfVisionRange = false) const;
    /**
     * @brief calculateCounteBuildingDamage
     * @param pUnit
     * @param pBuildings
     * @param pEnemyBuildings
     * @return
     */
    float calculateCounteBuildingDamage(Unit* pUnit, QPoint newPosition, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings) const;
    /**
     * @brief clearEnemyData
     */
    void clearUnitData();
    /**
     * @brief buildUnits
     * @param pBuildings
     * @param pUnits
     * @return
     */
    bool buildUnits(spQmlVectorBuilding & pBuildings, spQmlVectorUnit & pUnits,
                    spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief getEnemyDamageCounts
     * @param pUnits
     * @param pEnemyUnits
     * @param attackCount
     */
    void getEnemyDamageCounts(spQmlVectorUnit & pUnits,spQmlVectorUnit & pEnemyUnits, std::vector<NotAttackableData> & attackCount);
    /**
     * @brief getIndexInProductionData
     * @param pBuilding
     * @return
     */
    qint32 getIndexInProductionData(Building* pBuilding);
    /**
     * @brief getUnitProductionIdx
     * @param index
     * @param unitId
     * @return
     */
    qint32 getUnitProductionIdx(qint32 index, const QString & unitId,
                                spQmlVectorUnit & pUnits, std::vector<std::tuple<Unit*, Unit*>> & transportTargets,
                                spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                std::vector<NotAttackableData> & attackCount, std::vector<float> & buildData);
    /**
     * @brief calcBuildScore
     * @param data
     * @return
     */
    float calcBuildScore(std::vector<float>& data, UnitBuildData & unitBuildData);
    /**
     * @brief createUnitBuildData
     * @param x
     * @param y
     * @param unitBuildData
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @param attackCount
     * @param buildData
     */
    void createUnitBuildData(qint32 x, qint32 y, UnitBuildData & unitBuildData,
                             spQmlVectorUnit & pUnits, std::vector<std::tuple<Unit*, Unit*>> & transportTargets,
                             spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                             std::vector<NotAttackableData> & attackCount, std::vector<float> & buildData, const QStringList & buildList);
    /**
     * @brief calcCostScore
     * @param data
     * @return
     */
    float calcCostScore(std::vector<float>& data, UnitBuildData & unitBuildData);
    /**
     * @brief getTransporterData
     * @param unitBuildData
     * @param dummy
     * @param pUnits
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @param transportTargets
     */
    void getTransporterData(UnitBuildData & unitBuildData, Unit& dummy, spQmlVectorUnit & pUnits,
                            spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                            std::vector<std::tuple<Unit*, Unit*>>& transportTargets);
    /**
     * @brief calcTransporterScore
     * @param posX
     * @param posY
     * @param dummy
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    float calcTransporterScore(UnitBuildData & unitBuildData,  spQmlVectorUnit & pUnits, std::vector<float>& data);
    /**
     * @brief NormalAi::calcExpectedFundsDamage
     * @param dummy
     * @param pEnemyUnits
     * @return
     */
    ExpectedFundsData calcExpectedFundsDamage(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit & pEnemyUnits, const std::vector<NotAttackableData> & attackCount, float bonusFactor, float myMovepoints);
    /**
     * @brief getClosestTargetDistance
     * @param posX
     * @param posY
     * @param dummy
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    qint32 getClosestTargetDistance(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief calcSupplyScore
     * @param data
     * @param unitBuildData
     * @return
     */
    float calcSupplyScore(std::vector<float>& data, UnitBuildData & unitBuildData);
    /**
     * @brief updateAllUnitData
     * @param pUnits
     */
    void updateAllUnitData(spQmlVectorUnit & pUnits);
    /**
     * @brief updateUnitData
     * @param pUnitData
     * @param enemy
     */
    void updateUnitData(spQmlVectorUnit & pUnits, std::vector<MoveUnitData> & pUnitData, bool enemy);
    /**
     * @brief createUnitData
     * @param pUnit
     * @param data
     * @param enemy
     */
    void createUnitData(Unit* pUnit, MoveUnitData & data, bool enemy, qint32 moveMultiplier);
    /**
     * @brief createUnitInfluenceMap
     */
    void createUnitInfluenceMap();
    /**
     * @brief getOwnSupportDamage
     * @param moveTarget
     * @param pEnemy
     * @return
     */
    float getOwnSupportDamage(Unit* pUnit, QPoint moveTarget, Unit* pEnemy, float & hpDamage) const;
    /**
     * @brief calcSameFundsMatchUpScore
     * @param dummy
     * @param buildList
     * @return
     */
    float calcSameFundsMatchUpScore(Unit& dummy, const QStringList & buildList);
    /**
     * @brief getMapInfluenceModifier
     * @param pUnit
     * @param x
     * @param y
     * @return
     */
    float getMapInfluenceModifier(Unit* pUnit, qint32 x, qint32 y) const;
private:
    /**
     * @brief m_EnemyUnits all enemy units that exists at the start of turn
     */
    std::vector<MoveUnitData> m_EnemyUnits;
    /**
     * @brief m_OwnUnits
     */
    std::vector<MoveUnitData> m_OwnUnits;

    /**
     * @brief updatePoints points we need to update for the next pfs
     */
    std::vector<QPoint> m_updatePoints;
    /**
     * @brief m_productionData
     */
    std::vector<ProductionData> m_productionData;
    InfluenceFrontMap m_InfluenceFrontMap;


    double m_notAttackableDamage{25.0f};
    double m_midDamage{55.0f};
    double m_highDamage{75.0f};
    double m_directIndirectRatio{1.75f};
    double m_minSiloDamage{7000};
    double m_minMovementDamage{0.3f};
    double m_minAttackFunds{0.0f};
    double m_minSuicideDamage{0.75f};
    double m_spamingFunds{7500};
    double m_ownProdctionMalus{5000.0f};

    double m_minUnitHealth{3};
    double m_maxUnitHealth{7};
    double m_lockedUnitHp{4};
    double m_noMoveAttackHp{3.5f};
    double m_ownIndirectAttackValue{2.0f};
    double m_enemyKillBonus{2.0f};
    double m_enemyIndirectBonus{3.0f};
    double m_antiCaptureHqBonus{50.0f};
    double m_antiCaptureBonus{21.0f};
    double m_antiCaptureBonusScoreReduction{6.0f};
    double m_antiCaptureBonusScoreDivider{2.0f};
    double m_enemyCounterDamageMultiplier{10.0f};
    double m_watermineDamage{4.0f};
    double m_enemyUnitCountDamageReductionMultiplier{0.5f};
    double m_fundsPerBuildingFactorA{2.5f};
    double m_fundsPerBuildingFactorB{1.65f};
    double m_fundsPerBuildingFactorD{3.0f};
    double m_ownUnitEnemyUnitRatioAverager{10};
    double m_maxDayScoreVariancer{10};
    double m_directIndirectUnitBonusFactor{1.2f};
    double m_influenceUnitRange{1.75f};

    double m_scoringCutOffDamageHigh{Unit::DAMAGE_100};
    double m_scoringCutOffDamageLow{7.5f};
    double m_smoothingValue{3};
    double m_maxDistanceMultiplier{1.5f};
    double m_sameIslandBonusInRangeDays{2};
    double m_sameIslandOutOfDayMalusFactor{0.2f};
    double m_highDamageBonus{2};
    double m_midDamageBonus{1.5f};
    double m_lowDamageBonus{1};
    double m_veryLowDamageBonus{0.5f};
    double m_transportBonus{0.125f};
    double m_currentlyNotAttackableBonus{0.5};
    double m_differentIslandBonusInRangeDays{1};
    double m_differentIslandOutOfDayMalusFactor{0.33f};
    double m_noTransporterBonus{70};
    double m_transporterToRequiredPlaceFactor{3};
    double m_minFlyingTransportScoreForBonus{15};
    double m_flyingTransporterBonus{15};
    double m_smallTransporterBonus{30};
    double m_unitToSmallTransporterRatio{5};
    double m_additionalLoadingUnitBonus{5};
    double m_indirectUnitAttackCountMalus{4};
    double m_minAttackCountBonus{5};
    double m_lowIndirectUnitBonus{0.3f};
    double m_lowIndirectMalus{30.0f};
    double m_highIndirectMalus{0.6f};
    double m_lowDirectUnitBonus{0.35f};
    double m_lowDirectMalus{0.3f};
    double m_highDirectMalus{0.6f};
    double m_minUnitCountForDamageBonus{3};
    double m_minInfantryCount{5};
    double m_currentlyNotAttackableScoreBonus{30};
    double m_coUnitBuffBonus{17};
    double m_nearEnemyBonus{10};
    double m_lowOwnBuildingEnemyBuildingRatio{1.25f};
    double m_lowInfantryRatio{0.4f};
    double m_buildingBonusMultiplier{4.0f};
    double m_lowIncomeInfantryBonusMultiplier{50};
    double m_movementpointBonus{3.0f};
    double m_damageToUnitCostRatioBonus{20};
    double m_superiorityRatio{2.5f};
    double m_cheapUnitRatio{1.0f};
    double m_normalUnitRatio{1.0f};
    double m_cheapUnitBonusMultiplier{45};
    double m_normalUnitBonusMultiplier{10};
    double m_expensiveUnitBonusMultiplier{15};
    double m_canSupplyBonus{10};
    double m_maxSupplyUnitRatio{0.05f};
    double m_averageSupplySupport{8};
    double m_cappingFunds{4700};
    double m_cappedFunds{1999};
    double m_targetPriceDifference{0.3f};
    double m_sameFundsMatchUpNoMatchUpValue{0.5f};
    double m_fundsPerBuildingFactorC{4};
    double m_cheapUnitValue{3000.0f};
    double m_supportDamageBonus{1.0f};
    double m_maxCloseDistanceDamageBonus{1.0f};
    double m_minCloseDistanceDamageBonus{0.2f};
    double m_earlyGame{5};
    double m_startDayScoreVariancer{5.0f};
    double m_sameFundsMatchUpBonus{16.0f};
    double m_sameFundsMatchUpMovementMalus{0.3f};
    double m_counterDamageBonus{25.0f};
    double m_attackCountBonus{25.0f};
    double m_maxOverkillBonus{2.0f};
    double m_influenceIgnoreValue{0.2};
    double m_influenceMultiplier{2};
    double m_lowThreadDamage{10};
    double m_ProducingTransportSearchrange{6};
    double m_ProducingTransportSizeBonus{10};
    double m_ProducingTransportRatioBonus{1.7f};
    double m_ProducingTransportLoadingBonus{15.0f};
    double m_ProducingTransportMinLoadingTransportRatio{7.0f};
    double m_maxProductionBuildings{5};
    double m_maxProductionBuildingsForB{1};
    double m_turnOneDmageMalus{5};
    double m_counterUnitRatio{2};
    double m_spamInfantryChance{50};

    float m_currentDirectIndirectRatio{1.0f};

    QTimer m_timer;
    bool m_pause{false};
    bool m_secondMoveRound{false};
};

Q_DECLARE_INTERFACE(NormalAi, "NormalAi");

#endif // NORMALAI_H
