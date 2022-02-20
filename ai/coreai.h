#pragma once
#include <qtextstream.h>
#include <qvector.h>
#include <qvector3d.h>
#include <qvector4d.h>
#include <QRectF>

#include "gameinput/basegameinputif.h"
#include "game/gameaction.h"

#include "ai/islandmap.h"

#include "ai/decisiontree/decisiontree.h"

#include "coreengine/qmlvector.h"
#include "coreengine/LUPDATE_MACROS.h"

class GameMap;
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
    struct UnitCountData
    {
        qint32 infantryUnits{0};
        qint32 indirectUnits{0};
        qint32 directUnits{0};
        qint32 transporterUnits{0};
        qint32 supplyUnits{0};
        qint32 supplyNeededUnits{0};
        QVector<std::tuple<Unit*, Unit*>> transportTargets;
    };

    struct IniData
    {
        IniData(QString name, QString group, double * value, double defaultValue, double minValue, double maxValue)
            : m_name(name),
              m_group(group),
              m_value(value),
              m_defaultValue(defaultValue),
              m_minValue(minValue),
              m_maxValue(maxValue)
        {
        }
        QString m_name;
        QString m_group;
        double * m_value;
        double m_defaultValue;
        double m_minValue;
        double m_maxValue;
    };

    struct DamageData
    {
        qint32 x{-1};
        qint32 y{-1};
        float fundsDamage{0};
        float hpDamage{0};
        float counterDamage{0.0f};
    };

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
    Q_ENUM(AISteps)
    // static string list of actions so we only define them once
    static const QString ACTION_WAIT;
    static const QString ACTION_HOELLIUM_WAIT;
    static const QString ACTION_SUPPORTSINGLE;
    static const QString ACTION_SUPPORTSINGLE_REPAIR;
    static const QString ACTION_SUPPORTSINGLE_FREEREPAIR;
    static const QString ACTION_SUPPORTSINGLE_SUPPLY;
    static const QString ACTION_SUPPORTALL;
    static const QString ACTION_SUPPORTALL_RATION;
    static const QString ACTION_SUPPORTALL_RATION_MONEY;
    static const QString ACTION_UNSTEALTH;
    static const QString ACTION_PLACE;
    static const QString ACTION_STEALTH;
    static const QString ACTION_BUILD_UNITS;
    static const QString ACTION_CAPTURE;
    static const QString ACTION_MISSILE;
    static const QString ACTION_FIRE;
    static const QString ACTION_JOIN;
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
    static const QString ACTION_TRAP;

    static const QString UNIT_INFANTRY;

    explicit CoreAI(GameMap* pMap, GameEnums::AiTypes aiType);
    virtual ~CoreAI() = default;
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
    static bool contains(QVector<QVector3D>& points, const QPoint & point);
    /**
     * @brief index
     * @param points
     * @param point
     * @return
     */
    static qint32 index(QVector<QVector3D>& points, const QPoint & point);
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
        return 8;
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
    virtual void readIni(QString name);
    /**
     * @brief saveIni
     * @param name
     */
    void saveIni(QString name) const;
    /**
     * @brief randomizeIni
     */
    void randomizeIni(QString name, float chance, float mutationRate = 0.1f);
    /**
     * @brief setInitValue changes an ini-file loaded parameter to the given value if the name doesn't exist nothing happens
     * @param name
     * @param newValue
     */
    void setInitValue(QString name, double newValue);
    /**
     * @brief getInitValue gets the current value of an ini-file loaded parameter if the name doesn't exist returns 0.
     * @param name
     * @return
     */
    double getInitValue(QString name) const;
    /**
     * @brief useCOPower
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    bool useCOPower(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits);
    /**
     * @brief calcBuildingDamage
     * @param pUnit
     * @param pBuilding
     * @return
     */
    float calcBuildingDamage(Unit* pUnit, const QPoint & newPosition, Building* pBuilding) const;
    /**
     * @brief createMovementMap
     */
    void createMovementMap(spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings);
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
    bool useBuilding(spQmlVectorBuilding & pBuildings);
    /**
     * @brief moveOoziums moves all those sweet nice ooziums :)
     * @param pUnits
     * @param pEnemyUnits
     */
    bool moveOoziums(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits);
    /**
     * @brief moveFlares
     * @param pUnits
     * @return
     */
    bool moveFlares(spQmlVectorUnit & pUnits);
    /**
     * @brief moveBlackBombs
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    bool moveBlackBombs(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits);
    /**
     * @brief moveSupport
     * @param pUnits
     * @return
     */
    bool moveSupport(CoreAI::AISteps step, spQmlVectorUnit & pUnits, bool useTransporters);
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
    QRectF calcUnitDamage(spGameAction & pAction, const QPoint & target) const;
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
    static QRectF calcVirtuelUnitDamage(GameMap* pMap,
                                        Unit* pAttacker, float attackerTakenDamage, const QPoint & atkPos, GameEnums::LuckDamageMode luckModeAtk,
                                        Unit* pDefender, float defenderTakenDamage, const QPoint & defPos, GameEnums::LuckDamageMode luckModeDef,
                                        bool ignoreOutOfVisionRange = false);
    /**
     * @brief getBestTarget
     * @param pUnit
     * @param pAction
     * @param pPfs
     * @return target unit x, y and z = fonddamage
     */
    void getBestTarget(Unit* pUnit, spGameAction & pAction, UnitPathFindingSystem* pPfs, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getAttacksFromField
     * @param pUnit
     * @param pAction
     * @param ret
     */
    void getBestAttacksFromField(Unit* pUnit, spGameAction & pAction, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields);
    /**
     * @brief getAttackTargets
     * @param pUnit
     * @param pAction
     * @param pPfs
     * @param ret
     * @param moveTargetFields
     */
    void getAttackTargets(Unit* pUnit, spGameAction & pAction, UnitPathFindingSystem* pPfs, QVector<CoreAI::DamageData>& ret, QVector<QVector3D>& moveTargetFields) const;
    /**
     * @brief getAttacksFromField
     * @param pUnit
     * @param pAction
     * @param ret
     * @param moveTargetFields
     */
    void getAttacksFromField(Unit* pUnit, spGameAction & pAction, QVector<DamageData>& ret, QVector<QVector3D>& moveTargetFields) const;
    /**
     * @brief moveAwayFromProduction
     * @param pUnits
     * @return
     */
    bool moveAwayFromProduction(spQmlVectorUnit & pUnits);
    /**
     * @brief CoreAI::calcFundsDamage
     * @param damage
     * @param pAtk
     * @param pDef
     * @return
     */
    QPointF calcFundsDamage(const QRectF & damage, Unit* pAtk, Unit* pDef) const;
    /**
     * @brief appendAttackTargets
     * @param pUnit
     * @param pEnemyUnits
     * @param targets
     */
    void appendAttackTargets(Unit* pUnit, spQmlVectorUnit & pEnemyUnits, QVector<QVector3D>& targets);

protected:
    void addMenuItemData(spGameAction & pGameAction, const QString & itemID, qint32 cost);
    void addSelectedFieldData(spGameAction & pGameAction, const QPoint & point);
    /**
     * @brief isAttackOnTerrainAllowed
     * @param pTerrain
     * @return
     */
    bool isAttackOnTerrainAllowed(Terrain* pTerrain, float damage) const;
    /**
     * @brief processPredefinedAi
     * @return
     */
    bool processPredefinedAi();
    void processPredefinedAiHold(Unit* pUnit);
    void processPredefinedAiDefensive(Unit* pUnit);
    void processPredefinedAiOffensive(Unit* pUnit, spQmlVectorUnit & pEnemyUnits);
    void processPredefinedAiPatrol(Unit* pUnit);
    bool processPredefinedAiAttack(Unit* pUnit, spGameAction & pAction, UnitPathFindingSystem & pfs);
    virtual void finishTurn();
    // helper functions to get targets for unit actions
    void appendSupportTargets(const QStringList & actions, Unit* pCurrentUnit, spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, QVector<QVector3D>& targets);
    void appendCaptureTargets(const QStringList & actions, Unit* pUnit, spQmlVectorBuilding & pEnemyBuildings,  QVector<QVector3D>& targets);
    void appendAttackTargetsIgnoreOwnUnits(Unit* pUnit, spQmlVectorUnit & pEnemyUnits, QVector<QVector3D>& targets);
    void appendRepairTargets(Unit* pUnit, spQmlVectorBuilding & pBuildings, QVector<QVector3D>& targets);
    void appendSupplyTargets(Unit* pUnit, spQmlVectorUnit & pUnits, QVector<QVector3D>& targets);
    void appendTransporterTargets(Unit* pUnit, spQmlVectorUnit & pUnits, QVector<QVector3D>& targets);
    void appendCaptureTransporterTargets(Unit* pUnit, spQmlVectorUnit & pUnits,
                                         spQmlVectorBuilding & pEnemyBuildings, QVector<QVector3D>& targets);
    QVector<Unit*> appendLoadingTargets(Unit* pUnit, spQmlVectorUnit & pUnits,
                                        spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                        bool addCaptureTargets, bool virtualLoading, QVector<QVector3D>& targets,
                                        bool all = false);
    /**
     * @brief hasTargets checks if a unit has anything to do on this island
     * @param transporterMovement movement points of the transporting unit
     * @param pLoadingUnit
     * @param canCapture
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool hasTargets(qint32 transporterMovement, Unit* pLoadingUnit, bool canCapture, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                    qint32 loadingIslandIdx, qint32 loadingIsland);
    /**
     * @brief hasCaptureTarget
     * @param pLoadingUnit
     * @param canCapture
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @param loadingIslandIdx
     * @param loadingIsland
     * @return
     */
    bool hasCaptureTarget(Unit* pLoadingUnit, bool canCapture, spQmlVectorBuilding & pEnemyBuildings,
                                  qint32 loadingIslandIdx, qint32 loadingIsland);
    /**
     * @brief appendNearestUnloadTargets searches for unload fields closest to our current position
     * @param pUnit
     * @param pEnemyUnits
     * @param pEnemyBuildings
     */
    void appendNearestUnloadTargets(Unit* pUnit, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings, QVector<QVector3D>& targets);
    /**
     * @brief appendUnloadTargetsForCapturing searches unload fields near enemy buildings
     * @param pUnit
     * @param pEnemyBuildings
     */
    void appendUnloadTargetsForCapturing(Unit* pUnit, spQmlVectorBuilding & pEnemyBuildings, QVector<QVector3D>& targets);
    /**
     * @brief appendTerrainBuildingAttackTargets
     * @param pUnit
     * @param pEnemyBuildings
     * @param targets
     */
    void appendTerrainBuildingAttackTargets(Unit* pUnit, spQmlVectorBuilding & pEnemyBuildings, QVector<QVector3D>& targets);
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
    void getBestFlareTarget(Unit* pUnit, spGameAction & pAction, UnitPathFindingSystem* pPfs, QPoint& flareTarget, QPoint& moveTargetField);
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
    void rebuildIsland(spQmlVectorUnit & pUnits);
    /**
     * @brief onSameIsland checks if unit1 can reach unit 2. This may be vice versa but isn't checked here
     * @param pUnit1
     * @param pUnit2
     * @return
     */
    bool onSameIsland(Unit* pUnit1, Unit* pUnit2) const;
    /**
     * @brief onSameIsland checks if unit1 can reach the building. This may be vice versa but isn't checked here
     * @param pUnit1
     * @param pBuilding
     * @return
     */
    bool onSameIsland(Unit* pUnit1, Building* pBuilding) const;
    /**
     * @brief onSameIsland
     * @param movemnetType
     * @param x
     * @param y
     * @param x1
     * @param y1
     * @return
     */
    bool onSameIsland(const QString & movemnetType, qint32 x, qint32 y, qint32 x1, qint32 y1) const;
    /**
     * @brief onSameIsland
     * @param islandIdx
     * @param x
     * @param y
     * @param x1
     * @param y1
     * @return
     */
    bool onSameIsland(qint32 islandIdx, qint32 x, qint32 y, qint32 x1, qint32 y1) const;
    /**
     * @brief getIsland
     * @param pUnit1
     * @return
     */
    qint32 getIsland(Unit* pUnit) const;
    /**
     * @brief getIslandIndex
     * @param pUnit1
     * @return
     */
    qint32 getIslandIndex(Unit* pUnit) const;
    /**
     * @brief createIslandMap
     * @param movementType
     * @param unitID
     */
    void createIslandMap(const QString & movementType, const QString & unitID);
    /**
     * @brief needsRefuel
     * @param pUnit
     * @return
     */
    bool needsRefuel(Unit* pUnit) const;
    /**
     * @brief isRefuelUnit
     * @param pUnit
     * @return
     */
    bool isRefuelUnit(Unit* pUnit) const;
    /**
     * @brief isRefuelUnit
     * @param actionList
     * @return
     */
    bool isRefuelUnit(const QStringList & actionList) const;
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
     * @brief getAiCoBuildRatioModifier
     * @param pCO
     * @return
     */
    float getAiCoBuildRatioModifier();
    /**
     * @brief GetUnitCounts
     * @param pUnits
     * @param infantryUnits
     * @param indirectUnits
     * @param directUnits
     * @param transportTargets
     */
    void GetOwnUnitCounts(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                          UnitCountData & countData);
    /**
     * @brief buildCOUnit
     * @return
     */
    bool buildCOUnit(spQmlVectorUnit & pUnits);
    /**
     * @brief canTransportToEnemy
     * @param pUnit
     * @param pLoadedUnit
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool canTransportToEnemy(Unit* pUnit, Unit* pLoadedUnit, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief isMoveableBuilding
     * @param pBuilding
     * @return
     */
    bool isMoveableTile(Building* pBuilding) const;
    /**
     * @brief deserializeObjectVersion
     * @param stream
     * @param version
     */
    void deserializeObjectVersion(QDataStream &stream, qint32 version);
protected:
    QVector<spIslandMap> m_IslandMaps;
    double m_buildingValue{1.0f};
    double m_ownUnitValue{1.0f};
    GameEnums::AiTurnMode turnMode{GameEnums::AiTurnMode_StartOfDay};
    AISteps m_aiStep;
    bool m_usedTransportSystem{false};
    bool m_missileTarget{false};
    double m_fuelResupply{0.33f};
    double m_ammoResupply{0.25f};

    double m_minCoUnitScore{5000.0f};
    double m_coUnitValue{6000};
    double m_coUnitRankReduction{1000.0f};
    double m_coUnitScoreMultiplier{1.1f};
    double m_minCoUnitCount{5};
    double m_minSameIslandDistance{2.5};
    double m_slowUnitSpeed{2};
    double m_minTerrainDamage{20.0f};

    QVector<IniData> m_iniData;
private:
    bool finish{false};
    struct FlareInfo
    {
        qint32 minRange{0};
        qint32 maxRange{0};
        qint32 unfogRange{0};
    };
    FlareInfo m_flareInfo;
    QStringList m_iniFiles;
};
