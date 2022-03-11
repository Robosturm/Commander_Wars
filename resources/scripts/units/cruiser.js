var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_ANTI_SHIP_CRUISER_MISSILE");

        unit.setAmmo2(9);
        unit.setMaxAmmo2(9);
        unit.setWeapon2ID("WEAPON_A_AIR_GUN");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(5);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("cruiser+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };
    this.getBaseCost = function()
    {
        return 12000;
    };
    this.getName = function()
    {
        return qsTr("Cruiser");
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
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.transportList = ["T_HELI", "K_HELI"];
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
        return qsTr("<r>Naval Unit. Strong against air units and submarines. </r><div c='#00ff00'>Transports X 2</div><r> Helis.</r>");
    };

    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
}

Constructor.prototype = UNIT;
var CRUISER = new Constructor();
