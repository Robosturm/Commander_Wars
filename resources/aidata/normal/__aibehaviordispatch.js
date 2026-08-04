var AI_BEHAVIOR_DISPATCH =
{
    // Resolved on every callback rather than captured at load time: save loading restores the
    // AI controllers before the final GameRules object is deserialized.
    getStrategy : function(map)
    {
        if (map === null || map === undefined)
        {
            return COREAI;
        }
        var rules = map.getGameRules();
        if (rules === null || rules === undefined)
        {
            return COREAI;
        }
        if (rules.getAiBehaviorMode() === GameEnums.AiBehavior_Counterpoint)
        {
            return COUNTERPOINTAI;
        }
        return COREAI;
    },

    _callable : function(strategy, name)
    {
        return strategy !== null && strategy !== undefined && typeof strategy[name] === "function";
    },

    initializeSimpleProductionSystem : function(system, ai, map, groupDistribution)
    {
        return AI_BEHAVIOR_DISPATCH.getStrategy(map).initializeSimpleProductionSystem(
            system, ai, map, groupDistribution);
    },

    // Counterpoint-only seam: Standard has no early planning phase and returns false so the
    // engine leaves the building scan untouched.
    prepareProduction : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        var strategy = AI_BEHAVIOR_DISPATCH.getStrategy(map);
        if (!AI_BEHAVIOR_DISPATCH._callable(strategy, "prepareProduction"))
        {
            return false;
        }
        return strategy.prepareProduction(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },

    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map, groupDistribution)
    {
        return AI_BEHAVIOR_DISPATCH.getStrategy(map).onNewBuildQueue(
            system, ai, buildings, units, enemyUnits, enemyBuildings, map, groupDistribution);
    },

    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    {
        return AI_BEHAVIOR_DISPATCH.getStrategy(map).buildUnitSimpleProductionSystem(
            system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    },

    getFactoryMenuItem : function(ai, action, ids, costs, enabled, units, buildings, owner, map)
    {
        return AI_BEHAVIOR_DISPATCH.getStrategy(map).getFactoryMenuItem(
            ai, action, ids, costs, enabled, units, buildings, owner, map);
    },

    // Shared by every wrapper so the building id table stays wrapper-owned and moddable while
    // the selected strategy is still resolved per call.
    getBuildingMenuItem : function(menuFunctions, ai, action, ids, costs, enabled, units, buildings, owner, map)
    {
        var building = action.getTargetBuilding();
        if (building === null || building === undefined)
        {
            return false;
        }
        var buildingId = building.getBuildingID();
        for (var i = 0; i < menuFunctions.length; ++i)
        {
            if (menuFunctions[i][0] === buildingId)
            {
                return menuFunctions[i][1](ai, action, ids, costs, enabled, units, buildings, owner, map);
            }
        }
        return false;
    },

    onBuildingMenuItemResult : function(ai, action, succeeded, x, y, actionId, map)
    {
        var strategy = AI_BEHAVIOR_DISPATCH.getStrategy(map);
        if (!AI_BEHAVIOR_DISPATCH._callable(strategy, "onBuildingMenuItemResult"))
        {
            return false;
        }
        return strategy.onBuildingMenuItemResult(ai, action, succeeded, x, y, actionId, map);
    },
};
