#ifndef COREAI_H
#define COREAI_H

#include <qtextstream.h>
#include <qvector.h>
#include <qvector3d.h>
#include <qvector4d.h>
#include <QRectF>

#include "gameinput/basegameinputif.h"

#include "ai/decisionquestion.h"
#include "ai/islandmap.h"
#include "ai/decisiontree.h"

#include "coreengine/qmlvector.h"
#include "coreengine/LUPDATE_MACROS.h"

class GameAction;
typedef oxygine::intrusive_ptr<GameAction> spGameAction;
class Unit;
class CO;
class UnitPathFindingSystem;
class QmlVectorUnit;
class QmlVectorBuilding;
class QmlVectorPoint;
class Building;
class Terrain;

class CoreAI : public BaseGameInputIF
{
    Q_OBJECT
public:
    /**
     * @brief The AISteps enum
     */
    ENUM_CLASS AISteps
    {
        moveUnits = 0,
                moveToTargets,
                loadUnits,
                moveTransporters,
                moveSupportUnits,
                moveAway,
                buildUnits,
    };
    // static string list of actions so we only define them once
    static const QString ACTION_WAIT;
    static const QString ACTION_HOELLIUM_WAIT;
    static const QString ACTION_SUPPORTSINGLE;
    static const QString ACTION_SUPPORTSINGLE_REPAIR;
    static const QString ACTION_SUPPORTSINGLE_FREEREPAIR;
    static const QString ACTION_SUPPORTALL;
    static const QString ACTION_SUPPORTALL_RATION;
    static const QString ACTION_UNSTEALTH;
    static const QString ACTION_PLACE;
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
    static const QString ACTION_FLARE;

    explicit CoreAI(GameEnums::AiTypes aiType);
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
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 5;
    }
signals:
    /**
     * @brief performAction signal with an action to be performed the action has to be deleted by the reciever of this slot. Only one slot can be connected to this signal
     * @param pAction
     */
    void performAction(spGameAction pAction);
