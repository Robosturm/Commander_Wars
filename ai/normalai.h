#ifndef NORMALAI_H
#define NORMALAI_H

#include <QObject>

#include "ai/coreai.h"

#include "game/smartUnit.h"

#include "game/unitpathfindingsystem.h"
#include "ai/decisiontree.h"

class QmlVectorUnit;
class QmlVectorBuilding;
class QmlVectorPoint;
class Building;

class NormalAi : public CoreAI
{
    Q_OBJECT

    static const float minMovementDamage;
public:
    explicit NormalAi();
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
    virtual qint32 getVersion() override
    {
        return 1;
    }
signals:

public slots:
    virtual void process() override;
protected:
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
                   qint32 minfireRange, qint32 maxfireRange);
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
                  bool shortenPathForTarget);
    /**
     * @brief loadUnits
     * @param pUnits
     * @return
     */
    bool loadUnits(QmlVectorUnit* pUnits);
    /**
     * @brief NormalAi::moveTransporters
     * @param pUnits
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool moveTransporters(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief moveToUnloadArea
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @return
     */
    bool moveToUnloadArea(GameAction* pAction, Unit* pUnit, QmlVectorUnit* pUnits, QStringList& actions,
                          QVector<QVector3D>& targets);
    /**
     * @brief repairUnits
     * @param pUnits
     * @param pBuildings
     * @return
     */
    bool repairUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings);
    /**
     * @brief getMoveTargetField shortens the movepath so we take no damage
     * @param pUnit
     * @param movePath
     * @return
     */
    qint32 getMoveTargetField(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QPoint>& movePath);
    /**
     * @brief moveToSafety
     * @param pUnit
     * @param turnPfs
     * @return
     */
    std::tuple<QPoint, float, bool> moveToSafety(Unit* pUnit, QmlVectorUnit* pUnits, UnitPathFindingSystem& turnPfs, QPoint target);
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
    bool fireWithUnits(QmlVectorUnit* pUnits, qint32 minfireRange, qint32 maxfireRange);
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
    qint32 getBestAttackTarget(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QVector4D>& ret, QVector<QVector3D>& moveTargetFields);
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
    float calculateCounterDamage(Unit* pUnit, QmlVectorUnit* pUnits, QPoint newPosition, Unit* pEnemyUnit, float enemyTakenDamage);
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
     * @brief NormalAi::calcExpectedFondsDamage
     * @param dummy
     * @param pEnemyUnits
     * @return
     */
    std::tuple<float, qint32> calcExpectedFondsDamage(qint32 posX, qint32 posY, Unit& dummy, QmlVectorUnit* pEnemyUnits, QVector<QVector4D> attackCount);
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
};

#endif // NORMALAI_H
