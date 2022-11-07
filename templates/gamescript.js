var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return false;
    };

    this.victory = function()
    {
        // called when a player wins
    };
    this.gameStart = function(map)
    {
        // called before a game starts
    };
    this.actionDone = function(action, map)
    {
        // function called after all animations are finished
    };
    this.turnStart = function(turn, player, map)
    {
        // called at the start of each players turn
    };
    //this.PredefinedUnitMapScriptBehaviour = function(ai, action, unit, enemies, enemyBuildings, map)
    //{
    // called when a unit has the scripted ai modify the action and return true for the ai to execute the defined action
    //};

    //this.GenericPredefinedUnitMapScriptBehaviour = function(ai, action, unit, enemies, enemyBuildings, map)
    //{
    // called when a unit has no predefined ai behaviour modify the action and return true for the ai to execute the defined action
    //};

    // example code to have all units move at random fields doing nothing else
    //    var pfs = unit.createUnitPathFindingSystem(unit.getOwner());
    //    var points = pfs.getAllQmlVectorPoints();
    //    var size = points.size();
    //    var ret = false;
    //    if (size > 0)
    //    {
    //        var targetIdx = globals.randInt(0, size - 1);
    //        var target = points.at(targetIdx);
    //        var terrain = map.getTerrain(target.x, target.y);
    //        var fieldUnit = terrain.getUnit();
    //        if (fieldUnit === null)
    //        {
    //            var path = pfs.getPath(target.x, target.y);
    //            action.setActionID("ACTION_WAIT");
    //            action.setMovepath(path, pfs.getCosts(path));
    //            ret =  true;
    //        }
    //    }
    //    return ret;

    //    getBuildingTarget : function(ai, action, points, wholeMap, owner, map)
    //    {
    //        return Qt.point(-1, -1); // return the target point the building should use for that action
    //        return false; // in case the core implementation should take care of this
    //    },

    //    getBuildingMenuItem : function(ai, action, ids, costs, enabled, units, owner, map)
    //    {
    //        return -1; // valid index of the ids array
    //        return false; // in case the core implementation should take care of this
    //    },

    //    groupDistribution : [1,      // infantry units
    //                         1,      // light units
    //                         1,      // medium units
    //                         1,],    // heavy units
    //    initializeSimpleProductionSystem : function(system, ai, map)
    //    {
    //        return COREAI.initializeSimpleProductionSystem(system, ai, map, gameScript.groupDistribution);
    //    },
    //    buildUnitSimpleProductionSystem : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    //    {
    //        return COREAI.buildUnitSimpleProductionSystem(system, ai, buildings, units, enemyUnits, enemyBuildings, map);
    //    },
    //    onNewBuildQueue : function(system, ai, buildings, units, enemyUnits, enemyBuildings, map)
    //    {
    //        return COREAI.onNewBuildQueue(system, ai, buildings, units, enemyUnits, enemyBuildings, map, gameScript.groupDistribution);
    //    },
}

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
