#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <vector>
#include <map>
#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/qmlvector.h"
#include "game/unit.h"

class Building;
class CoreAI;

class SimpleProductionSystem final : public QObject, public FileSerializable
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
        return 0;
    }
    void initialize();
    bool buildUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, QmlVectorUnit * pEnemyUnits, QmlVectorBuilding * pEnemyBuildings, bool & executed);
    void onNewBuildQueue(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, QmlVectorUnit * pEnemyUnits, QmlVectorBuilding * pEnemyBuildings);
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
                                   qreal minAverageIslandSize = 0.025, qint32 minBaseCost = 0, qint32 maxBaseCost = -1);
    Q_INVOKABLE void addInitialProduction(const QStringList unitIds, qint32 count);
    Q_INVOKABLE void addForcedProduction(const QStringList unitId, qint32 x = -1, qint32 y = -1);
    Q_INVOKABLE void addForcedProductionCloseToTargets(const QStringList unitIds, QmlVectorUnit* targets);
    Q_INVOKABLE void addItemToBuildDistribution(const QString group, const QStringList unitIds, const QVector<qint32> chance, qreal distribution, qint32 buildMode, const QString guardCondition = "", qreal maxUnitDistribution = 1.0);
    /**
     * @brief getDummyUnit creates a dummy unit to calculate values not only one dummy unit will be alive at all time.
     * @param unitId
     * @return
     */
    Q_INVOKABLE Unit* getDummyUnit(const QString unitId);
    Q_INVOKABLE qint32 getProductionFromList(const QStringList unitIds, QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings, qint32 minBuildMode, qint32 maxBuildMode, const QVector<bool> enableList = QVector<bool>());
    Q_INVOKABLE void updateIslandSizeForBuildings(QmlVectorBuilding* pBuildings);
private:
    bool buildUnit(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize);
    bool buildUnitCloseTo(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize, const spQmlVectorUnit & pUnits);
    bool buildUnit(qint32 x, qint32 y, QString unitId);
    void getBuildDistribution(std::vector<CurrentBuildDistribution> & buildDistribution, QmlVectorUnit* pUnits,
                              qint32 minBuildMode, qint32 maxBuildMode, qint32 minBaseCost, qint32 maxBaseCost);
    void updateActiveProductionSystem(QmlVectorBuilding* pBuildings);
private:
    CoreAI * m_owner{nullptr};
    bool m_init{false};
    bool m_enabled{true};
    std::vector<InitialProduction> m_initialProduction;
    std::vector<ForcedProduction> m_forcedProduction;
    std::map<QString, BuildDistribution> m_buildDistribution;
    std::map<QString, BuildDistribution> m_activeBuildDistribution;
    std::map<Building*, AverageBuildData> m_averageMoverange;
    ScriptVariables m_Variables;
    spUnit m_dummy;
};

Q_DECLARE_INTERFACE(SimpleProductionSystem, "SimpleProductionSystem");
