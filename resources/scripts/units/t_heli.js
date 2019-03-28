var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setWeapon1ID("");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(1);
    };
    this.getBaseCost = function()
    {
        return 5000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("t_heli", false);
        unit.loadSprite("t_heli+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
    this.getName = function()
    {
        return qsTr("Transport Copter");
    };
    this.startOfTurn = function(unit)
    {
        // pay unit upkeep
        var fuelCosts = 2 + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), 2);
        if (fuelCosts < 0)
        {
            fuelCosts = 0;
        }
        unit.setFuel(unit.getFuel() - fuelCosts);
        UNIT.transporterRefilling(unit);
    };
    this.createExplosionAnimation = function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+air", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+air.wav");
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 1;
    };
    this.getTransportUnits = function()
    {
        return ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER"];
    };
    this.useTerrainDefense = function()
    {
        return false;
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1);
        animation.loadSprite(unitID + "+walk", false, 1);
        animation.setSound("movecopter.wav", -2);
        return animation;
    };
}

Constructor.prototype = UNIT;
var T_HELI = new Constructor();
