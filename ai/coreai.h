#ifndef COREAI_H
#define COREAI_H

#include <qtextstream.h>

#include <qvector.h>
#include <qvector3d.h>
#include <qvector4d.h>
#include <QRectF>

#include "gameinput/basegameinputif.h"

#include "ai/decisionquestion.h"

#include "qvector.h"
#include "ai/islandmap.h"

#include "ai/decisiontree.h"

class GameAction;
class Unit;
class UnitPathFindingSystem;
class QmlVectorUnit;
class QmlVectorBuilding;
class QmlVectorPoint;
class Building;

class CoreAI : public BaseGameInputIF
{
    Q_OBJECT
public:
    enum class TurnTime
    {
        startOfTurn,
        onGoingTurn,
        endOfTurn,
    };
    // static string list of actions so we only define them once
    static const QString ACTION_WAIT;
    static const QString ACTION_HOELLIUM_WAIT;
    static const QString ACTION_REPAIR;
    static const QString ACTION_RATION;
    static const QString ACTION_UNSTEALTH;
    static const QString ACTION_PLACE_WATERMINE;
    static const QString ACTION_STEALTH;
    static const QString ACTION_BUILD_UNITS;
    static const QString ACTION_CAPTURE;
    static const QString ACTION_MISSILE;
    static const QString ACTION_FIRE;
    static const QString ACTION_UNLOAD;
    static const QString ACTION_LOAD;
    static const QString ACTION_NEXT_PLAYER;
    static const QString ACTION_SWAP_COS;
    static const QString ACTION_ACTIVATE_TAGPOWER;
    static const QString ACTION_ACTIVATE_POWER_CO_0;
    static const QString ACTION_ACTIVATE_POWER_CO_1;
    static const QString ACTION_ACTIVATE_SUPERPOWER_CO_0;
    static const QString ACTION_ACTIVATE_SUPERPOWER_CO_1;
    static const QString ACTION_CO_UNIT_0;
    static const QString ACTION_CO_UNIT_1;
    static const QString ACTION_EXPLODE;

    explicit CoreAI(BaseGameInputIF::AiTypes aiType);
    virtual ~CoreAI();
    /**
     * @brief init
     */
    virtual void init() override;
    /**
     * @brief contains
     * @param points
     * @param point
     * @return
     */
    static bool contains(QVector<QVector3D>& points, QPoint point);
    /**
     * @brief index
     * @param points
     * @param point
     * @return
     */
    static qint32 index(QVector<QVector3D>& points, QPoint point);
signals:
    /**
     * @brief performAction signal with an action to be performed the action has to be deleted by the reciever of this slot. Only one slot can be connected to this signal
     * @param pAction
     */
    void performAction(GameAction* pAction);
public slots:
    /**
     * @brief process
     */
    virtual void process() = 0;
    /**
     * @brief useCOPower
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    bool useCOPower(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits);
    /**
     * @brief useBuilding
     * @param pBuildings
     * @return
     */
    bool useBuilding(QmlVectorBuilding* pBuildings);
    /**
     * @brief moveOoziums moves all those sweet nice ooziums :)
     * @param pUnits
     * @param pEnemyUnits
     */
    bool moveOoziums(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits);
    /**
     * @brief moveBlackBombs
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    bool moveBlackBombs(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits);
    /**
     * @brief moveRepair
     * @param pUnits
     * @return
     */
    bool moveRepair(QmlVectorUnit* pUnits);

