var NORMALAI =
{    
//    GenericPredefinedUnitMapScriptBehaviour : function(ai, action, unit, enemies, enemyBuildings, map)
//    {
//        // called when a unit has no predefined ai behaviour modify the action and return true for the ai to execute the defined action
//    },
    initializeSimpleProductionSystem : function(system, ai, map)
    {
        system.addInitialProduction("INFANTRY", 6);
    },
    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return system.buildNextUnit(buildings, units, 0, 10);
    }
};
