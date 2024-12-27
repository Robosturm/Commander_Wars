var BASEGAMESCRIPT =
{
    immediateStart : function(map)
    {
        // called to check if the game should start immediately
        return false;
    },
    immediateStart : null,

    getVictoryInfo : function(map)
    {
        return qsTr("No additional Victory Conditions are active.");
    },

    victory : function(team, map)
    {
        // called when a player wins
    },
    victory : null,
    gameStart : function(map)
    {
        // called before a game starts
    },
    gameStart : null,
    actionDone : function(action, map)
    {
        // function called after all animations are finished
    },
    actionDone : null,
    turnStart : function(turn, player, map)
    {
        // called at the start of each players turn
    },
    turnStart : null,
    onGameLoaded : function(menu, map)
    {
    },
    onGameLoaded : null,
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
