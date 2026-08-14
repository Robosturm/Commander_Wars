#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <vector>
#include <map>
#include <memory>
#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/qmlvector.h"
#include "coreengine/jsthis.h"
#include "game/unit.h"
#include "ai/productionSystem/productionactiondata.h"

class Building;
class CoreAI;

class SimpleProductionSystem final : public QObject, public FileSerializable, public JsThis
{
    Q_OBJECT
public:
    struct InitialProduction
    {
        QStringList unitIds;
        qint32 count{0};
    };
    struct ForcedProduction
    {
        qint32 x{-1};
        qint32 y{-1};
        QStringList unitIds;
        spQmlVectorUnit targets;
    };
    struct PriorityProduction
    {
        qint32 x{-1};
        qint32 y{-1};
        bool blocking{true};
        QStringList unitIds;
    };
    // ordered by how retryable the failure is, so keeping the best reason is a plain comparison
    enum class BuildFailure
    {
        NoFactory,
        Danger,
        InvalidPosition,
        NotAFactory,
        NotAllowed,
        NotInBuildList,
        Disabled,
        FactoryBlocked,
        NoFunds,
    };
    struct BuildDistribution
    {
        QStringList unitIds;
        QVector<qint32> chance;
        QVector<spUnit> units;
        qint32 totalChance;
        qreal distribution;
        qreal maxUnitDistribution;
        qint32 buildMode;
        QString guardCondition;
    };
    struct CurrentBuildDistribution
    {
        qreal currentValue;
        BuildDistribution distribution;
    };
    struct AverageBuildData
    {
        qreal averageValue{0};
        std::map<QString, qint32> islandSizes;
    };

    explicit SimpleProductionSystem(CoreAI * owner);
    virtual ~SimpleProductionSystem() = default;

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
        return 2;
    }
    void initialize();
    bool buildUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, QmlVectorUnit * pEnemyUnits, QmlVectorBuilding * pEnemyBuildings, bool & executed);
    void onNewBuildQueue(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, spQmlVectorUnit &pEnemyUnits, QmlVectorBuilding * pEnemyBuildings);
    void prepareProduction(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits);
    void resetProductionPreparation();
    Q_INVOKABLE bool getInit() const;
    Q_INVOKABLE bool getEnabled() const;
    Q_INVOKABLE void setEnabled(bool newEnabled);
    /**
     * @brief getVariables
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    Q_INVOKABLE void resetBuildDistribution();
    Q_INVOKABLE void resetForcedProduction();
    Q_INVOKABLE void resetInitialProduction();
    Q_INVOKABLE void resetPriorityProduction();
    Q_INVOKABLE bool buildNextUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, qint32 minBuildMode, qint32 maxBuildMode,
                                   qreal minAverageIslandSize = 0.025, qint32 minBaseCost = 0, qint32 maxBaseCost = -1, bool alwaysBuild = false);
    Q_INVOKABLE void addInitialProduction(const QStringList & unitIds, qint32 count);
    Q_INVOKABLE void addForcedProduction(const QStringList & unitId, qint32 x = -1, qint32 y = -1);
    Q_INVOKABLE void addForcedProductionCloseToTargets(const QStringList & unitIds, QmlVectorUnit* targets);
    /**
     * @brief addPriorityProduction tried before any other production, skips the danger check but
     * never the funds or occupancy checks. A blocking entry that fails stops all other production
     * so funds bank up. An explicit (x, y) is never relocated; x = -1 allows any factory. Entries
     * persist through save/load, duplicates are ignored, and an entry is kept only while it fails
     * with NO_FUNDS or FACTORY_BLOCKED; any other failure drops it with a console warning so a
     * blocking entry cannot starve the ai forever.
     */
    Q_INVOKABLE void addPriorityProduction(const QStringList & unitIds, qint32 x = -1, qint32 y = -1, bool blocking = true);
    Q_INVOKABLE qint32 getPriorityProductionCount() const;
    /**
     * @brief getLastPriorityBuildResult "RESULT|unitId|x,y" of the last priority attempt, e.g.
     * BUILT, NO_FUNDS, FACTORY_BLOCKED, NOT_IN_BUILD_LIST, INVALID_POSITION, NOT_A_FACTORY, DISABLED, NOT_ALLOWED, NO_FACTORY
     */
    Q_INVOKABLE QString getLastPriorityBuildResult() const;
    Q_INVOKABLE void addItemToBuildDistribution(const QString & group, const QStringList & unitIds, const QVector<qint32> & chance, qreal distribution, qint32 buildMode, const QString & guardCondition = "", qreal maxUnitDistribution = 1.0);
    /**
     * @brief getDummyUnit creates a dummy unit to calculate values not only one dummy unit will be alive at all time.
     * @param unitId
     * @return
     */
    Q_INVOKABLE Unit* getDummyUnit(const QString & unitId);
    Q_INVOKABLE qint32 getProductionFromList(const QStringList & unitIds, QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings, qint32 minBuildMode, qint32 maxBuildMode, const QVector<bool> & enableList = QVector<bool>());
    Q_INVOKABLE void updateIslandSizeForBuildings(QmlVectorBuilding* pBuildings);
    Q_INVOKABLE qint32 getCurrentTurnProducedUnitsCounter() const;
    Q_INVOKABLE void setCurrentTurnProducedUnitsCounter(qint32 newCurrentTurnProducedUnitsCounter);

    Q_INVOKABLE qint32 getMaxDamageCheckRange() const;
    Q_INVOKABLE void setMaxDamageCheckRange(qint32 newMaxDamageCheckRange);

    Q_INVOKABLE qint32 getMaxSingleDamage() const;
    Q_INVOKABLE void setMaxSingleDamage(qint32 newMaxSingleDamage);
    Q_INVOKABLE bool reasonableBuildField(qint32 x, qint32 y, QString unitId, qint32 maxDamageCheckRange, qint32 maxSingleDamage);
    // Selects the first row when a unit id appears more than once in one action menu.
    static constexpr qint32 DEFAULT_ACTION_ORDINAL = 0;
    // Skips the live cost check, so the build proceeds at whatever the menu currently reports.
    static constexpr qint32 NO_EXPECTED_COST = -1;

    Q_INVOKABLE ProductionActionData* getProductionActionData(Building* pBuilding, const QString & actionId) const;
    Q_INVOKABLE quint32 deriveCounterpointSeed(qint32 algorithmVersion, qint32 generation) const;
    Q_INVOKABLE quint32 randomCounterpointSeed() const;
    Q_INVOKABLE qreal getCounterpointBaseDamage(const QString & attackerId, const QString & defenderId);
    Q_INVOKABLE bool executeCounterpointBuild(qint32 x, qint32 y, const QString & unitId, qint32 ordinal = DEFAULT_ACTION_ORDINAL, qint32 expectedCost = NO_EXPECTED_COST);
