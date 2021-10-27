var Constructor = function()
{
	this.getUnitDamageID = function(unit)
    {
        return "SUBMARINE";
    };
	
    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon1ID("WEAPON_ROCKET_MISSILE");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(2);
        unit.setMaxRange(6);
        unit.setVision(1);
    };
	this.startOfTurn = function(unit)
    {
        if (unit.getTerrain() !== null)
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
        }
    };
    this.actionList = ["ACTION_FIRE", "ACTION_STEALTH", "ACTION_UNSTEALTH", "ACTION_JOIN", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("missile_sub+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };
    this.getBaseCost = function()
    {
        return 20000;
    };
    this.getName = function()
    {
        return qsTr("Missile Submarine");
    };
	this.createExplosionAnimation = function(x, y, unit)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.setSound("explosion+water.wav");
        return animation;
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        animation.loadSpriteV2("missile_sub+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.getDescription = function()
    {
        return "Submarine that can fire at range at enemy forces without taking a lot of risks.";
    };

    this.getTerrainAnimationBase = function(unit, terrain)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "base_" + weatherModifier + "air";
    };

	this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier +"sea";
    };

    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };

    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_MISSILE_SUB = new Constructor();
