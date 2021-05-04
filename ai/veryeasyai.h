#ifndef VERYEASYAI_H
#define VERYEASYAI_H

#include "qvector.h"

#include "ai/coreai.h"
#include "ai/decisiontree.h"

class QmlVectorUnit;
class QmlVectorBuilding;
class QmlVectorPoint;
class Building;

class VeryEasyAI;
using spVeryEasyAI = oxygine::intrusive_ptr<VeryEasyAI>;

class VeryEasyAI : public CoreAI
{
    Q_OBJECT
public:

    VeryEasyAI();
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
protected:
    /**
     * @brief performActionSteps
     * @param pUnits
     * @param pEnemyUnits
     * @param pBuildings
     * @param pEnemyBuildings
     * @return
     */
    bool performActionSteps(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits,
                            spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings);
    /**
     * @brief captureBuildings
     * @param pUnits
     * @return
     */
    bool captureBuildings(spQmlVectorUnit pUnits);
    /**
     * @brief fireWithIndirectUnits
     * @param pUnits
     * @return
     */
    bool fireWithIndirectUnits(spQmlVectorUnit pUnits);
    /**
     * @brief fireWithDirectUnits
     * @param pUnits
     * @return
     */
    bool fireWithDirectUnits(spQmlVectorUnit pUnits);
    /**
     * @brief attack
     * @param pUnit
     * @return
     */
    bool attack(Unit* pUnit);
    /**
     * @brief moveUnits
     * @param pUnits
     * @param pBuildings
     * @param pEnemyUnits
     * @param pEnemyBuildings
     * @return
     */
    bool moveUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings,
                    spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings, bool supportUnits = false);

    bool moveTransporters(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings);

    /**
     * @brief loadUnits
     * @param pUnits
     * @return
     */
    bool loadUnits(spQmlVectorUnit pUnits);
    /**
     * @brief VeryEasyAI::moveUnit
     * @param pAction
     * @param pUnit
     * @param actions
     * @param targets
     * @param transporterTargets
     * @return
     */
    bool moveUnit(spGameAction pAction, Unit* pUnit, QStringList& actions,
                  QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets, bool unload = false);
    /**
     * @brief buildUnits
     * @param pBuildings
     * @param pUnits
     * @return
     */
    bool buildUnits(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits);
    /**
     * @brief finishTurn
     */
    virtual void finishTurn() override;
protected:
private:
    DecisionTree m_GeneralBuildingTree;
    DecisionTree m_AirportBuildingTree;
    DecisionTree m_HarbourBuildingTree;
    bool rebuildIslandMaps{true};
    qint32 m_minSiloDamage{4000};
    qint32 m_minDamage{-500};
    float m_ownUnitDamageDivider{4};
    qint32 m_minAllBuildingFunds{8000};
    qint32 m_maxTreeDecisionTries{10};
};

#endif // VERYEASYAI_H
