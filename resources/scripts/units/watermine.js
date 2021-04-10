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
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("watermine", false);
        unit.loadSpriteV2("watermine+mask", GameEnums.Recoloring_Table);
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
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
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
                var targetUnit = terrain.getUnit();
                if (targetUnit !== null &&
                    owner.isEnemyUnit(targetUnit) &&
                    terrain.getBaseTerrainID() === "SEA" &&
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
                    terrain = map.getTerrain(x + point.x, y + point.y);
                    targetUnit = terrain.getUnit();
                    if (targetUnit !== null &&
                        terrain.getBaseTerrainID() === "SEA" &&
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
                    animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
                    animation.setSound("explosion+water.wav");
                }
            }
            fields.remove();
            // we destroyed a unit
            map.getGameRecorder().destroyedUnit(owner.getPlayerID(), unit.getUnitID());
            unit.killUnit();
        }
    };
    this.createExplosionAnimation = function(x, y, unit)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        animation.setSound("explosion+water.wav");
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 0;
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

    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "";
    };

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
