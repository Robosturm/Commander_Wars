var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bg"],
                     ["bm", "bm"],
                     ["dm", "dm"],
                     ["ge", "ge"],
                     ["gs", "gs"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.animationData = [["ac", ["bazooka_bm", Qt.point(20, 16), Qt.point(-50, 20), -90]],
                          ["bd", ["bazooka_bm", Qt.point(17, 16), Qt.point(-50, 20), -90]],
                          ["bh", ["bazooka_bh", Qt.point(15, 12), Qt.point(-50, 20), -90]],
                          ["bg", ["bazooka_bh", Qt.point(15, 12), Qt.point(-50, 20), -90]],
                          ["bm", ["bazooka_bm", Qt.point(17, 16), Qt.point(-50, 20), -90]],
                          ["dm", ["bazooka_ge", Qt.point(14, 9),  Qt.point(-50, 20), -90]],
                          ["ge", ["bazooka_ge", Qt.point(15, 14), Qt.point(-50, 20), -90]],
                          ["gs", ["bazooka_ge", Qt.point(14, 9),  Qt.point(-50, 20), -90]],
                          ["ma", ["bazooka_os", Qt.point(20, 10), Qt.point(-50, 20), -90]],
                          ["os", ["bazooka_os", Qt.point(18, 17), Qt.point(-50, 20), -90]],
                          ["pf", ["bazooka_os", Qt.point(17, 16), Qt.point(-50, 20), -90]],
                          ["ti", ["bazooka_yc", Qt.point(18, 17), Qt.point(-50, 20), -90]],
                          ["yc", ["bazooka_yc", Qt.point(19, 17), Qt.point(-50, 20), -90]],];

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
            terrainId === "DESERT_ROCK")
        {
            return true
        }
        return false;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1 || defender === null)
        {
            var count = sprite.getUnitCount(BATTLEANIMATION_MECH.getMaxUnitCount());
            var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_MECH.armyData);
            var riverName = BATTLEANIMATION_MECH.getRiverString(unit);            
            sprite.loadMovingSpriteV2("mech+" + armyName + riverName + "+walk+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), Qt.point(-75, 5),
                                      Qt.point(65, 0), 600, false,
                                      1, 1);
            var spriteId = "mech+" + armyName + riverName + "+walk";
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
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadMoveInAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1 || defender === null)
        {
            var terrainId = unit.getTerrain().getTerrainID();
            if (BATTLEANIMATION_MECH.isMountain(terrainId))
            {
                BATTLEANIMATION_MECH.loadStandingAnimation(sprite, unit, defender, weapon);
            }
            else
            {
                BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "+stop", 1, 0, 1);
            }
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadStopAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count, startFrame = 0, endFrame = 0)
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_MECH.armyData);
        var riverName = BATTLEANIMATION_MECH.getRiverString(unit);
        var offset = Qt.point(-10, 5);
        sprite.loadSpriteV2("mech+" + armyName + riverName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_MECH.getMaxUnitCount(), offset, count, 1, 0, 0,
                            false, false, 100, endFrame, startFrame);
        var spriteId = "mech+" + armyName + riverName + ending;
        if (sprite.existResAnim(spriteId))
        {
            sprite.loadSpriteV2(spriteId, GameEnums.Recoloring_None,
                                BATTLEANIMATION_MECH.getMaxUnitCount(), offset, count, 1, 0, 0,
                                false, false, 100, endFrame, startFrame);
        }
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1 || defender === null)
        {
            BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "", 1);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadStandingAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MECH.getMaxUnitCount());
        if (weapon === 1)
        {
            BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "+fire", 1, 0, 2);
            var player = unit.getOwner();
            var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MECH.armyData);
            var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_MECH.animationData);
            var weaponRes = data[0];
            var offset = data[1];
            sprite.loadMovingSprite(weaponRes, false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(160, 0), 500, false,
                                    1, 1, -1);
            sprite.loadSprite("rocket_trailing_smoke",  false, BATTLEANIMATION_MECH.getMaxUnitCount(), offset);
            offset = Qt.point(data[1].x - 55, data[1].y - 5);
            sprite.loadSprite("bazooka_launch",  false, BATTLEANIMATION_MECH.getMaxUnitCount(), offset);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("baazoka_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadFireAnimation(sprite, unit, defender, weapon);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            return 700 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_MECH.getMaxUnitCount());
        }
        else
        {
            return 600 - BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_MECH.getMaxUnitCount());
        }
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "+fire", 1, 2, 2);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadStandingAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
        }
        else
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 2, 0);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MECH.getMaxUnitCount());
        var i = 0;
        if (weapon === 1)
        {
            sprite.loadSprite("cannon_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 400, true);
            sprite.addSpriteScreenshake(8, 0.95, 800, 500);
            var player = unit.getOwner();
            var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MECH.armyData);
            var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_MECH.animationData);
            var weaponRes = data[0];
            sprite.loadMovingSprite(weaponRes, false, sprite.getMaxUnitCount(), Qt.point(127, 24),
                                    Qt.point(-127, 0), 400, true,
                                    1, 1, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("rocket_flying.wav", 1, 0);
                sprite.loadSound("impact_explosion.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            var yOffset = 22;
            if (unit.getUnitType()  === GameEnums.UnitType_Air)
            {
                yOffset = 40
            }
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, yOffset),
                              1, 1.0, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_MECH.getMaxUnitCount());
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_MECH.getMaxUnitCount());
        }
    };

    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain !== null)
        {
            if (BATTLEANIMATION_MECH.isMountain(terrain.getID()))
            {
                if (unitIdx >= 4)
                {
                    return Qt.point(-20 * (6 - unitIdx), 0);
                }
            }
        }
        return Qt.point(0, 0);
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var terrainId = unit.getTerrain().getTerrainID();
        if (BATTLEANIMATION_MECH.isMountain(terrainId))
        {
            return false;
        }
        else
        {
            // return true if the unit has an implementation for loadMoveInAnimation
            return true;
        }
    };
    this.getMoveInDurationMS = function()
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_MECH.getMaxUnitCount();
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
        if (weapon === 1)
        {
            var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_MECH.armyData);
            var riverName = BATTLEANIMATION_MECH.getRiverString(unit);
            var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_MECH.animationData);
            var offset = Qt.point(-10, 5);
            var rotation = data[3];
            var movement = data[2];
            var ending = "";
            if (sprite.getHasFired())
            {
                ending = "+fire";
            }
            sprite.loadDyingMovingSprite("mech+" + armyName + riverName + ending + "+mask",
                                         "mech+" + armyName + "+dying+mask",
                                         GameEnums.Recoloring_Matrix,
                                         offset, movement, rotation, 400, 0, 2);
            var spriteId = "mech+" + armyName + riverName + ending;
            if (sprite.existResAnim(spriteId))
            {
                sprite.loadDyingMovingSprite(spriteId,
                                             "mech+" + armyName + "+dying",
                                             GameEnums.Recoloring_None,
                                             offset, movement, rotation, 400, 0, 2);
            }
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadDyingAnimation(sprite, unit, defender, weapon);
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MECH = new Constructor();
