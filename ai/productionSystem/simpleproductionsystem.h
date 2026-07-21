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

class Building;
class CoreAI;
class GameMap;
class ProductionActionData;
using spProductionActionData = std::shared_ptr<ProductionActionData>;

class ProductionActionData final : public QObject
{
    Q_OBJECT
public:
    explicit ProductionActionData(GameMap* pMap, qint32 x, qint32 y, QString actionId);
    Q_INVOKABLE void addData(QString text, QString unitId, QString icon, qint32 transactionCost = 0, bool enabled = true);
    Q_INVOKABLE qint32 getX() const;
    Q_INVOKABLE qint32 getY() const;
    Q_INVOKABLE QString getActionId() const;
    Q_INVOKABLE bool getActionAvailable() const;
    Q_INVOKABLE QStringList getUnitIds() const;
    Q_INVOKABLE QVector<qint32> getTransactionCosts() const;
    Q_INVOKABLE QVector<qint32> getStrategicValues() const;
    Q_INVOKABLE QVector<bool> getEnabledList() const;
    void setActionAvailable(bool actionAvailable);
    bool validData() const;

private:
    GameMap* m_pMap{nullptr};
    qint32 m_x{-1};
    qint32 m_y{-1};
    QString m_actionId;
    bool m_actionAvailable{false};
    QStringList m_unitIds;
    QVector<qint32> m_transactionCosts;
    QVector<qint32> m_strategicValues;
    QVector<bool> m_enabledList;
};

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
        return 1;
    }
    void initialize();
    bool buildUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, QmlVectorUnit * pEnemyUnits, QmlVectorBuilding * pEnemyBuildings, bool & executed);
    void onNewBuildQueue(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, spQmlVectorUnit &pEnemyUnits, QmlVectorBuilding * pEnemyBuildings);
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
    Q_INVOKABLE bool buildNextUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, qint32 minBuildMode, qint32 maxBuildMode,
                                   qreal minAverageIslandSize = 0.025, qint32 minBaseCost = 0, qint32 maxBaseCost = -1, bool alwaysBuild = false);
    Q_INVOKABLE void addInitialProduction(const QStringList & unitIds, qint32 count);
    Q_INVOKABLE void addForcedProduction(const QStringList & unitId, qint32 x = -1, qint32 y = -1);
    Q_INVOKABLE void addForcedProductionCloseToTargets(const QStringList & unitIds, QmlVectorUnit* targets);
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
    Q_INVOKABLE ProductionActionData* getProductionActionData(Building* pBuilding, const QString & actionId) const;
    Q_INVOKABLE quint32 deriveCounterpointSeed(qint32 algorithmVersion, qint32 generation) const;
    Q_INVOKABLE qreal getCounterpointBaseDamage(const QString & attackerId, const QString & defenderId) const;
    Q_INVOKABLE bool executeCounterpointBuild(qint32 x, qint32 y, const QString & unitId, qint32 ordinal = 0, qint32 expectedCost = -1);
private:
    spProductionActionData queryProductionAction(Building* pBuilding, const QString & actionId) const;
    bool executeBuildAction(Building* pBuilding, const QString & unitId, qint32 ordinal, qint32 expectedCost, bool alwaysBuild);
    bool buildUnit(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize, bool alwaysBuild);
    bool buildUnitCloseTo(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize, const spQmlVectorUnit & pUnits, bool alwaysBuild);
    bool buildUnit(qint32 x, qint32 y, QString unitId, bool alwaysBuild);
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
    std::map<QString, BuildDistribution> m_buildDistribution;
    std::map<QString, BuildDistribution> m_activeBuildDistribution;
    std::map<Building*, AverageBuildData> m_averageMoverange;
    ScriptVariables m_Variables;
    spUnit m_dummy;
    qint32 m_currentTurnProducedUnitsCounter{0};
};

Q_DECLARE_INTERFACE(SimpleProductionSystem, "SimpleProductionSystem");
