var NORMALAIDEFENSIVE =
{    
    //GenericPredefinedUnitMapScriptBehaviour : function(ai, action, unit, enemies, enemyBuildings, map)
    //{
    // called when a unit has no predefined ai behaviour modify the action and return true for the ai to execute the defined action
    //};

    groupDistribution : [1,      // infantry units
                         1,      // light units
                         3,      // medium units
                         2,],    // heavy units
    initializeSimpleProductionSystem : function(system, ai, map)
    {
        return COREAI.initializeSimpleProductionSystem(system, ai, map, NORMALAIDEFENSIVE.groupDistribution);
    },
    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return COREAI.buildUnitSimpleProductionSystem(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },
    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return COREAI.onNewBuildQueue(system, ai, buildings, units, enemyUnits, enemyBuildings, map, NORMALAIDEFENSIVE.groupDistribution);
    },
};
