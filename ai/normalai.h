#ifndef NORMALAI_H
#define NORMALAI_H

#include <QObject>

#include "ai/coreai.h"

#include "game/unitpathfindingsystem.h"
#include "ai/decisiontree.h"

class QmlVectorUnit;
class QmlVectorBuilding;
class QmlVectorPoint;
class Building;

class NormalAi;
using spNormalAi = oxygine::intrusive_ptr<NormalAi>;

class NormalAi : public CoreAI
{
    Q_OBJECT

    struct UnitBuildData
    {
        QString unitId;
        bool isTransporter{false};
        bool canMove{false};
        bool indirectUnit{false};
        bool infantryUnit{false};
        qint32 cost{0};
        qint32 baseRange{0};
        float damage{0.0f};
        qint32 notAttackableCount{0};
        float coBonus{0.0f};
        float closestTarget{0.0f};
        qint32 movePoints{0};

        qint32 smallTransporterCount{0};
        qint32 loadingPlace{0};
        qint32 noTransporterBonus{0};
        qint32 transportCount{0};
        qint32 loadingCount{0};
        bool flying{false};
        bool isSmallTransporter{false};
    };
    struct ProductionData
    {
        qint32 m_x = -1;
        qint32 m_y = -1;
        QVector<UnitBuildData> m_buildData;
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
        Max,
    };

    explicit NormalAi(QString configurationFile);
signals:

public slots:
    virtual void process() override;
    /**
     * @brief readIni
     * @param name
     */
    virtual void readIni(QString name) override;
protected:
    static constexpr float maxDayDistance = 6.0f;;

