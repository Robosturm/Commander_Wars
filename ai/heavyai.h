#pragma once
#include <QVector>
#include <QTimer>

#include "ai/coreai.h"
#include "ai/influencefrontmap.h"
#include "ai/targetedunitpathfindingsystem.h"
#include "ai/neuralnetwork/neural/neuralnetwork.h"

#include "game/unitpathfindingsystem.h"

#include "coreengine/LUPDATE_MACROS.h"

class GameMap;
class HeavyAi;
using spHeavyAi = oxygine::intrusive_ptr<HeavyAi>;

class HeavyAi : public CoreAI
{
    Q_OBJECT
    /**
     * @brief The BuildingEntry enum
     */
    enum BuildingEntry
    {
        DirectUnitRatio         ,
        IndirectUnitRatio       ,
        InfantryUnitRatio       ,
        TransportUnitRatio      ,
        TotalBuildingRatio      ,
        SupplyUnitRatio         ,
        SupplyRequiredRatio     ,
        EnemyRatio              ,
        ProductionUsage         ,
        DayProgression          ,
        LocalUnitData           ,
        BasicAttackRange = LocalUnitData,
        CaptureUnit             ,
        CoUnitValue             ,
        Movementpoints          ,
        MapMovementpoints,
        FondsUsage              ,
        FireRange               ,
        Flying                  ,
        LoadingPotential        ,
        OwnInfluence,
        HighestEnemyInfluence,
        DealingFundsDamage      ,
        DealingHpDamage         ,
        ReceivingFundsDamge     ,
        ReceivingHpDamage       ,
        CapturePotential        ,
        CanAttackImmuneUnitRatio,
        UnitsToTransportRatio,
        RequiredUnitsToTransportRatio,
        MovementPotential,
        VisionPotential,
        MaxUnitValue,
        BuildingEntryMaxSize,
    };

    enum BasicFieldInfo
    {
        OwnInfluenceValue,      // 0
        EnemyInfluenceValue,    // 1
        // the higher the more units haven't moved yet
        MoveTurnProgress,       // 2
        // amount of adjustant tiles blocked by an allied unit
        WallCount,              // 3
        // if the unit is potentially stealthed at this tile
        VisionHide,             // 4
        // how much movement points getting here will be consumed
        UsedMovement,           // 5
        EnemyThread,            // 6
        OwnProtection,          // 7
        UnitHealth,             // 8
        VisionRange,            // 9
        TerrainDefense,         // 10
        ProductionOwner,        // 11
        FrontTile,              // 12
        BasicFieldInfoMaxSize,
    };

    enum AttackInfo
    {
        AttackInfoStart = BasicFieldInfoMaxSize,
        AttackDealingHpDamage = AttackInfoStart,
        AttackReceavingHpDamage,
        AttackDealingAbsolutDamage,
        AttackReceicingAbsolutDamage,
        AttackDealingFundsDamage,
        AttackReceicingFundsDamage,
        AttackRemainingCaptureDays,
        HqThread,
        AttackInfoMaxSize,
    };

    enum CaptureInfo
    {
        CaptureInfoStart = BasicFieldInfoMaxSize,
        CaptureInfoIsHq = CaptureInfoStart,
        CaptureInfoIsComTower,
        CaptureInfoProductionPotential,
        CaptureInfoRemainingDays,
        CaptureInfoCaptureOptions,
        CaptureInfoUniqueCaptureBuilding,
        CaptureInfoMaxSize,
    };

    enum WaitTargetTypes
    {
        WaitTargetTypes_Capture,
        WaitTargetTypes_Silo,
        WaitTargetTypes_Enemy,
        WaitTargetTypes_Terrain,
        WaitTargetTypes_CaptureLoad,
        WaitTargetTypes_Load,
        WaitTargetTypes_Unload,
        WaitTargetTypesMaxSize,
    };

    // flare?
    // place watermine
    // support repair and ration actions

    // load
    // unload always
    // wait / stealth / unstealth

    enum NeuralNetworks
    {
        Production,
        ActionFire,
        ActionCapture,
        WaitDistanceMultiplier,
        NeuralNetworksMax,
    };
public:
    ENUM_CLASS ThreadLevel
    {
        Normal,
        High,
        Hq,
        Max,
    };
    ENUM_CLASS FunctionType
    {
        JavaScript,
        CPlusPlus,
    };

