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
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("lander", false);
        unit.loadSpriteV2("lander+mask", GameEnums.Recoloring_Table);
    };
    this.getMovementType = function()
    {
        return "MOVE_BOAT";
    };
    this.getBaseCost = function()
    {
        return 10000;
    };
    this.getName = function()
    {
        return qsTr("Lander");
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
            UNIT.transporterRefilling(unit);
        }
    };
    this.createExplosionAnimation = function(x, y, unit)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        animation.setSound("explosion+water.wav");
        return animation;
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.getTransportUnits = function()
    {
        return ["ANTITANKCANNON", "APC", "ARTILLERY", "FLAK", "FLARE",
                "HEAVY_HOVERCRAFT", "HEAVY_TANK", "HOVERCRAFT", "HOVERFLAK",
                "INFANTRY", "LIGHT_TANK", "MECH", "MEGATANK", "MISSILE",
                "MOTORBIKE", "NEOTANK", "RECON", "ROCKETTHROWER", "SNIPER",
                "ZCOUNIT_AT_CYCLE", "ZCOUNIT_CRYSTAL_TANK", "ZCOUNIT_INTEL_TRUCK",
                "ZCOUNIT_IRON_SHIELD_GENERATOR", "ZCOUNIT_NEOSPIDER_TANK",
                "ZCOUNIT_REPAIR_TANK", "ZCOUNIT_ROYAL_GUARD",
                "ZCOUNIT_SIEGE_CANNON", "ZCOUNIT_SMUGGLER",
                "ZCOUNIT_TANK_HUNTER", "ZCOUNIT_COMMANDO", "ZCOUNIT_PARTISAN",
                "ZCOUNIT_LOGIC_TRUCK", "ZCOUNIT_CHAPERON", "ZCOUNIT_HOT_TANK",
                "ZCOUNIT_AUTO_TANK"];
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
