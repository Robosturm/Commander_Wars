#pragma once

#include "menue/ingamemenue.h"
#include "game/player.h"

class MovementPlanner : public InGameMenue
{
public:
    MovementPlanner(GameMap* pSourceMap, Player* pViewPlayer);
};

