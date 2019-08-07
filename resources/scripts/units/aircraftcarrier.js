var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_CARRIER_ANTI_AIR_MISSILE");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(3);
        unit.setMaxRange(8);
		unit.setVision(4);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("aircraftcarrier", false);
        unit.loadSprite("aircraftcarrier+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };

    this.getBaseCost = function()
    {
        return 28000;
    };

    this.getName = function()
    {
        return qsTr("Aircraft Carrier");
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.startOfTurn = function(unit)
    {
        // pay unit upkeep
        var fuelCosts = 1 + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), 1);
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
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+water.wav");
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.getTransportUnits = function()
    {
        return ["T_HELI", "K_HELI", "BOMBER", "DUSTER", "FIGHTER", "WATERPLANE", "STEALTHBOMBER"];
    };
    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_air";
    };

    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_sea";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_sea";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_FIRE,ACTION_JOIN,ACTION_LOAD,ACTION_UNLOAD,ACTION_BUILD_WATERPLANE,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };

    this.getDescription = function()
    {
        return qsTr("Naval Unit. Can <div c='00ff00'>carry X 2</div> air units and <div c='00ff00'>produce </div>Waterplanes.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
}

Constructor.prototype = UNIT;
var AIRCRAFTCARRIER = new Constructor();
