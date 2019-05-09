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
    this.gameStart = function()
    {
        // called before a game starts
    };
    this.actionDone = function()
    {
        // function called after all animations are finished
    };
}

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
