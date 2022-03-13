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
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("lander+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_BOAT";
    };
    this.getBaseCost = function()
    {
        return 8000;
    };
    this.getName = function()
    {
        return qsTr("Lander");
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
            UNIT.transporterRefilling(unit, map);
        }
    };
    this.createExplosionAnimation = function(x, y, unit, map)
    {
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.setSound("explosion+water.wav");
        return animation;
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };

    this.transportList = ["ANTITANKCANNON", "APC", "ARTILLERY", "FLAK", "FLARE",
                          "HEAVY_HOVERCRAFT", "HEAVY_TANK", "HOVERCRAFT", "HOVERFLAK",
                          "INFANTRY", "LIGHT_TANK", "MECH", "MEGATANK", "MISSILE",
                          "MOTORBIKE", "NEOTANK", "RECON", "ROCKETTHROWER", "SNIPER",
                          "ZCOUNIT_AT_CYCLE", "ZCOUNIT_CRYSTAL_TANK", "ZCOUNIT_INTEL_TRUCK",
                          "ZCOUNIT_IRON_SHIELD_GENERATOR", "ZCOUNIT_NEOSPIDER_TANK",
                          "ZCOUNIT_REPAIR_TANK", "ZCOUNIT_ROYAL_GUARD",
                          "ZCOUNIT_SIEGE_CANNON", "ZCOUNIT_SMUGGLER",
                          "ZCOUNIT_TANK_HUNTER", "ZCOUNIT_COMMANDO", "ZCOUNIT_PARTISAN",
                          "ZCOUNIT_LOGIC_TRUCK", "ZCOUNIT_CHAPERON", "ZCOUNIT_HOT_TANK",
                          "ZCOUNIT_AUTO_TANK", "ZCOUNIT_RANGER"];
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

    this.getDescription = function()
    {
        return qsTr("<r>Naval unit. Can </r><div c='#00ff00'>transport X 2 </div><r> ground units. No attack capability.</r>");
    };

    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
}

Constructor.prototype = UNIT;
var LANDER = new Constructor();
