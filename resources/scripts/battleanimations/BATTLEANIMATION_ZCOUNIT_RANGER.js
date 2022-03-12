var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["bd", "os"],];

    //                                                          up              down            // normal
    this.animationData = [["", [65, Qt.point(-75, 5), Qt.point(12, 24), Qt.point(13, 15), Qt.point(10, 17), Qt.point(-50, 20), -90]],];

    this.getArmyName = function(unit)
    {
        var player = unit.getOwner();
        return Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ZCOUNIT_RANGER.armyData);
    };

    this.getRiverString = function(unit)
    {
        var terrainId = "PLAINS";
        var terrain = unit.getTerrain();
        if (terrain !== null)
        {
            terrainId = unit.getTerrain().getTerrainID();
        }
        if (terrainId === "RIVER" ||
                terrainId === "DESERT_TRY_RIVER")
        {
            return "+river";
        }
        return "";
    };

    this.isMountain = function(terrainId)
    {
        if (terrainId === "MOUNTAIN" ||
            terrainId === "SNOW_MOUNTAIN" ||
            terrainId === "DESERT_ROCK" ||
            terrainId === "WASTE_MOUNTAIN")
        {
            return true
        }
        return false;
    };

    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain !== null)
        {
            if (terrain.getID() === "MOUNTAIN")
            {
                if (unitIdx >= 4)
                {
                    return Qt.point(-20 * (6 - unitIdx), 0);
                }
            }
        }
        return Qt.point(0, 0);
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon, direction = "")
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_RANGER.getMaxUnitCount());
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_ZCOUNIT_RANGER.armyData);
        var riverName = BATTLEANIMATION_ZCOUNIT_RANGER.getRiverString(unit);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_ZCOUNIT_RANGER.animationData);
        sprite.loadMovingSpriteV2("ranger" + riverName + "+walk+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), data[1],
                                  Qt.point(data[0], 0), 600, false,
                                  1, 1);
        var spriteId = "ranger" + riverName + "+walk";
        if (sprite.existResAnim(spriteId))
        {
            sprite.loadMovingSprite(spriteId, false, sprite.getMaxUnitCount(), Qt.point(-75, 5),
                                    Qt.point(65, 0), 600, false,
                                    1, 1);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("infantry_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
        if (weapon === 0)
        {
            BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
        }
        else
        {
            BATTLEANIMATION_ZCOUNIT_RANGER.loadDesignator(sprite, unit, direction);
        }
    };

    this.loadDesignator = function(sprite, unit, direction)
    {
        var riverName = BATTLEANIMATION_ZCOUNIT_RANGER.getRiverString(unit);
        sprite.loadSpriteV2("ranger+designator" + riverName + direction + "+mask", GameEnums.Recoloring_Matrix, 1, Qt.point(90, 5),
                            1, 1, 20);
        var spriteId = "ranger+designator" + riverName + direction;
        if (sprite.existResAnim(spriteId))
        {
            sprite.loadSpriteV2(spriteId, GameEnums.Recoloring_None, 1, Qt.point(90, 5),
                                1, 1, 20);
        }
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count, direction = "")
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_ZCOUNIT_RANGER.armyData);
        var riverName = BATTLEANIMATION_ZCOUNIT_RANGER.getRiverString(unit);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_ZCOUNIT_RANGER.animationData);
        var offset = Qt.point(data[0] + data[1].x, data[1].y);
        if (weapon === 1)
        {
            ending = "+idle" + ending;
        }

        sprite.loadSpriteV2("ranger" + riverName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ZCOUNIT_RANGER.getMaxUnitCount(), offset, count);
        var spriteId = "ranger" + riverName + ending;
        if (sprite.existResAnim(spriteId))
        {
            sprite.loadSpriteV2(spriteId, GameEnums.Recoloring_None,
                                BATTLEANIMATION_ZCOUNIT_RANGER.getMaxUnitCount(), offset, count);
        }
        if (weapon === 1)
        {
            BATTLEANIMATION_ZCOUNIT_RANGER.loadDesignator(sprite, unit, direction);
        }
        else
        {
            BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_RANGER.loadSprite(sprite, unit, defender, weapon, "+stop", 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_RANGER.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_RANGER.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var position = BATTLEANIMATION.getRelativePosition(unit, defender);
        if (weapon === 0)
        {
            var offset = Qt.point(0, 0);
            var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_ZCOUNIT_RANGER.armyData);
            var data = Global.getDataFromTable(armyName, BATTLEANIMATION_ZCOUNIT_RANGER.animationData);
            if (position > 0)
            {
                BATTLEANIMATION_ZCOUNIT_RANGER.loadSprite(sprite, unit, defender, weapon, "+fire+up", 3);
                offset = data[2];
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset);
            }
            else if (position < 0)
            {
                BATTLEANIMATION_ZCOUNIT_RANGER.loadSprite(sprite, unit, defender, weapon, "+fire+down", 3);
                offset = data[3];
                sprite.loadSprite("mg_shot_down",  false, sprite.getMaxUnitCount(), offset);
            }
            else
            {
                BATTLEANIMATION_ZCOUNIT_RANGER.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
                offset = data[4];
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset);
            }
            BATTLEANIMATION.showMgBullets(sprite, offset);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
                sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
            }
        }
        else
        {
            var direction = "";
            if (position > 0)
            {
                direction = "+up";
            }
            else if (position < 0)
            {
                direction = "+down";
            }
            BATTLEANIMATION_ZCOUNIT_RANGER.loadSprite(sprite, unit, defender, weapon, "", 6, direction);
            sprite.loadSprite("designator_shot" + direction,  false, 1, Qt.point(90, 5), 10);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_RANGER.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_RANGER.getMaxUnitCount());
        if (weapon === 0)
        {
            var yOffset = 22;
            if (unit.getUnitType()  === GameEnums.UnitType_Air)
            {
                yOffset = 40
            }
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, yOffset),
                              1, 1.0, 0, 0, true);
            BATTLEANIMATION.playMgImpactSound(sprite, unit, defender, weapon, count);
        }
        else
        {
            var i = 0;
            sprite.loadSprite("artillery_hit", false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 0, true);
            sprite.addSpriteScreenshake(8, 0.98, 800, 200);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("artillery_explode.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_RANGER.getMaxUnitCount());
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_RANGER.getMaxUnitCount());
        }
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var terrainId = unit.getTerrain().getTerrainID();
        if (BATTLEANIMATION_ZCOUNIT_RANGER.isMountain(terrainId) ||
            weapon === 1)
        {
            return false;
        }
        else
        {
            return true;
        }
    };

    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_RANGER.getMaxUnitCount();
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600;
    };

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_ZCOUNIT_RANGER.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_ZCOUNIT_RANGER.animationData);
        var offset = Qt.point(-10, 5);
        var riverName = BATTLEANIMATION_ZCOUNIT_RANGER.getRiverString(unit);
        var rotation = data[6];
        var movement = data[5];
        sprite.loadDyingMovingSprite("ranger" + riverName + "+mask",
                                     "ranger" + "+dying+mask",
                                     GameEnums.Recoloring_Matrix,
                                     offset, movement, rotation, 400);
        var spriteId = "ranger" + riverName;
        if (sprite.existResAnim(spriteId))
        {
            sprite.loadDyingMovingSprite(spriteId,
                                         "ranger+" + armyName + "+dying",
                                         GameEnums.Recoloring_None,
                                         offset, movement, rotation, 400);
        }
        if (weapon === 1)
        {
            BATTLEANIMATION_ZCOUNIT_RANGER.loadDesignator(sprite, unit, "");
        }
        else
        {
            BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_RANGER = new Constructor();
