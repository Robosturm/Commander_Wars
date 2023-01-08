var BASEGAMESCRIPT =
{
    immediateStart : function(map)
    {
        // called to check if the game should start immediately
        return false;
    },

    getVictoryInfo : function(map)
    {
        return qsTr("No additional Victory Conditions are active.");
    },

    victory : function(team, map)
    {
        // called when a player wins
    },
    gameStart : function(map)
    {
        // called before a game starts
    },
    actionDone : function(action, map)
    {
        // function called after all animations are finished
    },
    turnStart : function(turn, player, map)
    {
        // called at the start of each players turn
    },
    onGameLoaded : function(menu, map)
    {

    },
    // optional functions not defined in the base class
    //this.PredefinedUnitMapScriptBehaviour = function(ai, action, unit, enemies, enemyBuildings, map)
    //{
    // called when a unit has the scripted ai modify the action and return true for the ai to execute the defined action
    //};

    //this.GenericPredefinedUnitMapScriptBehaviour = function(ai, action, unit, enemies, enemyBuildings, map)
    //{
    // called when a unit has no predefined ai behaviour modify the action and return true for the ai to execute the defined action
    //};
};
