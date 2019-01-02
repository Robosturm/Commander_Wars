var Constructor = function()
{
    this.init = function(unit)
    {
        unit.ammo1 = 10;
        unit.maxAmmo1 = 10;
        unit.ammo2 = 0;
        unit.maxAmmo2 = 0;
        unit.fuel = 100;
        unit.maxFuel = 100;
        unit.baseMovementPoints = 3;
    };
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
        return "MOVE_FEET";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_WAIT,ACTION_CAPTURE";
    };
}

Constructor.prototype = UNIT;
var INFANTRY = new Constructor();
