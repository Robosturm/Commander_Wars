var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_STEALTHROCKET");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setVisionHigh(999);
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(7);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("stealthbomber+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
    this.getBaseCost = function()
    {
        return 22000;
    };
    this.getName = function()
    {
        return qsTr("Stealthbomber");
    };
    this.startOfTurn = function(unit)
    {
        if (unit.getTerrain() !== null)
        {
            // pay unit upkeep
            var fuelCosts = 5;
            if (unit.getHidden())
            {
                fuelCosts += 3;
            }
            fuelCosts = fuelCosts + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), fuelCosts);
            if (fuelCosts < 0)
            {
                fuelCosts = 0;
            }
            unit.setFuel(unit.getFuel() - fuelCosts);
        }
    };
    this.createExplosionAnimation = function(x, y, unit)
    {
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+air", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.setSound("explosion+air.wav");
        return animation;
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveair.wav", -2);
        return animation;
    };
    this.actionList = ["ACTION_FIRE", "ACTION_STEALTH", "ACTION_UNSTEALTH", "ACTION_LOAD", "ACTION_JOIN", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.useTerrainDefense = function()
    {
        return false;
    };
    this.useTerrainHide = function()
    {
        return false;
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
        return "";
    };

    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Air unit. After </r><div c='#00ff00'>hiding </div><r>, it remains hidden unless enemy units adjacent. Can attack any unit.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Air;
    };
}

Constructor.prototype = UNIT;
var STEALTHBOMBER = new Constructor();