    struct UnitData
    {
        Unit* m_pUnit;
        spUnitPathFindingSystem m_pPfs;
        qint32 m_movepoints{0};
        float m_virtualDamage{0.0f};
        ThreadLevel m_threadLevel{ThreadLevel::Normal};
        QPoint m_hqThread;
        spGameAction m_action;
        float m_score{0};
        QVector<QPoint> m_capturePoints;
        QStringList m_actions;
    };
    struct UnitBuildData
    {
        bool enabled{true};
        QString unitId;
        qint32 cost{0};
        QVector<double> unitBuildingDataInput = QVector<double>(static_cast<qsizetype>(BuildingEntryMaxSize));
    };
    struct BuildingData
    {
        Building* m_pBuilding;
        spGameAction m_action;
        float m_score{0};
        qint32 m_selectedData{-1};
        QVector<UnitBuildData> buildingDataInput;
    };

    explicit HeavyAi(QString type);
    virtual ~HeavyAi() = default;

    void loadNeuralNetwork(QString netName, spNeuralNetwork & network, qint32 inputVectorSize, qint32 netDepth, qint32 outputSize = 1);
public slots:
    /**
     * @brief process
     */
    virtual void process() override;
    /**
     * @brief readIni
     * @param name
     */
    virtual void readIni(QString name) override;


