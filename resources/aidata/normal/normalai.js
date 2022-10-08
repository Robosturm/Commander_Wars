var NORMALAI =
{    
//    GenericPredefinedUnitMapScriptBehaviour : function(ai, action, unit, enemies, enemyBuildings, map)
//    {
//        // called when a unit has no predefined ai behaviour modify the action and return true for the ai to execute the defined action
//    },

    infantryGroup : ["INFANTRY_GROUP", ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE", "ZCOUNIT_PARTISAN", "ZCOUNIT_COMMANDO", "ZCOUNIT_RANGER", "ZCOUNIT_AT_CYCLE"],
                                       [70,         10,     20,       30,          10,                 30,                 10,               10],
                     40, 0, ""],
    lightTankGroup : ["LIGHT_TANK_GROUP",   ["RECON", "FLARE", "LIGHT_TANK", "ARTILLERY", "ZCOUNIT_CHAPERON", "ZCOUNIT_HOT_TANK", "ZCOUNIT_SMUGGLER", "ZCOUNIT_AUTO_TANK"],
                                            [5,       5,       80,           15,          15,                 15,                 15,                 30],
                     40, 1, ""],
    mediumTankGroup : ["MEDIUM_TANK_GROUP", ["ANTITANKCANNON", "HEAVY_TANK", "ROCKETTHROWER", "ZCOUNIT_CRYSTAL_TANK", "ZCOUNIT_NEOSPIDER_TANK", "ZCOUNIT_ROYAL_GUARD", "ZCOUNIT_TANK_HUNTER"],
                                            [30,               70,           10,              40,                     40,                       40,                    40],
                     20, 2, ""],
    heavyTankGroup : ["HEAVY_TANK_GROUP", ["MEGATANK", "NEOTANK", "ZCOUNIT_SIEGE_CANNON"],
                                            [60,       30,        60,],
                     10, 3, ""],
    lightAmphibiousGroup : ["LIGHT_AMPHIBIOUS_GROUP", ["ARTILLERYCRAFT", "HOVERCRAFT", "HOVERFLAK"],
                                                      [15,               80,           10,],
                     40, 1, ""],
    mediumAmphibiousGroup : ["MEDIUM_AMPHIBIOUS_GROUP", ["HEAVY_HOVERCRAFT",],
                                                        [100,],
                     20, 2, ""],
    lightAirForce : ["LIGHT_AIR_GROUP", ["K_HELI", "DUSTER",],
                                        [90,       10,],
                                        30, 1, ""],
    heavyAirForce : ["HEAVY_AIR_GROUP", ["BOMBER", "FIGHTER", "ZCOUNIT_KIROV"],
                                        [60,       10,        60],
                                        10, 3, ""],

    initializeSimpleProductionSystem : function(system, ai, map)
    {
        return COREAI.initializeSimpleProductionSystem(system, ai, map);
    },
    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return COREAI.buildUnitSimpleProductionSystem(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },
    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return COREAI.onNewBuildQueue(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },
};
