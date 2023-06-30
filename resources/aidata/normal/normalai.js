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

    buildingMenuFunctions : [["ZBLACKHOLE_FACTORY", COREAI.getFactoryMenuItem],
                             ["ZBLACKHOLE_FACTORYWASTE", COREAI.getFactoryMenuItem],
                             ["ZBLACKHOLE_FACTORYSNOW", COREAI.getFactoryMenuItem],
                             ["ZBLACKHOLE_FACTORYDESERT", COREAI.getFactoryMenuItem],
                             ["ZNEST_FACTORY", COREAI.getFactoryMenuItem],],

    getBuildingMenuItem : function(ai, action, ids, costs, enabled, units, buildings, owner, map)
    {
        var building = action.getTargetBuilding();
        var buildingId = building.getBuildingID();
        for (var i = 0; i < NORMALAI.buildingMenuFunctions.length; i++)
        {
            if (NORMALAI.buildingMenuFunctions[i][0] === buildingId)
            {
                return NORMALAI.buildingMenuFunctions[i][1](ai, action, ids, costs, enabled, units, buildings, owner, map);
            }
        }
        return false;
    },

    groupDistribution : [1,      // infantry units
                         1,      // light units
                         1,      // medium units
                         1,],    // heavy units
    initializeSimpleProductionSystem : function(system, ai, map)
    {
        return COREAI.initializeSimpleProductionSystem(system, ai, map, NORMALAI.groupDistribution);
    },
    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return COREAI.buildUnitSimpleProductionSystem(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },
    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return COREAI.onNewBuildQueue(system, ai, buildings, units, enemyUnits, enemyBuildings, map, NORMALAI.groupDistribution);
    },


};