    bool performActionSteps(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits,
                            spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
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
    bool moveUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings,
                   spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                   qint32 minfireRange, qint32 maxfireRange, bool supportUnits = false);
    /**
     * @brief refillUnits
     * @param pUnits
     * @param pBuildings
     * @param pEnemyBuildings
     * @return
     */
    bool refillUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief getBestRefillTarget
     * @param pfs
     * @param maxRefillCount
     * @param moveTarget
     * @param refillTarget
     * @return
     */
    bool getBestRefillTarget(UnitPathFindingSystem & pfs, qint32 maxRefillCount, QPoint & moveTarget, QPoint & refillTarget);
    /**
     * @brief appendRefillTargets
     * @param actions
     * @param pUnit
     * @param pUnits
     * @param targets
     */
    void appendRefillTargets(QStringList actions, Unit* pUnit, spQmlVectorUnit pUnits, QVector<QVector3D>& targets);
    /**
     * @brief moveUnit
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @param transporterTargets
     * @return
     */
    bool moveUnit(spGameAction pAction, Unit* pUnit, spQmlVectorUnit pUnits, QStringList& actions,
                  QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets,
                  bool shortenPathForTarget,
                  spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief loadUnits
     * @param pUnits
     * @return
     */
    bool loadUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief NormalAi::moveTransporters
     * @param pUnits
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool moveTransporters(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief moveToUnloadArea
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @return
     */
    bool moveToUnloadArea(spGameAction pAction, Unit* pUnit, spQmlVectorUnit pUnits, QStringList& actions,
                          QVector<QVector3D>& targets,
                          spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief repairUnits
     * @param pUnits
     * @param pBuildings
     * @return
     */
    bool repairUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief getMoveTargetField shortens the movepath so we take no damage
     * @param pUnit
     * @param movePath
     * @return
     */
    qint32 getMoveTargetField(Unit* pUnit, spQmlVectorUnit pUnits, UnitPathFindingSystem& turnPfs,
                              QVector<QPoint>& movePath, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief moveToSafety
     * @param pUnit
     * @param turnPfs
     * @return
     */
    std::tuple<QPoint, float, bool> moveToSafety(Unit* pUnit, spQmlVectorUnit pUnits,
                                                 UnitPathFindingSystem& turnPfs, QPoint target,
                                                 spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief captureBuildings
     * @param pUnits
     * @return
     */
    bool captureBuildings(spQmlVectorUnit pUnits);
    /**
     * @brief fireWithIndirectUnits
     * @param pUnits
     * @return
     */
    bool fireWithUnits(spQmlVectorUnit pUnits, qint32 minfireRange, qint32 maxfireRange,
                       spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief suicide
     * @param pAction
     * @param pUnit
     * @param turnPfs
     * @return
     */
    bool suicide(spGameAction pAction, Unit* pUnit, UnitPathFindingSystem& turnPfs);
    /**
     * @brief getIndirectTarget
     * @param pUnit
     * @param ret
     * @param moveTargetFields
     * @return
     */
    qint32 getIndirectTarget(Unit* pUnit, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getBestAttackTarget
     * @param pUnit
     * @param ret
     * @param moveTargetFields
     * @return
     */
    qint32 getBestAttackTarget(Unit* pUnit, spQmlVectorUnit pUnits, QVector<QVector4D>& ret,
                               QVector<QVector3D>& moveTargetFields,
                               spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief updateEnemyData
     */
    void updateEnemyData(spQmlVectorUnit pUnits);
    /**
     * @brief calcVirtualDamage
     */
    void calcVirtualDamage(spQmlVectorUnit pUnits);
    /**
     * @brief calculateCaptureBonus
     * @param pUnit
     * @param newLife
     * @return
     */
    float calculateCaptureBonus(Unit* pUnit, float newLife);
    /**
     * @brief calculateCounterDamage
     * @param pUnit
     * @param newPosition
     * @param pEnemyUnit
     * @param enemyNewLife
     */
    float calculateCounterDamage(Unit* pUnit, spQmlVectorUnit pUnits, QPoint newPosition,
                                 Unit* pEnemyUnit, float enemyTakenDamage,
                                 spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings,
                                 bool ignoreOutOfVisionRange = false);
    /**
     * @brief calculateCounteBuildingDamage
     * @param pUnit
     * @param pBuildings
     * @param pEnemyBuildings
     * @return
     */
    float calculateCounteBuildingDamage(Unit* pUnit, QPoint newPosition, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief clearEnemyData
     */
    void clearEnemyData();
    /**
     * @brief buildUnits
     * @param pBuildings
     * @param pUnits
     * @return
     */
    bool buildUnits(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                    spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief getEnemyDamageCounts
     * @param pUnits
     * @param pEnemyUnits
     * @param attackCount
     */
    void getEnemyDamageCounts(spQmlVectorUnit pUnits,spQmlVectorUnit pEnemyUnits, QVector<QVector4D> & attackCount);
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
    qint32 getUnitProductionIdx(qint32 index, QString unitId,
                                spQmlVectorUnit pUnits, QVector<std::tuple<Unit*, Unit*>> & transportTargets,
                                spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                                QVector<QVector4D> & attackCount, QVector<float> & buildData);
    /**
     * @brief calcBuildScore
     * @param data
     * @return
     */
    float calcBuildScore(QVector<float>& data);
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
                             spQmlVectorUnit pUnits, QVector<std::tuple<Unit*, Unit*>> & transportTargets,
                             spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                             QVector<QVector4D> & attackCount, QVector<float> & buildData);
    /**
     * @brief calcCostScore
     * @param data
     * @return
     */
    float calcCostScore(QVector<float>& data);
    /**
     * @brief getTransporterData
     * @param unitBuildData
     * @param dummy
     * @param pUnits
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @param transportTargets
     */
    void getTransporterData(UnitBuildData & unitBuildData, Unit& dummy, spQmlVectorUnit pUnits,
                            spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                            QVector<std::tuple<Unit*, Unit*>>& transportTargets);
    /**
     * @brief calcTransporterScore
     * @param posX
     * @param posY
     * @param dummy
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    float calcTransporterScore(UnitBuildData & unitBuildData,  spQmlVectorUnit pUnits, QVector<float>& data);
    /**
     * @brief canTransportToEnemy
     * @param pUnit
     * @param pLoadedUnit
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool canTransportToEnemy(Unit* pUnit, Unit* pLoadedUnit, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief NormalAi::calcExpectedFundsDamage
     * @param dummy
     * @param pEnemyUnits
     * @return
     */
    std::tuple<float, qint32> calcExpectedFundsDamage(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit pEnemyUnits, QVector<QVector4D> attackCount, float bonusFactor);
    /**
     * @brief getClosestTargetDistance
     * @param posX
     * @param posY
     * @param dummy
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    qint32 getClosestTargetDistance(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings);
private:
    /**
     * @brief m_EnemyUnits all enemy units that exists at the start of turn
     */
    QVector<spUnit> m_EnemyUnits;
    /**
     * @brief m_EnemyPfs all enemy pfs currently correct.
     */
    QVector<spUnitPathFindingSystem> m_EnemyPfs;
    /**
     * @brief updatePoints points we need to update for the next pfs
     */
    QVector<QPoint> m_updatePoints;
    /**
     * @brief m_VirtualEnemyData
     */
    QVector<QPointF> m_VirtualEnemyData;
    /**
     * @brief m_productionData
     */
    QVector<ProductionData> m_productionData;


    float m_notAttackableDamage{25.0f};
    float m_midDamage{55.0f};
    float m_highDamage{65.0f};
    float m_directIndirectRatio{1.75f};
    qint32 m_minSiloDamage{7000};
    float m_minMovementDamage{0.3f};
    float m_minAttackFunds{0.0f};
    float m_minSuicideDamage{0.75f};
    float m_spamingFunds{7500};

    qint32 m_minUnitHealth{3};
    qint32 m_maxUnitHealth{7};
    float m_lockedUnitHp{4};
    float m_noMoveAttackHp{3.5f};
    float m_ownIndirectAttackValue{2.0f};
    float m_enemyKillBonus{2.0f};
    float m_enemyIndirectBonus{3.0f};
    float m_antiCaptureHqBonus{50.0f};
    float m_antiCaptureBonus{21.0f};
    float m_antiCaptureBonusScoreReduction{6.0f};
    float m_antiCaptureBonusScoreDivider{2.0f};
    float m_enemyCounterDamageMultiplier{10.0f};
    float m_watermineDamage{4.0f};
    float m_enemyUnitCountDamageReductionMultiplier{0.5f};
    float m_fundsPerBuildingFactorA{2.5f};
    float m_fundsPerBuildingFactorB{1.65f};
    float m_ownUnitEnemyUnitRatioAverager{10};
    float m_maxDayScoreVariancer{10};
    float m_directIndirectUnitBonusFactor{1.2f};

    float m_maxBuildingTargetFindLoops{5};
    float m_scoringCutOffDamageHigh{Unit::DAMAGE_100};
    float m_scoringCutOffDamageLow{7.5f};
    float m_smoothingValue{3};
    float m_maxDistanceMultiplier{1.5f};
    float m_sameIslandBonusInRangeDays{2};
    float m_sameIslandOutOfDayMalusFactor{0.2f};
    float m_highDamageBonus{2};
    float m_midDamageBonus{1.5f};
    float m_lowDamageBonus{1};
    float m_veryLowDamageBonus{0.5f};
    float m_transportBonus{0.125f};
    float m_currentlyNotAttackableBonus{0.5};
    float m_differentIslandBonusInRangeDays{1};
    float m_differentIslandOutOfDayMalusFactor{0.33f};
    float m_noTransporterBonus{70};
    float m_transporterToRequiredPlaceFactor{3};
    float m_minFlyingTransportScoreForBonus{15};
    float m_flyingTransporterBonus{15};
    float m_smallTransporterBonus{30};
    float m_unitToSmallTransporterRatio{5};
    float m_additionalLoadingUnitBonus{5};
    qint32 m_indirectUnitAttackCountMalus{4};
    float m_minAttackCountBonus{5};
    float m_lowIndirectUnitBonus{0.3f};
    float m_lowIndirectMalus{0.5f};
    float m_highIndirectMalus{0.6f};
    float m_lowDirectUnitBonus{0.35f};
    float m_lowDirectMalus{0.3f};
    float m_highDirectMalus{0.6f};
    float m_minUnitCountForDamageBonus{3};
    float m_minInfantryCount{5};
    float m_currentlyNotAttackableScoreBonus{30};
    float m_coUnitBuffBonus{17};
    float m_nearEnemyBonus{10};
    float m_lowOwnBuildingEnemyBuildingRatio{1.25f};
    float m_lowInfantryRatio{0.4f};
    float m_buildingBonusMultiplier{0.75f};
    float m_lowIncomeInfantryBonusMultiplier{50};
    float m_movementpointBonus{0.33f};
    float m_damageToUnitCostRatioBonus{20};
    float m_superiorityRatio{2.5f};
    float m_cheapUnitRatio{0.9f};
    float m_cheapUnitBonusMultiplier{40};
    float m_normalUnitBonusMultiplier{10};
    float m_expensiveUnitBonusMultiplier{5};

    float m_ProducingTransportSearchrange{6};
    float m_ProducingTransportSizeBonus{10};
    float m_ProducingTransportRatioBonus{1.7f};
    float m_ProducingTransportLoadingBonus{15.0f};
    float m_ProducingTransportMinLoadingTransportRatio{3.0f};
};

#endif // NORMALAI_H
