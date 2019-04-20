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
    bool useCOPower(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits);
    bool useBuilding(QmlVectorBuilding* pBuildings);
    bool captureBuildings(QmlVectorUnit* pUnits);
    bool fireWithIndirectUnits(QmlVectorUnit* pUnits);
    bool fireWithDirectUnits(QmlVectorUnit* pUnits);
    bool attack(Unit* pUnit);
    void finishTurn();
private:
    DecisionTree m_COPowerTree;

    DecisionTree m_BaseBuildingTree;
    DecisionTree m_AirportBuildingTree;
    DecisionTree m_HarbourBuildingTree;

    TurnTime turnMode{TurnTime::startOfTurn};
};

#endif // VERYEASYAI_H
