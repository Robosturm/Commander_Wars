#ifndef HEAVYAI_H
#define HEAVYAI_H

#include <QVector>
#include <QTimer>

#include "ai/coreai.h"
#include "ai/influencefrontmap.h"
#include "ai/targetedunitpathfindingsystem.h"
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
        DirectUnitRatio,
        IndirectUnitRatio,
        InfantryUnitRatio,
        TotalBuildingRatio,
        EnemyRatio,
        ProductionUsage,
        LocalUnitData,
        BasicAttackRange = LocalUnitData,
        CaptureUnit,
        CoUnitValue,
        Movementpoints,
        FondsUsage,


        MaxSize,
    };
public:
    ENUM_CLASS ThreadLevel
    {
        Normal,
        High,
        Hq,
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
    };
    struct UnitBuildData
    {
        QString unitId;
        qint32 cost{0};
        QVector<double> unitBuildingDataInput;
    };

    struct BuildingData
    {
        Building* m_pBuilding;
        spGameAction m_action;
        float m_score{0};
        QVector<UnitBuildData> buildingDataInput;
    };


    explicit HeavyAi(QString type);
    virtual ~HeavyAi() = default;
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

    void toggleAiPause();

    void showFrontMap();
    void showFrontLines();
    void hideFrontMap();
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
    void updateUnits();
    void updateUnits(QVector<UnitData> & units, bool enemyUnits);
    void findHqThreads(const spQmlVectorBuilding & buildings);
    bool isCaptureTransporterOrCanCapture(Unit* pUnit);
    void mutateActionForFields(UnitData & unit, const QVector<QPoint> & moveTargets,
                               QString action, FunctionType type, qint32 index,
                               float & bestScore, QVector<float> & bestScores,
                               QVector<spGameAction> & bestActions);
    bool mutateAction(spGameAction pAction, UnitData & unitData, FunctionType type, qint32 functionIndex,
                      qint32 & step, QVector<qint32> & stepPosition, float & score);
    /**
     * @brief performAction
     */
    bool selectActionToPerform();
    /**
     * @brief scoreCapture
     * @param action
     * @return
     */
    float scoreCapture(spGameAction action, UnitData & unitData);
    /**
     * @brief scoreFire
     * @param action
     * @return
     */
    float scoreFire(spGameAction action, UnitData & unitData);
    /**
     * @brief scoreWait
     * @param unit
     */
    void scoreActionWait();
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
    float scoreWait(spGameAction action, UnitData & unitData);
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
     * @brief getCaptureDistanceModifier
     * @return
     */
    qint32 getMovingToCaptureDistanceModifier();
    /**
     * @brief addAttackTargets
     * @param pUnit
     * @param pTargetFields
     * @param targets
     */
    void addAttackTargets(Unit* pUnit, Terrain* pTerrain, QmlVectorPoint* pTargetFields, QVector<QVector3D> & targets);
    /**
     * @brief getMovingToAttackDistanceModifier
     */
    qint32 getMovingToAttackDistanceModifier();
    /**
     * @brief getMovingToAttackEnvironmentDistanceModifier
     * @return
     */
    qint32 getMovingToAttackEnvironmentDistanceModifier();
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
     * @brief scoreProduction
     */
    void scoreProduction();
    /**
     * @brief getProductionInputVector
     * @param pBuilding
     * @param pUnit
     */
    void getProductionInputVector(Building* pBuilding, Unit* pUnit, QVector<double> & data);
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
     * @brief getGlobalBuildInfo
     * @param pBuildings
     * @param pUnits
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    QVector<double> getGlobalBuildInfo(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                                       spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief createUnitBuildData
     * @param building
     */
    void createUnitBuildData(BuildingData & building, QVector<double> & data, qint32 funds);
    /**
     * @brief UpdateUnitBuildData
     * @param unitData
     * @param data
     * @param funds
     */
    void updateUnitBuildData(UnitBuildData & unitData, QVector<double> & data, qint32 funds);
private:
    // function for scoring a function
    using scoreFunction = std::function<float (spGameAction action, UnitData & unitData)>;
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
    InfluenceFrontMap m_InfluenceFrontMap;

    spQmlVectorUnit m_pUnits = nullptr;
    spQmlVectorUnit m_pEnemyUnits = nullptr;

    QTimer m_timer;
    bool m_pause{false};

    spTargetedUnitPathFindingSystem m_currentTargetedfPfs;

    static const qint32 minSiloDamage;
    float m_minActionScore{0.1f};
    float m_actionScoreVariant{0.05f};
    float m_stealthDistanceMultiplier{2.0f};
    float m_alliedDistanceModifier{5.0f};
    float m_maxMovementpoints{20.0f};

    // storable stuff
    QString m_aiName{"HEAVY_AI"};
};

#endif // HEAVYAI_H
