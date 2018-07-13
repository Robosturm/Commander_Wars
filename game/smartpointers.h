#ifndef SMARTPOINTERS_H
#define SMARTPOINTERS_H

#include "oxygine-framework.h"

class Building;
typedef oxygine::intrusive_ptr<Building> spBuilding;

class Terrain;
typedef oxygine::intrusive_ptr<Terrain> spTerrain;

class Player;
typedef oxygine::intrusive_ptr<Player> spPlayer;

class Unit;
typedef oxygine::intrusive_ptr<Unit> spUnit;

#endif // SMARTPOINTERS_H
