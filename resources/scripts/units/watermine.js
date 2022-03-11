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
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.startOfTurn = function(unit, map)
    {
        // explode mine?
        var owner = unit.getOwner();
        var x = unit.getX();
        var y = unit.getY();
        var fields = globals.getCircle(1, 2);
        var explode = false;
        var terrain = null;
        var baseId = "";
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                terrain = map.getTerrain(x + point.x, y + point.y);
                baseId = terrain.getBaseTerrainID();
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
            var animationCount = GameAnimationFactory.getAnimationCount();
            var queueAnimation = null;
            if (animationCount > 0)
            {
                queueAnimation = GameAnimationFactory.getAnimation(animationCount - 1);
            }
            var size = fields.size();
            for (i = 0; i < size; i++)
            {
                point = fields.at(i);
                if (map.onMap(x + point.x, y + point.y))
                {
                    var xPos = x + point.x;
                    var yPos = y + point.y;
                    terrain = map.getTerrain(xPos, yPos);
                    baseId = terrain.getBaseTerrainID();
                    if (baseId === "SEA" ||
                        baseId === "LAKE")
                    {
                        var animation = GameAnimationFactory.createAnimation(map, xPos, yPos);
                        animation.writeDataInt32(xPos);
                        animation.writeDataInt32(yPos);
                        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
                        animation.setEndOfAnimationCall("WATERMINE", "postAnimationMineDamge")
                        if (queueAnimation !== null)
                        {
                            queueAnimation.queueAnimation(animation);
                        }
                    }

                }
            }
            // we destroyed a unit
            var animation = GameAnimationFactory.createAnimation(map, x, y);
            animation.writeDataInt32(x);
            animation.writeDataInt32(y);
            animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
            animation.setSound("explosion+water.wav");
            animation.setEndOfAnimationCall("WATERMINE", "postAnimationSelfKill")
            if (queueAnimation !== null)
            {
                queueAnimation.queueAnimation(animation);
            }
        }
        fields.remove();
    };

    this.postAnimationSelfKill = function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var xPos = postAnimation.readDataInt32();
        var yPos = postAnimation.readDataInt32();
        var terrain = map.getTerrain(xPos, yPos);
        var unit = terrain.getUnit();
        if (unit !== null)
        {
            var owner = unit.getOwner();
            map.getGameRecorder().destroyedUnit(owner.getPlayerID(), unit.getUnitID());
            unit.removeUnit();
        }
    }

    this.postAnimationMineDamge = function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var xPos = postAnimation.readDataInt32();
        var yPos = postAnimation.readDataInt32();
        var terrain = map.getTerrain(xPos, yPos);
        var targetUnit = terrain.getUnit();
        if (targetUnit !== null &&
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
    }

    this.createExplosionAnimation = function(x, y, unit, map)
    {
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.setSound("explosion+water.wav");
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 0;
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
