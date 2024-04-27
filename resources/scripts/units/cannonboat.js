var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(1);
        unit.setMaxAmmo1(1);
        unit.setWeapon1ID("WEAPON_ANTI_SHIP_MISSILE");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(60);
        unit.setMaxFuel(60);
        unit.setBaseMovementPoints(7);
        unit.setMinRange(1);
        unit.setMaxRange(2);
		unit.setVision(2);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("missileboat+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_SMALL_BOAT";
    };
    this.getBaseCost = function()
    {
        return 7000;
    };
    this.getName = function()
    {
        return qsTr("Missile boat");
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
        animation.loadSpriteV2("missileboat+walk+mask", GameEnums.Recoloring_Matrix, 1.5);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.getTerrainAnimationBase = function(unit, terrain, defender, map)
    {
        var terrainId = terrain.getID();
        if (terrainId === "RIVER" || terrainId === "DESERT_TRY_RIVER")
        {
            return Global[terrainId].getTerrainAnimationBase(unit, terrain, defender, map);
        }
        else
        {
            var weatherModifier = TERRAIN.getWeatherModifier(map);
            return "base_" + weatherModifier + "air";
        }
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var terrainId = terrain.getID();
        if (terrainId === "RIVER" || terrainId === "DESERT_TRY_RIVER")
        {
            return Global[terrainId].getTerrainAnimationBackground(unit, terrain, defender, map);
        }
        else
        {
            var weatherModifier = TERRAIN.getWeatherModifier(map);
            return "back_" + weatherModifier +"sea";
        }
    };

    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Short-range Naval unit that can engage other Naval units at a distance. Like all boats, it can also traverse rivers.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveship.wav";
    };
}

Constructor.prototype = UNIT;
var CANNONBOAT = new Constructor();