    /*******************************************************************/
    // training section
    /*******************************************************************/
    /**
     * @brief getMaxNeuralNetworks amount of neural networks owned by the heavy ai
     * @return
     */
    qint32 getMaxNeuralNetworks()
    {
        return static_cast<qint32>(NeuralNetworksMax);
    }
    /**
     * @brief saveNeuralNetwork saves the network under the given name
     * @param network
     */
    void saveNeuralNetwork(qint32 network);
    /**
     * @brief getNeuralNetworkName
     * @param network
     * @return
     */
    QString getNeuralNetworkName(qint32 network);
    /**
     * @brief mutateNeuralNetwork
     * @param network
     * @param mutationChance
     * @return
     */
    void mutateNeuralNetwork(qint32 network, double mutationChance);
    /**
     * @brief getAiName
     * @return
     */
    const QString &getAiName() const;
    /**
     * @brief setAiName
     * @param newAiName
     */
    void setAiName(const QString &newAiName);
    /**
     * @brief loadNeuralNetworks
     */
    void loadNeuralNetworks();
    /**
     * @brief combineAi
     * @param aisToUse
     */
    void combineAi(QStringList aisToUse);
    /*******************************************************************/
    // debugging section
    /*******************************************************************/
    /**
     * @brief toggleAiPause for debugging
     */
    void toggleAiPause();
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
    /**
     * @brief showUnitPfs
     * @param enemy
     * @param index
     */
    void showUnitPfs(bool enemy, qint32 index);
protected:
    /**
     * @brief scoreActions
     * @param unit
     */
    void scoreActions(UnitData & unit);
    /**
     * @brief prepareWaitPfs
     * @param unitData
     * @param actions
     */
    void prepareWaitPfs(UnitData & unitData, QStringList & actions);
private:
    void setupTurn(const spQmlVectorBuilding & buildings);
    void endTurn();
    void createIslandMaps();
    void initUnits(spQmlVectorUnit pUnits, QVector<UnitData> & units, bool enemyUnits);
    void addNewUnitToUnitData(QVector<UnitData> & units, Unit* pUnit, bool enemyUnits);
    void updateUnits();
    void updateUnits(QVector<UnitData> & units, spQmlVectorUnit & pUnits, bool enemyUnits);
    void updateCaptureBuildings(UnitData & unitData);
    void findHqThreads(const spQmlVectorBuilding & buildings);
    bool isCaptureTransporterOrCanCapture(Unit* pUnit);
    void mutateActionForFields(UnitData & unit, const QVector<QPoint> & moveTargets,
                               QString action, FunctionType type, qint32 index,
                               float & bestScore, QVector<float> & bestScores,
                               QVector<spGameAction> & bestActions);
    bool mutateAction(spGameAction pAction, UnitData & unitData, QVector<double> & baseData, FunctionType type, qint32 functionIndex,
                      qint32 & step, QVector<qint32> & stepPosition, float & score);
    /**
     * @brief scoreWait
     * @param unit
     */
    void scoreMoveToTargets();
    /**
     * @brief performAction
     */
    bool selectActionToPerform();
    /**
     * @brief scoreCapture
     * @param action
     * @return
     */
    float scoreCapture(spGameAction action, UnitData & unitData, QVector<double> baseData);
    /**
     * @brief scoreFire
     * @param action
     * @return
     */
    float scoreFire(spGameAction action, UnitData & unitData, QVector<double> baseData);
    /**
     * @brief scoreJoin
     * @param action
     * @param unitData
     * @param baseData
     * @return
     */
    float scoreJoin(spGameAction action, UnitData & unitData, QVector<double> baseData);
    /**
     * @brief scoreMissile
     * @param action
     * @param unitData
     * @param baseData
     * @return
     */
    float scoreMissile(spGameAction action, UnitData & unitData, QVector<double> baseData);
    /**
     * @brief scoreLoad
     * @param action
     * @param unitData
     * @param baseData
     * @return
     */
    float scoreLoad(spGameAction action, UnitData & unitData, QVector<double> baseData);
    /**
     * @brief getMoveTargets
     * @param unit
     * @param targets
     */
    void getMoveTargets(UnitData & unit, QStringList & actions, QVector<QVector3D> & targets);
    /**
     * @brief scoreWait
     * @param action
     * @return
     */
    float scoreWait(spGameAction action, UnitData & unitData, QVector<double> baseData);
    /**
     * @brief addCaptureTargets
     * @param pUnit
     * @param actions
     * @param pEnemyBuildings
     * @param targets
     */
    void addCaptureTargets(const QStringList & actions,
                           Terrain* pTerrain, QVector<QVector3D>& targets);
    /**
     * @brief addAttackTargets
     * @param pUnit
     * @param pTargetFields
     * @param targets
     */
    void addAttackTargets(Unit* pUnit, Terrain* pTerrain, QmlVectorPoint* pTargetFields, QVector<QVector3D> & targets);
    /**
     * @brief addTransporterTargets
     */
    void addTransporterTargets(Unit* pUnit, Terrain* pTerrain, QVector<QVector3D>& targets);
    /**
     * @brief addCaptureTransporterTargets
     */
    void addCaptureTransporterTargets(Unit* pUnit, const QStringList & actions,
                                      Terrain* pTerrain, QVector<QVector3D>& targets);
    /**
     * @brief getBasicFieldInputVector
     * @param action
     * @param data
     */
    void getBasicFieldInputVector(spGameAction action, QVector<double> & data);
    /**
     * @brief getFunctionType
     * @param action
     * @param type
     * @param index
     */
    void getFunctionType(QString action, FunctionType & type, qint32 & index);
    /**
     * @brief getProductionInputVector
     * @param pBuilding
     * @param pUnit
     */
    void getProductionInputVector(Building* pBuilding, Unit* pUnit, UnitBuildData & data, const QVector<Unit*> & immuneUnits, qint32 movementPoints);
    /**
     * @brief buildUnits
     * @return
     */
    bool buildUnits(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                    spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief scoreUnitBuilding
     */
    void scoreUnitBuildings(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                            spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief scoreBuildingProductionData
     */
    void scoreBuildingProductionData(BuildingData & building);
    /**
     * @brief getGlobalBuildInfo
     * @param pBuildings
     * @param pUnits
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    QVector<double> getGlobalBuildInfo(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                                       spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                                       QVector<std::tuple<Unit*, Unit*>> & transportTargets);
    /**
     * @brief createUnitBuildData
     * @param building
     */
    void createUnitBuildData(BuildingData & building, QVector<double> & data, qint32 funds, const QVector<Unit*> & immuneUnits,
                             const QVector<std::tuple<Unit*, Unit*>> & transportTargets, const spQmlVectorBuilding & pEnemyBuildings);
    /**
     * @brief UpdateUnitBuildData
     * @param unitData
     * @param data
     * @param funds
     */
    void updateUnitBuildData(BuildingData & building, QVector<double> & data, qint32 funds);
    /**
     * @brief updateUnitBuildData
     * @param unitData
     * @param data
     * @param funds
     */
    void updateUnitBuildData(UnitBuildData &unitData, QVector<double> &data, qint32 funds);
    /**
     * @brief calculateUnitProductionDamage
     * @param pBuilding
     * @param pUnit
     * @param data
     */
    void calculateUnitProductionDamage(Building* pBuilding, Unit* pUnit, qint32 movementPoints, QPoint position, UnitBuildData & data, const QVector<Unit*> & immuneUnits);
    /**
     * @brief getBaseDamage
     * @param pAttacker
     * @param pDefender
     */
    float getBaseDamage(Unit* pAttacker, Unit* pDefender);
    /**
     * @brief getProductionScoreMultiplier
     * @param position
     * @param target
     * @param movementPoints
     * @return
     */
    float getProductionScoreMultiplier(QPoint position, QPoint target, qint32 movementPoints);
    /**
     * @brief getImmuneUnits
     * @param pUnits
     * @param pEnemyUnits
     * @param immuneUnits
     */
    void getImmuneUnits(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, QVector<Unit*> & immuneUnits);
    /**
     * @brief getTransportInputVector
     * @param building
     * @param transportTargets
     * @param unitData
     * @param pEnemyBuildings
     * @param data
     */
    void getTransportInputVector(Building* pBuilding, Unit* pUnit, const QVector<std::tuple<Unit*, Unit*>> transportTargets,
                                 const spQmlVectorBuilding & pEnemyBuildings, qint32 movementPoints, UnitBuildData & data);
    /**
     * @brief isPrimaryEnemy
     * @param pPlayer
     * @return
     */
    bool isPrimaryEnemy(Player* pPlayer) const;
    /**
     * @brief isPrimaryEnemy
     * @param pUnit
     * @return
     */
    bool isPrimaryEnemy(Unit* pUnit) const;
    /**
     * @brief isPrimaryEnemy
     * @param pBuilding
     * @return
     */
    bool isPrimaryEnemy(Building* pBuilding) const;
    /**
     * @brief isScoringAllowed
     * @param action
     * @param actions
     * @return
     */
    bool isScoringAllowed(QString action, QStringList actions);
    /**
     * @brief getNumberOfTargetsOnIsland
     * @param ignoreList
     * @return
     */
    qint32 getNumberOfTargetsOnIsland(const QVector<QPoint> & ignoreList);
private:
    // function for scoring a function
    using scoreFunction = std::function<float (spGameAction action, UnitData & unitData, QVector<double> baseData)>;
    struct ScoreInfo
    {
        QString m_actionId;
        scoreFunction callback;
    };
    const QVector<ScoreInfo> m_scoreInfos;
    QVector<UnitData> m_enemyUnits;
    QVector<UnitData> m_ownUnits;
    QVector<QPoint> m_updatePoints;
    QVector<BuildingData> m_BuildingData;
    QVector<QPoint> m_planedCaptureTargets;
    InfluenceFrontMap m_InfluenceFrontMap;
    spQmlVectorUnit m_pUnits = spQmlVectorUnit();
    spQmlVectorUnit m_pEnemyUnits = spQmlVectorUnit();
    Player* m_pPrimaryEnemy{nullptr};
    QTimer m_timer;
    bool m_pause{false};
    QStringList m_secondyActions
    {
        ACTION_WAIT,
        ACTION_LOAD,
        ACTION_STEALTH,
        ACTION_UNSTEALTH,
    };
    QStringList m_thirdActions
    {
        ACTION_UNLOAD,
    };
    spTargetedUnitPathFindingSystem m_currentTargetedfPfs;

    double m_minActionScore{0.2};
    double m_actionScoreVariant{0.05};
    double m_stealthDistanceMultiplier{2.0};
    double m_alliedDistanceModifier{5.0};
    double m_maxMovementpoints{15.0};
    double m_maxFirerange{10};
    double m_maxProductionTurnRange{4};
    double m_maxVision{10};
    double m_primaryEnemyMultiplier{1.2};
    double m_maxLoadingPlace{4};
    double m_notAttackableDamage{20.0};
    double m_ownUnitProtection{5};
    double m_enemyUnitThread{5};
    double m_maxUnitValue{40000.0};
    double m_maxScore{10.0f};
    double m_maxTerrainDefense{15.0};
    double m_maxCapturePoints = 20;
    double m_earlyGameDays{6.0f};

    // storable stuff
    QString m_aiName{"HEAVY_AI"};
    QList<spNeuralNetwork> m_neuralNetworks{static_cast<qsizetype>(NeuralNetworksMax)};

    // static constants
    static const qint32 minSiloDamage;
    static const QStringList NeuralNetworkNames;
    static const char* const NeuralNetworkFileEnding;
    static const char* const NeuralNetworkPath;
};
