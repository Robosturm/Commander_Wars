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

        unit.setFuel(0);
        unit.setMaxFuel(0);
        unit.setBaseMovementPoints(0);
        unit.setMinRange(0);
        unit.setMaxRange(0);
        unit.setVision(1);
		
		unit.setHidden(true);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("watermine+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };

    this.getBaseCost = function()
    {
        return 1000;
    };

    this.getName = function()
    {
        return qsTr("Watermine");
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
        // explode mine?
        var owner = unit.getOwner();
        var x = unit.getX();
        var y = unit.getY();
        var fields = globals.getCircle(1, 2);
        var explode = false;
        for (var i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var terrain = map.getTerrain(x + point.x, y + point.y);
                var baseId = terrain.getBaseTerrainID();
                var targetUnit = terrain.getUnit();
                if (targetUnit !== null &&
                    owner.isEnemyUnit(targetUnit) &&
                    (baseId  === "SEA" || baseId  === "LAKE") &&
                    targetUnit.getUnitType() !== GameEnums.UnitType_Air)
                {
                    explode = true;
                    break;
                }
            }
        }
        if (explode)
        {
            for (i = 0; i < fields.size(); i++)
            {
                point = fields.at(i);
                if (map.onMap(x + point.x, y + point.y))
                {
                    var terrain = map.getTerrain(x + point.x, y + point.y);
                    var baseId = terrain.getBaseTerrainID();
                    targetUnit = terrain.getUnit();
                    if (targetUnit !== null &&                            
                       (baseId  === "SEA" || baseId  === "LAKE") &&
                        targetUnit.getUnitType() !== GameEnums.UnitType_Air)
                    {
                        targetUnit.setHp(targetUnit.getHpRounded() - 4);
                        if (targetUnit.getHp() <= 0)
                        {
                            // we destroyed a unit
                            map.getGameRecorder().destroyedUnit(targetUnit.getOwner().getPlayerID(), targetUnit.getUnitID());
                            targetUnit.killUnit();
                        }
                    }
                    var animation = GameAnimationFactory.createAnimation(x + point.x, y + point.y);
                    animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
                    animation.setSound("explosion+water.wav");
                }
            }
            // we destroyed a unit
            map.getGameRecorder().destroyedUnit(owner.getPlayerID(), unit.getUnitID());
            unit.killUnit();
        }
        fields.remove();
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
        return 0;
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
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier +"sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };
    this.actionList = [];
    this.getDescription = function()
    {
        return qsTr("Naval Mine. Always explodes at the start of a turn if an enemy unit is inside the explosion range. This unit is always stealthed. It deals 4 damage to all sea and ground units in a 2 square area on sea tiles.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Naval;
    };
}

Constructor.prototype = UNIT;
var WATERMINE = new Constructor();
