#ifndef HEAVYAI_H
#define HEAVYAI_H

#include <QVector>
#include <QTimer>

#include "ai/coreai.h"
#include "ai/influencefrontmap.h"
#include "ai/targetedunitpathfindingsystem.h"
#include "game/unitpathfindingsystem.h"

#include "coreengine/LUPDATE_MACROS.h"

class HeavyAi : public CoreAI
{
    Q_OBJECT
    static const QString heavyAiObject;
    /**
     * @brief The BuildingEntry enum
     */
    enum BuildingEntry
    {
        BasicAttackRange,
        CaptureUnit,
        CoUnitValue,
        Movementpoints,
        MaxSize,
    };
public:
    ENUM_CLASS ThreadLevel
    {
        Normal,
                High,
                Hq
    };
    ENUM_CLASS FunctionType
    {
        JavaScript,
                CPlusPlus
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
    explicit HeavyAi();
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
    void scoreActions(UnitData & unit);
private:
    void setupTurn(const spQmlVectorBuilding & buildings);
    void endTurn();
    void createIslandMaps();
    void initUnits(QmlVectorUnit* pUnits, QVector<UnitData> & units, bool enemyUnits);
    void updateUnits();
    void updateUnits(QVector<UnitData> & units, bool enemyUnits);
    void findHqThreads(const spQmlVectorBuilding & buildings);
    bool isCaptureTransporterOrCanCapture(Unit* pUnit);
    void mutateActionForFields(UnitData & unit, const QVector<QPoint> & moveTargets,
                               QString action, FunctionType type, qint32 index,
                               float & bestScore, QVector<float> & bestScores,
                               QVector<spGameAction> & bestActions);
    bool mutateAction(spGameAction pAction, FunctionType type, qint32 functionIndex,
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
    float scoreCapture(spGameAction action);
    /**
     * @brief scoreFire
     * @param action
     * @return
     */
    float scoreFire(spGameAction action);
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
    void getMoveTargets(UnitData & unit, QVector<QVector3D> & targets);
    /**
     * @brief scoreWait
     * @param action
     * @return
     */
    float scoreWait(spGameAction action);
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
    void getProductionInputVector(Building* pBuilding, Unit* pUnit);
    /**
     * @brief buildUnits
     * @return
     */
    bool buildUnits();
private:
    // function for scoring a function
    using scoreFunction = std::function<float (spGameAction action)>;
    struct ScoreInfo
    {
        QString m_actionId;
        scoreFunction callback;
    };
    const QVector<ScoreInfo> m_scoreInfos;
    QVector<UnitData> m_enemyUnits;
    QVector<UnitData> m_ownUnits;
    QVector<QPoint> m_updatePoints;
    InfluenceFrontMap m_InfluenceFrontMap;

    spQmlVectorUnit m_pUnits = nullptr;
    spQmlVectorUnit m_pEnemyUnits = nullptr;

    QTimer m_timer;
    bool m_pause{false};

    spTargetedUnitPathFindingSystem m_currentTargetefPfs;

    static const qint32 minSiloDamage;
    float m_minActionScore{0.1f};
    float m_actionScoreVariant{0.05f};

};

#endif // HEAVYAI_H
