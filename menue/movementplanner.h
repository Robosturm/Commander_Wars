#pragma once

#include "menue/gamemenue.h"
#include "game/player.h"
#include "game/gameanimation/animationskipper.h"

class MovementPlanner;
using spMovementPlanner = oxygine::intrusive_ptr<MovementPlanner>;

class MovementPlanner final : public GameMenue
{
public:
    MovementPlanner(GameMenue* pOwner, GameMap* pSourceMap, Player* pViewPlayer);
    ~MovementPlanner() = default;

    void onShowPlanner();
    void onExitPlanner();
private:
    GameMenue* m_pOwner{nullptr};
    AnimationSkipper m_animationSkipper;
};

