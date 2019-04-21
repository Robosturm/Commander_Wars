#ifndef VERYEASYAI_H
#define VERYEASYAI_H

#include "ai/coreai.h"
#include "ai/decisiontree.h"

class QmlVectorUnit;
class QmlVectorBuilding;

class VeryEasyAI : public CoreAI
{
    Q_OBJECT
public:
    enum class TurnTime
    {
        startOfTurn,
        onGoingTurn,
        endOfTurn,
    };
    VeryEasyAI();

    /**
     * @brief process
     */
    virtual void process() override;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() override
    {
        return 1;
    }
protected:
    /**
     * @brief useCOPower
     * @param pUnits
     * @param pEnemyUnits
     * @return
     */
    bool useCOPower(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits);
    /**
     * @brief useBuilding
     * @param pBuildings
     * @return
     */
    bool useBuilding(QmlVectorBuilding* pBuildings);
    /**
     * @brief buildCOUnit
     * @param pUnits
     * @return
     */
    bool buildCOUnit(QmlVectorUnit* pUnits);
    /**
     * @brief captureBuildings
     * @param pUnits
     * @return
     */
    bool captureBuildings(QmlVectorUnit* pUnits);
    /**
     * @brief fireWithIndirectUnits
     * @param pUnits
     * @return
     */
    bool fireWithIndirectUnits(QmlVectorUnit* pUnits);
    /**
     * @brief fireWithDirectUnits
     * @param pUnits
     * @return
     */
    bool fireWithDirectUnits(QmlVectorUnit* pUnits);
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
    bool moveUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings,
                    QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings);
    /**
     * @brief buildUnits
     * @param pBuildings
     * @param pUnits
     * @return
     */
    bool buildUnits(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits);
    /**
     * @brief finishTurn
     */
    void finishTurn();
private:
    DecisionTree m_COPowerTree;
    DecisionTree m_COUnitTree;
    DecisionTree m_GeneralBuildingTree;
    DecisionTree m_AirportBuildingTree;
    DecisionTree m_HarbourBuildingTree;

    TurnTime turnMode{TurnTime::startOfTurn};
};

#endif // VERYEASYAI_H
