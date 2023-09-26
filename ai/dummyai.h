#pragma once

#include "ai/coreai.h"

class DummyAi;
using spDummyAi = std::shared_ptr<DummyAi>;

class DummyAi final : public CoreAI
{
public:
    explicit DummyAi(GameMap* pMap, GameEnums::AiTypes type);
    virtual ~DummyAi() = default;
public slots:
    virtual void nextAction() override{};
    /**
     * @brief process empty impl
     */
    virtual void process() override {}
};
