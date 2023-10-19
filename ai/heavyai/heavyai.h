#pragma once

#include "ai/coreai.h"

class GameMap;
class HeavyAi;
using spHeavyAi = std::shared_ptr<HeavyAi>;
class SituationEvaluator;
using spSituationEvaluator = std::shared_ptr<SituationEvaluator>;

/**
 * @brief The HeavyAi class
 * How the ai and what it does:
 * 1. Uses the same capturing system as the normal ai with the difference to run the situationevalutor before-hand
 * 2. Loading and transporting is done with the same code as for all other ai's
 * 3. Waiting uses the situationevalutor instead of the normal ai's waiting code
 * 4. Situationevalutor takes the closest 40 units and tests if the new simulated game state is a win or a loose and makes the move with the highest score.
 * 5. MovementFilterer will make a fast decision which fields should be evaluated by the Situationevalutor to save same processing time
 * 6. The unitselector makes a decision which unit should be moved next based on the current situation
 * 7. The buildsystem will make the decision which unit should be build next
 */
class HeavyAi final : public CoreAI
{
    Q_OBJECT
public:

    explicit HeavyAi(GameMap* pMap, QString type, GameEnums::AiTypes aiType);
    virtual ~HeavyAi() = default;
    virtual void init(BaseGamemenu* pMenu) override;
    /*******************************************************************/
    // debugging section
    /*******************************************************************/
    /**
     * @brief toggleAiPause for debugging
     */
    Q_INVOKABLE void toggleAiPause();
    /**
     * @brief showIslandMap
     * @param unitId
     */
    Q_INVOKABLE void showIslandMap(QString unitId);
    /**
     * @brief showIslandMap
     * @param unitId
     */
    Q_INVOKABLE void hideIslandMap(QString unitId);

public slots:
    /**
     * @brief process
     */
    virtual void process() override;
protected:
    /**
     * @brief finishTurn
     */
    virtual void finishTurn() override;
    /**
     * @brief clearUnitCache
     */
    void clearUnitCache(spQmlVectorUnit & pUnits);
    /**
     * @brief updateUnitCache
     * @param pUnits
     */
    void updateUnitCache(spQmlVectorUnit & pUnits);
private:
    double m_minSiloDamage{7000};
    QTimer m_timer;
    bool m_pause{false};
    spSituationEvaluator m_evaluator;
};

Q_DECLARE_INTERFACE(HeavyAi, "HeavyAi");
