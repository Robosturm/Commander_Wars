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
public slots:
    /**
     * @brief process
     */
    virtual void process() override;
};

Q_DECLARE_INTERFACE(HeavyAi, "HeavyAi");
