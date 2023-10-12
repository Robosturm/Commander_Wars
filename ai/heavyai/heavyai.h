#pragma once

#include "ai/coreai.h"

class GameMap;
class HeavyAi;
using spHeavyAi = std::shared_ptr<HeavyAi>;

class HeavyAi final : public CoreAI
{
    Q_OBJECT
public:

    explicit HeavyAi(GameMap* pMap, QString type, GameEnums::AiTypes aiType);
    virtual ~HeavyAi() = default;
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
};

Q_DECLARE_INTERFACE(HeavyAi, "HeavyAi");
