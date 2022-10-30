var NORMALAIOFFENSIVE =
{    
    //GenericPredefinedUnitMapScriptBehaviour : function(ai, action, unit, enemies, enemyBuildings, map)
    //{
    // called when a unit has no predefined ai behaviour modify the action and return true for the ai to execute the defined action
    //};

    groupDistribution : [2,      // infantry units
                         3,      // light units
                         1,      // medium units
                         1,],    // heavy units
    initializeSimpleProductionSystem : function(system, ai, map)
    {
        return COREAI.initializeSimpleProductionSystem(system, ai, map, NORMALAIOFFENSIVE.groupDistribution);
    },
    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return COREAI.buildUnitSimpleProductionSystem(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },
    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return COREAI.onNewBuildQueue(system, ai, buildings, units, enemyUnits, enemyBuildings, map, NORMALAIOFFENSIVE.groupDistribution);
    },
};