private:
    // Game script first, owning ai second, which is the seam every scripted hook here goes through.
    QJSValue dispatchScriptFunction(const QString & function, const QJSValueList & args) const;
    Building* ownedBuildingAt(qint32 x, qint32 y) const;
    bool isBaseProductionAction(const QString & actionId) const;
    spUnit getCounterpointUnit(const QString & unitId);
    spProductionActionData queryProductionAction(Building* pBuilding, const QString & actionId) const;
    bool executeBuildAction(Building* pBuilding, const QString & unitId, qint32 ordinal, qint32 expectedCost, bool alwaysBuild, BuildFailure * failureReason = nullptr);
    bool buildUnit(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize, bool alwaysBuild, BuildFailure * failureReason = nullptr);
    bool buildUnitCloseTo(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize, const spQmlVectorUnit & pUnits, bool alwaysBuild);
    bool buildUnit(qint32 x, qint32 y, QString unitId, bool alwaysBuild, BuildFailure * failureReason = nullptr);
    static void setBuildFailure(BuildFailure * failureReason, BuildFailure reason);
    static const char* buildFailureName(BuildFailure reason);
    bool buildPriorityProduction(QmlVectorBuilding* pBuildings, bool & blocked);
    void getBuildDistribution(std::vector<CurrentBuildDistribution> & buildDistribution, QmlVectorUnit* pUnits,
                              qint32 minBuildMode, qint32 maxBuildMode, qint32 minBaseCost, qint32 maxBaseCost);
    void updateActiveProductionSystem(QmlVectorBuilding* pBuildings);
private:
    CoreAI * m_owner{nullptr};
    bool m_init{false};
    bool m_enabled{true};
    spQmlVectorUnit m_pEnemyUnits;
    qint32 m_maxDamageCheckRange{10};
    qint32 m_maxSingleDamage{70};
    std::vector<InitialProduction> m_initialProduction;
    std::vector<ForcedProduction> m_forcedProduction;
    std::vector<PriorityProduction> m_priorityProduction;
    QString m_lastPriorityBuildResult;
    std::map<QString, BuildDistribution> m_buildDistribution;
    std::map<QString, BuildDistribution> m_activeBuildDistribution;
    std::map<Building*, AverageBuildData> m_averageMoverange;
    ScriptVariables m_Variables;
    spUnit m_dummy;
    // Matchup scoring asks for the same ids repeatedly and Unit construction is not cheap.
    std::map<QString, spUnit> m_counterpointUnits;
    // Scripted lookups, cached for the match since action scripts cannot change mid-game.
    mutable std::map<QString, bool> m_baseProductionActions;
    qint32 m_currentTurnProducedUnitsCounter{0};
    // Transient: a mid-turn load re-dispatches once and the strategy's own planner state absorbs it.
    bool m_productionPrepared{false};
};

Q_DECLARE_INTERFACE(SimpleProductionSystem, "SimpleProductionSystem");
