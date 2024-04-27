
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
        unit.setMinRange(2);
        unit.setMaxRange(6);
		unit.setVision(4);
    };
    
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
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 1);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.startOfTurn = function(unit, map)
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
        UNIT.transporterRefilling(unit, map);
        var size = unit.getLoadedUnitCount();
        for (var i = 0; i < size; i++)
        {
            var transportUnit = unit.getLoadedUnit(i);
            UNIT.repairUnit(transportUnit, 2, map);
        }
    };
    this.createExplosionAnimation = function(x, y, unit, map)
    {
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.setSound("explosion+water.wav");
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.transportList = ["T_HELI", "K_HELI", "BOMBER", "DUSTER", "FIGHTER", "WATERPLANE", "STEALTHBOMBER"];
    this.getTerrainAnimationBase = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "base_" + weatherModifier + "air";
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "back_" + weatherModifier +"sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };
    this.actionList = ["ACTION_FIRE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_UNLOAD", "ACTION_BUILD_WATERPLANE", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];

    this.getDescription = function()
    {
        var text =  qsTr("<r>Naval unit. Can </r><div c='#00ff00'>carry, repair, and resupply %0</div><r> air units. Able to </r><div c='#00ff00'>produce </div><r>Waterplanes.</r>");
        text = replaceTextArgs(text, [AIRCRAFTCARRIER.getLoadingPlace()]);
        return text;
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
    this.getTypeOfWeapon1 = function(unit)
    {
        return GameEnums.WeaponType_Indirect;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveship.wav";
    };
}

Constructor.prototype = UNIT;
var AIRCRAFTCARRIER = new Constructor();
