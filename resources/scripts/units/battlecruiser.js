var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_ANTI_SHIP_CANNON");

        unit.setAmmo2(9);
        unit.setMaxAmmo2(9);
        unit.setWeapon2ID("WEAPON_BATTLECRUISER_A_AIR_GUN");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
        unit.setMinRange(2);
        unit.setMaxRange(3);
        unit.setVision(1);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("battlecruiser+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };

    this.getBaseCost = function()
    {
        return 14000;
    };

    this.getName = function()
    {
        return qsTr("Battlecruiser");
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
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 1);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.canCounterOnRangeAttacks = function(unit, defX, defY, attacker, atkX, atkY, pAction, luckMode, map)
    {
        return true;
    };
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
        return qsTr("<r>A pocket </r><div c='#ff8000'>Battleship</div><r>. Weaker and faster, more suited for engaging with other Naval units. Well-armed to defend against and engage Air units.</r>");
    };

    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
    this.getTypeOfWeapon1 = function(unit, map)
    {
        return GameEnums.WeaponType_Indirect;
    };
    this.getTypeOfWeapon2 = function(unit, map)
    {
        return GameEnums.WeaponType_Direct;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveship.wav";
    };
}

Constructor.prototype = UNIT;
var BATTLECRUISER = new Constructor();
