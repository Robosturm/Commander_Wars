var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy();
        // bh and bg have the same sprites
        if (armyName === "BG")
        {
            armyName = "BH"
        }
        // load sprites
        unit.loadSprite("infantry+" + armyName, false);
        unit.loadSprite("infantry+" + armyName +"+mask", true);
    };
    this.getMovementType = function()
    {
        return "FEET";
    };
}

Constructor.prototype = UNIT;
var INFANTRY = new Constructor();
