var BASEGAMESCRIPT =
{
    immediateStart : function()
    {
        // called to check if the game should start immediatly
        return false;
    },

    getVictoryInfo : function()
    {
        return qsTr("No additional Victory Conditions are active.");
    },

    victory : function(team)
    {
        // called when a player wins
    },
    gameStart : function()
    {
        // called before a game starts
    },
    actionDone : function(action)
    {
        // function called after all animations are finished
    },
    turnStart : function(turn, player)
    {
        // called at the start of each players turn
    },
};