    virtual void nextAction();
    /**
     * @brief calcUnitDamage
     * @param pUnit
     * @param position
     * @param target
     * @return x = unit damage and y = counter damage
     */
    QRectF calcUnitDamage(GameAction* pAction, QPoint target);
    /**
     * @brief CoreAI::calcVirtuelUnitDamage
     * @param pAttacker
     * @param attackerTakenDamage
     * @param atkPos
     * @param defX
     * @param defY
     * @param defenderTakenDamage
     * @return
     */
    QRectF calcVirtuelUnitDamage(Unit* pAttacker, float attackerTakenDamage, QPoint atkPos,
                                 Unit* pDefender, float defenderTakenDamage, QPoint defPos);
    /**
     * @brief getBestTarget
     * @param pUnit
     * @param pAction
     * @param pPfs
     * @return target unit x, y and z = fonddamage
     */
    void getBestTarget(Unit* pUnit, GameAction* pAction, UnitPathFindingSystem* pPfs, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getAttacksFromField
     * @param pUnit
     * @param pAction
     * @param ret
     */
    void getBestAttacksFromField(Unit* pUnit, GameAction* pAction, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getAttackTargets
     * @param pUnit
     * @param pAction
     * @param pPfs
     * @param ret
     * @param moveTargetFields
     */
    void getAttackTargets(Unit* pUnit, GameAction* pAction, UnitPathFindingSystem* pPfs, QVector<QVector4D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getAttacksFromField
     * @param pUnit
     * @param pAction
     * @param ret
     * @param moveTargetFields
     */
    void getAttacksFromField(Unit* pUnit, GameAction* pAction, QVector<QVector4D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief moveAwayFromProduction
     * @param pUnits
     * @return
     */
    bool moveAwayFromProduction(QmlVectorUnit* pUnits);
    /**
     * @brief CoreAI::calcFundsDamage
     * @param damage
     * @param pAtk
     * @param pDef
     * @return
     */
    QPointF calcFundsDamage(QRectF damage, Unit* pAtk, Unit* pDef);
    /**
     * @brief appendAttackTargets
     * @param pUnit
     * @param pEnemyUnits
     * @param targets
     */
    void appendAttackTargets(Unit* pUnit, QmlVectorUnit* pEnemyUnits, QVector<QVector3D>& targets);
    /**
     * @brief getTrainingData reads the training data from a training file for a pipeline either decision tree or neural network
     * @param file
     * @param trainingData
     * @param questions
     */
    static void getTrainingData(QString file, QVector<QVector<float>>& trainingData, QVector<QVector<spDecisionQuestion>>& questions);
    /**
     * @brief readTrainingFile
     * @param stream
     * @param questionsFound
     * @param types
     * @param readQuestions
     * @param trainingData
     * @param questions
     */
    static void readTrainingFile(QTextStream& stream, bool& questionsFound, QStringList& types,
                                 QVector<spDecisionQuestion>& readQuestions,
                                 QVector<QVector<float>>& trainingData, QVector<QVector<spDecisionQuestion>>& questions);
protected:
    void addMenuItemData(GameAction* pGameAction, QString itemID, qint32 cost);
    void addSelectedFieldData(GameAction* pGameAction, QPoint point);

    /**
     * @brief processPredefinedAi
     * @return
     */
    bool processPredefinedAi();
    void processPredefinedAiHold(Unit* pUnit);
    void processPredefinedAiDefensive(Unit* pUnit);
    void processPredefinedAiOffensive(Unit* pUnit, QmlVectorUnit* pEnemyUnits);
    virtual void finishTurn();
    // helper functions to get targets for unit actions
    void appendCaptureTargets(QStringList actions, Unit* pUnit, QmlVectorBuilding* pEnemyBuildings, QVector<QVector3D>& targets);
    void appendAttackTargetsIgnoreOwnUnits(Unit* pUnit, QmlVectorUnit* pEnemyUnits, QVector<QVector3D>& targets);
    void appendRepairTargets(Unit* pUnit, QmlVectorBuilding* pBuildings, QVector<QVector3D>& targets);
    void appendSupplyTargets(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QVector3D>& targets);
    void appendTransporterTargets(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QVector3D>& targets);
    void appendCaptureTransporterTargets(Unit* pUnit, QmlVectorUnit* pUnits,
                                         QmlVectorBuilding* pEnemyBuildings, QVector<QVector3D>& targets);
    QVector<Unit*> appendLoadingTargets(Unit* pUnit, QmlVectorUnit* pUnits,
                                        QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                                        bool ignoreCaptureTargets, bool virtualLoading, QVector<QVector3D>& targets);
    /**
     * @brief hasTargets checks if a unit has anything to do on this island
     * @param pLoadingUnit
     * @param canCapture
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool hasTargets(Unit* pLoadingUnit, bool canCapture, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief appendNearestUnloadTargets searches for unload fields closest to our current position
     * @param pUnit
     * @param pEnemyUnits
     * @param pEnemyBuildings
     */
    void appendNearestUnloadTargets(Unit* pUnit, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings, QVector<QVector3D>& targets);
    /**
     * @brief appendUnloadTargetsForCapturing searches unload fields near enemy buildings
     * @param pUnit
     * @param pEnemyBuildings
     */
    void appendUnloadTargetsForCapturing(Unit* pUnit, QmlVectorBuilding* pEnemyBuildings, QVector<QVector3D>& targets);

    void checkIslandForUnloading(Unit* pLoadedUnit, QVector<qint32>& checkedIslands,
                                 qint32 unitIslandIdx, qint32 unitIsland,
                                 qint32 loadedUnitIslandIdx, qint32 targetIsland,
                                 QmlVectorPoint* pUnloadArea, QVector<QVector3D>& targets);
    /**
     * @brief createIslandMap
     * @param pUnits
     */
    void rebuildIsland(QmlVectorUnit* pUnits);
    /**
     * @brief onSameIsland checks if unit1 can reach unit 2. This may be vice versa but isn't checked here
     * @param pUnit1
     * @param pUnit2
     * @return
     */
    bool onSameIsland(Unit* pUnit1, Unit* pUnit2);
    /**
     * @brief onSameIsland checks if unit1 can reach the building. This may be vice versa but isn't checked here
     * @param pUnit1
     * @param pBuilding
     * @return
     */
    bool onSameIsland(Unit* pUnit1, Building* pBuilding);
    /**
     * @brief onSameIsland
     * @param movemnetType
     * @param x
     * @param y
     * @param x1
     * @param y1
     * @return
     */
    bool onSameIsland(QString movemnetType, qint32 x, qint32 y, qint32 x1, qint32 y1);
    /**
     * @brief getIsland
     * @param pUnit1
     * @return
     */
    qint32 getIsland(Unit* pUnit);
    /**
     * @brief getIslandIndex
     * @param pUnit1
     * @return
     */
    qint32 getIslandIndex(Unit* pUnit);
    /**
     * @brief createIslandMap
     * @param movementType
     * @param unitID
     */
    void createIslandMap(QString movementType, QString unitID);


protected:
    DecisionTree m_COPowerTree;
    QVector<spIslandMap> m_IslandMaps;
    float buildingValue{1.0f};
    float ownUnitValue{1.0f};
    TurnTime turnMode{TurnTime::startOfTurn};
private:
    bool finish{false};
};

#endif // COREAI_H
