var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team)
    {
        
    };
    this.gameStart = function()
    {
       
    };
    this.actionDone = function(action)
    {
        
    };
    this.turnStart = function(turn, player)
    {
        // called at the start of each players turn
        if (turn === 1 && player === 0)
        {
            // play the inital dialog on day one and player 0
            gameScript.initDialog();
        }
    };
    this.initDialog = function()
    {
        
    };
    
}

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
