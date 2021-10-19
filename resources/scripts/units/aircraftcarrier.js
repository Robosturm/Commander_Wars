var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_CARRIER_ANTI_AIR_MISSILE");

        unit.setAmmo2(-1);
        unit.setMaxAmmo2(-1);
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
        unit.loadSpriteV2("aircraftcarrier+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };

    this.getBaseCost = function()
    {
        return 20000;
    };
    this.canMoveAndFire = function()
    {
        return true;
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
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.startOfTurn = function(unit)
    {
        if (unit.getTerrain() !== null)
        {
            // pay unit upkeep
            var fuelCosts = 1 + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), 1);
            if (fuelCosts < 0)
            {
                fuelCosts = 0;
            }
            unit.setFuel(unit.getFuel() - fuelCosts);            
        }
        UNIT.transporterRefilling(unit);
        for (var i = 0; i < unit.getLoadedUnitCount(); i++)
        {
            var transportUnit = unit.getLoadedUnit(i);
            UNIT.repairUnit(transportUnit, 2);
        }
    };
    this.createExplosionAnimation = function(x, y, unit)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.setSound("explosion+water.wav");
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.transportList = ["T_HELI", "K_HELI", "BOMBER", "DUSTER", "FIGHTER", "WATERPLANE", "STEALTHBOMBER"];
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
    this.actionList = ["ACTION_FIRE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_UNLOAD", "ACTION_BUILD_WATERPLANE", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];

    this.getDescription = function()
    {
        return "<r>" + qsTr("Naval Unit. Can ") + "</r>" +
               "<div c='#00ff00'>"+ qsTr("carry X 2") + "</div>" +
               "<r>" + qsTr(" air units and ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("produce ") + "</div>" +
               "<r>" + qsTr("Waterplanes") + "</r>";
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
}

Constructor.prototype = UNIT;
var AIRCRAFTCARRIER = new Constructor();