public slots:
    /**
     * @brief process
     */
    virtual void process() = 0;
    /**
     * @brief loadIni
     * @param file
     */
    void loadIni(QString file);
    /**
     * @brief readIni
     * @param name
     */
    virtual void readIni(QString name) = 0;
    /**
     * @brief useCOPower
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    bool useCOPower(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits);
    /**
     * @brief calcBuildingDamage
     * @param pUnit
     * @param pBuilding
     * @return
     */
    float calcBuildingDamage(Unit* pUnit, QPoint newPosition, Building* pBuilding);
    /**
     * @brief createMovementMap
     */
    void createMovementMap(QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief addMovementMap
     * @param pBuilding
     * @param damage
     */
    void addMovementMap(Building* pBuilding, float damage);
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
     * @brief moveFlares
     * @param pUnits
     * @return
     */
    bool moveFlares(QmlVectorUnit* pUnits);
    /**
     * @brief moveBlackBombs
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    bool moveBlackBombs(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits);
    /**
     * @brief moveSupport
     * @param pUnits
     * @return
     */
    bool moveSupport(AISteps step, QmlVectorUnit* pUnits, bool useTransporters);
    /**
     * @brief nextAction
     */
    virtual void nextAction();
    /**
     * @brief calcUnitDamage
     * @param pUnit
     * @param position
     * @param target
     * @return x = unit damage and y = counter damage
     */
    QRectF calcUnitDamage(spGameAction pAction, QPoint target);
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
                                 Unit* pDefender, float defenderTakenDamage, QPoint defPos,
                                 bool ignoreOutOfVisionRange = false);
    /**
     * @brief getBestTarget
     * @param pUnit
     * @param pAction
     * @param pPfs
     * @return target unit x, y and z = fonddamage
     */
    void getBestTarget(Unit* pUnit, spGameAction pAction, UnitPathFindingSystem* pPfs, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getAttacksFromField
     * @param pUnit
     * @param pAction
     * @param ret
     */
    void getBestAttacksFromField(Unit* pUnit, spGameAction pAction, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getAttackTargets
     * @param pUnit
     * @param pAction
     * @param pPfs
     * @param ret
     * @param moveTargetFields
     */
    void getAttackTargets(Unit* pUnit, spGameAction pAction, UnitPathFindingSystem* pPfs, QVector<QVector4D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getAttacksFromField
     * @param pUnit
     * @param pAction
     * @param ret
     * @param moveTargetFields
     */
    void getAttacksFromField(Unit* pUnit, spGameAction pAction, QVector<QVector4D>& ret, QVector<QVector3D>& moveTargetFields);
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
    void addMenuItemData(spGameAction pGameAction, QString itemID, qint32 cost);
    void addSelectedFieldData(spGameAction pGameAction, QPoint point);
    /**
     * @brief isAttackOnTerrainAllowed
     * @param pTerrain
     * @return
     */
    bool isAttackOnTerrainAllowed(Terrain* pTerrain);
    /**
     * @brief processPredefinedAi
     * @return
     */
    bool processPredefinedAi();
    void processPredefinedAiHold(Unit* pUnit);
    void processPredefinedAiDefensive(Unit* pUnit);
    void processPredefinedAiOffensive(Unit* pUnit, QmlVectorUnit* pEnemyUnits);
    void processPredefinedAiPatrol(Unit* pUnit);
    bool processPredefinedAiAttack(Unit* pUnit, spGameAction pAction, UnitPathFindingSystem & pfs);
    virtual void finishTurn();
    // helper functions to get targets for unit actions
    void appendSupportTargets(QStringList actions, Unit* pCurrentUnit, QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QVector<QVector3D>& targets);
    void appendCaptureTargets(QStringList actions, Unit* pUnit, QmlVectorBuilding* pEnemyBuildings,  QVector<QVector3D>& targets);
    void appendAttackTargetsIgnoreOwnUnits(Unit* pUnit, QmlVectorUnit* pEnemyUnits, QVector<QVector3D>& targets);
    void appendRepairTargets(Unit* pUnit, QmlVectorBuilding* pBuildings, QVector<QVector3D>& targets);
    void appendSupplyTargets(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QVector3D>& targets);
    void appendTransporterTargets(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QVector3D>& targets);
    void appendCaptureTransporterTargets(Unit* pUnit, QmlVectorUnit* pUnits,
                                         QmlVectorBuilding* pEnemyBuildings, QVector<QVector3D>& targets);
    QVector<Unit*> appendLoadingTargets(Unit* pUnit, QmlVectorUnit* pUnits,
                                        QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                                        bool ignoreCaptureTargets, bool virtualLoading, QVector<QVector3D>& targets,
                                        bool all = false);
    /**
     * @brief hasTargets checks if a unit has anything to do on this island
     * @param pLoadingUnit
     * @param canCapture
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool hasTargets(Unit* pLoadingUnit, bool canCapture, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                    qint32 loadingIslandIdx, qint32 loadingIsland);
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
    /**
     * @brief appendTerrainBuildingAttackTargets
     * @param pUnit
     * @param pEnemyBuildings
     * @param targets
     */
    void appendTerrainBuildingAttackTargets(Unit* pUnit, QmlVectorBuilding* pEnemyBuildings, QVector<QVector3D>& targets);
    /**
     * @brief checkIslandForUnloading
     * @param pUnit
     * @param pLoadedUnit
     * @param checkedIslands
     * @param unitIslandIdx
     * @param unitIsland
     * @param loadedUnitIslandIdx
     * @param targetIsland
     * @param pUnloadArea
     * @param targets
     */
    void checkIslandForUnloading(Unit* pUnit, Unit* pLoadedUnit, QVector<qint32>& checkedIslands,
                                 qint32 unitIslandIdx, qint32 unitIsland,
                                 qint32 loadedUnitIslandIdx, qint32 targetIsland,
                                 QmlVectorPoint* pUnloadArea, QVector<QVector3D>& targets);
    /**
     * @brief getBestFlareTarget
     * @param pUnit
     * @param pAction
     * @param pPfs
     * @param flareTarget
     * @param moveTargetField
     */
    void getBestFlareTarget(Unit* pUnit, spGameAction pAction, UnitPathFindingSystem* pPfs, QPoint& flareTarget, QPoint& moveTargetField);
    /**
     * @brief getFlareTargetScore
     * @param moveTarget
     * @param flareTarget
     * @param pUnfogCircle
     * @return
     */
    qint32 getFlareTargetScore(const QPoint& moveTarget, const QPoint& flareTarget, const spQmlVectorPoint& pUnfogCircle);
    /**
     * @brief isUnloadTerrain
     * @param pUnit
     * @param pTerrain
     * @return
     */
    bool isUnloadTerrain(Unit* pUnit, Terrain* pTerrain);
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
     * @brief onSameIsland
     * @param islandIdx
     * @param x
     * @param y
     * @param x1
     * @param y1
     * @return
     */
    bool onSameIsland(qint32 islandIdx, qint32 x, qint32 y, qint32 x1, qint32 y1);
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
    /**
     * @brief needsRefuel
     * @param pUnit
     * @return
     */
    bool needsRefuel(Unit* pUnit);
    /**
     * @brief hasMissileTarget
     * @return
     */
    bool hasMissileTarget()
    {
        return m_missileTarget;
    };
    /**
     * @brief getAiCoUnitMultiplier
     * @param pCO
     * @param pUnit
     * @return
     */
    float getAiCoUnitMultiplier(CO* pCO, Unit* pUnit);
    /**
     * @brief GetUnitCounts
     * @param pUnits
     * @param infantryUnits
     * @param indirectUnits
     * @param directUnits
     * @param transportTargets
     */
    void GetOwnUnitCounts(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                          qint32 & infantryUnits, qint32 & indirectUnits,
                          qint32 & directUnits, QVector<std::tuple<Unit*, Unit*>> & transportTargets);
    /**
     * @brief buildCOUnit
     * @return
     */
    bool buildCOUnit(QmlVectorUnit* pUnits);
protected:
    DecisionTree m_COPowerTree;
    QVector<spIslandMap> m_IslandMaps;
    float m_buildingValue{1.0f};
    float m_ownUnitValue{1.0f};
    GameEnums::AiTurnMode turnMode{GameEnums::AiTurnMode_StartOfDay};
    AISteps aiStep;
    bool usedTransportSystem{false};
    bool m_missileTarget{false};
    float m_fuelResupply{0.33f};
    float m_ammoResupply{0.25f};

    float m_minCoUnitScore{5000.0f};
    qint32 m_coUnitValue{1000};
    float m_coUnitRankReduction{1000.0f};
    float m_coUnitScoreMultiplier{1.1f};
    qint32 m_minCoUnitCount{5};
private:
    bool finish{false};
    struct FlareInfo
    {
        qint32 minRange{0};
        qint32 maxRange{0};
        qint32 unfogRange{0};
    };
    FlareInfo m_flareInfo;
    QStringList m_files;
};

#endif // COREAI_H
