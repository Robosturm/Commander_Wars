var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediately without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team, map)
    {
        
    };
    this.gameStart = function(map)
    {
       
    };
    this.actionDone = function(action, map)
    {
        
    };
    this.turnStart = function(turn, player, map)
    {
        // called at the start of each players turn
        if (turn === 1 && player === 0)
        {
            // play the inital dialog on day one and player 0
            gameScript.initDialog(map);
        }
    };
    this.initDialog = function(map)
    {
        
    };
    
}

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
