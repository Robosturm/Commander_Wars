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

class NormalAi : public CoreAI
{
    Q_OBJECT
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

    bool performActionSteps(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits,
                            QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
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
    bool moveUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings,
                   QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                   qint32 minfireRange, qint32 maxfireRange, bool supportUnits = false);
    /**
     * @brief refillUnits
     * @param pUnits
     * @param pBuildings
     * @param pEnemyBuildings
     * @return
     */
    bool refillUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
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
    void appendRefillTargets(QStringList actions, Unit* pUnit, QmlVectorUnit* pUnits, QVector<QVector3D>& targets);
    /**
     * @brief moveUnit
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @param transporterTargets
     * @return
     */
    bool moveUnit(spGameAction pAction, Unit* pUnit, QmlVectorUnit* pUnits, QStringList& actions,
                  QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets,
                  bool shortenPathForTarget,
                  QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief loadUnits
     * @param pUnits
     * @return
     */
    bool loadUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief NormalAi::moveTransporters
     * @param pUnits
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool moveTransporters(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief moveToUnloadArea
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @return
     */
    bool moveToUnloadArea(spGameAction pAction, Unit* pUnit, QmlVectorUnit* pUnits, QStringList& actions,
                          QVector<QVector3D>& targets,
                          QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief repairUnits
     * @param pUnits
     * @param pBuildings
     * @return
     */
    bool repairUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief getMoveTargetField shortens the movepath so we take no damage
     * @param pUnit
     * @param movePath
     * @return
     */
    qint32 getMoveTargetField(Unit* pUnit, QmlVectorUnit* pUnits, UnitPathFindingSystem& turnPfs,
                              QVector<QPoint>& movePath, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief moveToSafety
     * @param pUnit
     * @param turnPfs
     * @return
     */
    std::tuple<QPoint, float, bool> moveToSafety(Unit* pUnit, QmlVectorUnit* pUnits,
                                                 UnitPathFindingSystem& turnPfs, QPoint target,
                                                 QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief captureBuildings
     * @param pUnits
     * @return
     */
    bool captureBuildings(QmlVectorUnit* pUnits);
    /**
     * @brief fireWithIndirectUnits
     * @param pUnits
     * @return
     */
    bool fireWithUnits(QmlVectorUnit* pUnits, qint32 minfireRange, qint32 maxfireRange,
                       QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
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
    qint32 getBestAttackTarget(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QVector4D>& ret,
                               QVector<QVector3D>& moveTargetFields,
                               QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief updateEnemyData
     */
    void updateEnemyData(QmlVectorUnit* pUnits);
    /**
     * @brief calcVirtualDamage
     */
    void calcVirtualDamage(QmlVectorUnit* pUnits);
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
    float calculateCounterDamage(Unit* pUnit, QmlVectorUnit* pUnits, QPoint newPosition,
                                 Unit* pEnemyUnit, float enemyTakenDamage,
                                 QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings,
                                 bool ignoreOutOfVisionRange = false);
    /**
     * @brief calculateCounteBuildingDamage
     * @param pUnit
     * @param pBuildings
     * @param pEnemyBuildings
     * @return
     */
    float calculateCounteBuildingDamage(Unit* pUnit, QPoint newPosition, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
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
    bool buildUnits(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits,
                    QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief calcBuildScore
     * @param data
     * @return
     */
    float calcBuildScore(QVector<float>& data);
    /**
     * @brief calcCostScore
     * @param data
     * @return
     */
    float calcCostScore(QVector<float>& data);
    /**
     * @brief calcTransporterScore
     * @param posX
     * @param posY
     * @param dummy
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    float calcTransporterScore(Unit& dummy, QmlVectorUnit* pUnits,
                               QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                               QVector<std::tuple<Unit*, Unit*>>& transportTargets,
                               QVector<float>& data);
    /**
     * @brief canTransportToEnemy
     * @param pUnit
     * @param pLoadedUnit
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool canTransportToEnemy(Unit* pUnit, Unit* pLoadedUnit, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief NormalAi::calcExpectedFundsDamage
     * @param dummy
     * @param pEnemyUnits
     * @return
     */
    std::tuple<float, qint32> calcExpectedFundsDamage(qint32 posX, qint32 posY, Unit& dummy, QmlVectorUnit* pEnemyUnits, QVector<QVector4D> attackCount, float bonusFactor);
    /**
     * @brief getClosestTargetDistance
     * @param posX
     * @param posY
     * @param dummy
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    qint32 getClosestTargetDistance(qint32 posX, qint32 posY, Unit& dummy, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings);
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
    QVector<QPoint> updatePoints;
    /**
     * @brief m_VirtualEnemyData
     */
    QVector<QPointF> m_VirtualEnemyData;
    /**
     * @brief m_TransporterScores
     */
    QVector<std::tuple<float, QString, qint32, qint32>> m_TransporterScores;

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
};

#endif // NORMALAI_H
