var NORMALAI =
{    
//    GenericPredefinedUnitMapScriptBehaviour : function(ai, action, unit, enemies, enemyBuildings, map)
//    {
//        // called when a unit has no predefined ai behaviour modify the action and return true for the ai to execute the defined action
//    },

//    getBuildingTarget : function(ai, action, points, wholeMap, owner, map)
//    {
//        return Qt.point(-1, -1); // return the target point the building should use for that action
//        return false; // in case the core implementation should take care of this
//    },

    getHighPrioBuildings : function(ai)
    {
        return COREAI.getHighPrioBuildings(ai);
    },

    buildingMenuFunctions : [["ZBLACKHOLE_FACTORY", AI_BEHAVIOR_DISPATCH.getFactoryMenuItem],
                             ["ZBLACKHOLE_FACTORYWASTE", AI_BEHAVIOR_DISPATCH.getFactoryMenuItem],
                             ["ZBLACKHOLE_FACTORYSNOW", AI_BEHAVIOR_DISPATCH.getFactoryMenuItem],
                             ["ZBLACKHOLE_FACTORYDESERT", AI_BEHAVIOR_DISPATCH.getFactoryMenuItem],
                             ["ZNEST_FACTORY", AI_BEHAVIOR_DISPATCH.getFactoryMenuItem],],

    getBuildingMenuItem : function(ai, action, ids, costs, enabled, units, buildings, owner, map)
    {
        return AI_BEHAVIOR_DISPATCH.getBuildingMenuItem(NORMALAI.buildingMenuFunctions, ai, action, ids, costs, enabled, units, buildings, owner, map);
    },

    onBuildingMenuItemResult : function(ai, action, succeeded, x, y, actionId, map)
    {
        return AI_BEHAVIOR_DISPATCH.onBuildingMenuItemResult(ai, action, succeeded, x, y, actionId, map);
    },

    groupDistribution : [1,      // infantry units
                         1,      // light units
                         1,      // medium units
                         1,],    // heavy units
    initializeSimpleProductionSystem : function(system, ai, map)
    {
        return AI_BEHAVIOR_DISPATCH.initializeSimpleProductionSystem(system, ai, map, NORMALAI.groupDistribution);
    },
    prepareProduction : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return AI_BEHAVIOR_DISPATCH.prepareProduction(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },
    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return AI_BEHAVIOR_DISPATCH.buildUnitSimpleProductionSystem(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },
    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return AI_BEHAVIOR_DISPATCH.onNewBuildQueue(system, ai, buildings, units, enemyUnits, enemyBuildings, map, NORMALAI.groupDistribution);
    },


};
