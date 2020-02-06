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


    explicit NormalAi(float initMinMovementDamage = 0.3f, float initMinAttackFunds = 0.0f, float initMinSuicideDamage = 3.0f / 4.0f,
                      float spamingFunds = 7500);
signals:

public slots:
    virtual void process() override;
protected:
    bool performActionSteps(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits,
                            QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief buildCOUnit
     * @param pUnits
     * @return
     */
    bool buildCOUnit(QmlVectorUnit* pUnits);
    /**
     * @brief isUsingUnit
     * @param pUnit
     * @return
     */
    bool isUsingUnit(Unit* pUnit, bool ignoreHasMoved = false);
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
     * @brief moveUnit
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @param transporterTargets
     * @return
     */
    bool moveUnit(GameAction* pAction, Unit* pUnit, QmlVectorUnit* pUnits, QStringList& actions,
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
    bool moveToUnloadArea(GameAction* pAction, Unit* pUnit, QmlVectorUnit* pUnits, QStringList& actions,
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
    bool suicide(GameAction* pAction, Unit* pUnit, UnitPathFindingSystem& turnPfs);
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

    static const float notAttackableDamage;
    static const float midDamage;
    static const float highDamage;
    static const float directIndirectRatio;

    float minMovementDamage;
    float minAttackFunds;
    float minSuicideDamage;
    float m_spamingFunds;

};

#endif // NORMALAI_H
