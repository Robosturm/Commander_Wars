#pragma once

#include <QObject>

#include "3rd_party/opennn/opennn/opennn.h"

#include "ai/heavyai/heavyAiSharedData.h"

class Player;
class Unit;
class GameMap;
class QmlVectorPoint;
using spQmlVectorPoint = std::shared_ptr<QmlVectorPoint>;
class SituationEvaluator;
using spSituationEvaluator = std::shared_ptr<SituationEvaluator>;

class SituationEvaluator : public QObject
{
    Q_OBJECT
public:

    explicit SituationEvaluator(Player* pOwner);
    /**
     * @brief updateInputVector
     * @param pMap
     * @param searchPoint position around which we want to decide if it's a winning, loosing or draw situation
     * @param updateUnitData if units should be refetched and all pfs need to research
     */
    void updateInputVector(GameMap* pMap, const QPoint & searchPoint, bool updateUnitData);
    /**
     * @brief loadNetwork
     * @param filePath
     */
    void loadNetwork(const QString & filePath);
    /**
     * @brief getOutput
     * @return
     */
    float getOutput();
private:
    void getUnitsInRange(GameMap* pMap, const QPoint & searchPoint);
    void createPathFindingSystems(GameMap* pMap);
    void clearUnitInput(qint32 index);
    void fillUnitInput(qint32 index);
    bool shouldFillInfo(const HeavyAiSharedData::spUnitInfo & unitInfo, qint32 enemyUnit);
    void updateHp(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateHpDamage(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateFundsDamage(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateMovementPoints(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateDistance(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateHasMoved(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateDefense(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateRepairsOnPosition(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateCapturePoints(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateBuildingImportance(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateStealthed(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
    void updateBuildingImportance(qint32 unitPosition);
    void updateStealthInfo(GameMap* pMap, qint32 unitPosition);
private:
    opennn::Tensor<opennn::type, 2> m_inputVector;
    opennn::NeuralNetwork m_neuralNetwork;
    std::array<HeavyAiSharedData::spUnitInfo, HeavyAiSharedData::UNIT_COUNT> m_unitsInfo;
    spQmlVectorPoint m_searchRange;
    Player* m_pOwner{nullptr};
};

