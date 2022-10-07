#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <vector>
#include <map>
#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"

class CoreAI;
class QmlVectorUnit;
class QmlVectorBuilding;

class SimpleProductionSystem final : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    struct InitialProduction
    {
        QString unitId;
        qint32 count{0};
    };
    struct ForcedProduction
    {
        qint32 x{-1};
        qint32 y{-1};
        QString unitId;
    };
    struct BuildDistribution
    {
        QString group;
        QStringList unitIds;
        QVector<qint32> chance;
        qint32 totalChance;
        float distribution;
        qint32 buildMode;
        QString guardCondition;
    };
    struct CurrentBuildDistribution
    {
        float currentValue;
        BuildDistribution distribution;
    };

    explicit SimpleProductionSystem(CoreAI * owner);
    ~SimpleProductionSystem() = default;

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
public slots:
    bool getEnabled() const;
    void setEnabled(bool newEnabled);
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    void resetBuildDistribution();
    void resetForcedProduction();
    void resetInitialProduction();
    bool buildNextUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, qint32 minBuildMode, qint32 maxBuildMode);
    void addInitialProduction(const QString & unitId, qint32 count);
    void addForcedProduction(const QString & unitId, qint32 x = -1, qint32 y = -1);
    void addItemToBuildDistribution(const QString & group, const QStringList & unitIds, const QVector<qint32> & chance, float distribution, qint32 buildMode, const QString & guardCondition = "");
private:
    bool buildUnit(QmlVectorBuilding* pBuildings, QString unitId);
    bool buildUnit(qint32 x, qint32 y, QString unitId);
private:
    CoreAI * m_owner{nullptr};
    bool m_init{false};
    bool m_enabled{true};
    std::vector<InitialProduction> m_initialProduction;
    std::vector<ForcedProduction> m_forcedProduction;
    std::map<QString, BuildDistribution> m_buildDistribution;
    ScriptVariables m_Variables;
};

Q_DECLARE_INTERFACE(SimpleProductionSystem, "SimpleProductionSystem");
