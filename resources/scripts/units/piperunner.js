var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_PIPERUNNER_MISSILE");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(99);
        unit.setMaxFuel(99);
        unit.setBaseMovementPoints(9);
        unit.setMinRange(2);
        unit.setMaxRange(5);
        unit.setVision(4);
    };
    this.getBaseCost = function()
    {
        return 20000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("piperunner", false);
        unit.loadSprite("piperunner+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_PIPERUNNER";
    };
    this.getName = function()
    {
        return qsTr("Piperunner");
    };
    this.canMoveAndFire = function()
    {
        return false;
    };
}

Constructor.prototype = UNIT;
var PIPERUNNER = new Constructor();
