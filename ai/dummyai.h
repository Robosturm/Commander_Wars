#pragma once

#include "ai/coreai.h"

class DummyAi;
using spDummyAi = oxygine::intrusive_ptr<DummyAi>;

class DummyAi final : public CoreAI
{
public:
    explicit DummyAi(GameMap* pMap, GameEnums::AiTypes type);
    ~DummyAi() = default;
    /**
     * @brief process empty impl
     */
    virtual void process() override {}
public slots:
    virtual void nextAction() override{};
};
