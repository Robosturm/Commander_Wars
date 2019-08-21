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
        unit.setBaseMovementPoints(7);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(3);
    };
    this.getBaseCost = function()
    {
        return 20000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("transportplane", false);
        unit.loadSprite("transportplane+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
    this.getName = function()
    {
        return qsTr("Transportplane");
    };
    this.createExplosionAnimation = function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+air", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("moveair.wav", -2);
        return animation;
    };
    this.getTransportUnits = function()
    {
        return ["ANTITANKCANNON", "APC", "ARTILLERY", "FLAK", "FLARE",
                "HEAVY_HOVERCRAFT", "HEAVY_TANK", "HOVERCRAFT", "HOVERFLAK",
                "INFANTRY", "LIGHT_TANK", "MECH", "MEGATANK", "MISSILE",
                "MOTORBIKE", "NEOTANK", "RECON", "ROCKETTHROWER", "SNIPER"];
    };
    this.useTerrainDefense = function()
    {
        return false;
    };
    this.startOfTurn = function(unit)
    {
        // pay unit upkeep
        var fuelCosts = 5 + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), 5);
        if (fuelCosts < 0)
        {
            fuelCosts = 0;
        }
        unit.setFuel(unit.getFuel() - fuelCosts);
        UNIT.transporterRefilling(unit);
    };
    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_air";
    };

    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "";
    };

    this.getDescription = function()
    {
        return qsTr("Air unit. <div c='00ff00'>Carries X 2 </div> ground units. No attack capability.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Air;
    };
}

Constructor.prototype = UNIT;
var TRANSPORTPLANE = new Constructor();
