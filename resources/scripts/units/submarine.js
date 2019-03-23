var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_TORPEDO");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(6);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(5);
    };
    this.getBaseCost = function()
    {
        return 20000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("submarine", false);
        unit.loadSprite("submarine+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };
    this.getName = function()
    {
        return qsTr("Submarine");
    };
    this.startOfTurn = function(unit)
    {
        // pay unit upkeep
        var fuelCosts = 1 + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), 1);
        if (unit.getHidden())
        {
            fuelCosts += 4;
        }
        if (fuelCosts < 0)
        {
            fuelCosts = 0;
        }
        unit.setFuel(unit.getFuel() - fuelCosts);
    };
    this.createExplosionAnimation = function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+water.wav");
        this.doWalkingAnimation = function(action)
        {
            var unit = action.getTargetUnit();
            var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
            var unitID = unit.getUnitID().toLowerCase();
            animation.loadSprite(unitID + "+walk+mask", true, 1.25);
            animation.loadSprite(unitID + "+walk", false, 1.25);
            animation.setSound("moveship.wav", -2);
            return animation;
        };
        return animation;
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_FIRE,ACTION_STEALTH,ACTION_UNSTEALTH,ACTION_JOIN,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var SUBMARINE = new Constructor();
