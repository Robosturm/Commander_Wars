var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(3);
        unit.setMaxAmmo1(3);
        unit.setWeapon1ID("WEAPON_BOAT_TORPEDO");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(2);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("torpedoboat+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_SMALL_BOAT";
    };
    this.getBaseCost = function()
    {
        return 8500;
    };
    this.getName = function()
    {
        return qsTr("Torpedo boat");
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
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 1.5);
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
        return qsTr("<r>Short range Naval Unit intended for harrasing much larger Naval Units. Like all boats, can also traverse rivers.</r>");
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
var TORPEDOBOAT = new Constructor();
