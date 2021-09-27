var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["bm", "bm"],
                     ["dm", "dm"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.animationData = [["ac", [65, Qt.point(-75, 5), Qt.point(15, 19), Qt.point(15, 14), Qt.point(15, 14), Qt.point(-50, 20), -90]],
                          ["bd", [65, Qt.point(-75, 5), Qt.point(19, 27), Qt.point(21, 16), Qt.point(20, 19), Qt.point(-30, 15), 0]],
                          ["bh", [65, Qt.point(-75, 5), Qt.point(18, 23), Qt.point(18, 13), Qt.point(18, 16), Qt.point(-30, 15), 0]],
                          ["bm", [65, Qt.point(-75, 5), Qt.point(14, 26), Qt.point(15, 16), Qt.point(15, 19), Qt.point(-30, 15), 0]],
                          ["dm", [65, Qt.point(-75, 5), Qt.point(8, 20),  Qt.point(8, 12),  Qt.point(8, 13),  Qt.point(-50, 20), -90]],
                          ["ge", [65, Qt.point(-75, 5), Qt.point(14, 23), Qt.point(14, 14), Qt.point(14, 15), Qt.point(-30, 15), 0]],
                          ["ma", [65, Qt.point(-75, 5), Qt.point(16, 19), Qt.point(16, 14), Qt.point(16, 14), Qt.point(-50, 20), -90]],
                          ["os", [65, Qt.point(-75, 5), Qt.point(14, 21), Qt.point(14, 11), Qt.point(14, 13), Qt.point(-30, 15), 0]],
                          ["pf", [65, Qt.point(-75, 5), Qt.point(13, 19), Qt.point(13, 14), Qt.point(13, 14), Qt.point(-50, 20), -90]],
                          ["ti", [65, Qt.point(-75, 5), Qt.point(15, 24), Qt.point(15, 19), Qt.point(15, 19), Qt.point(-50, 20), -90]],
                          ["yc", [65, Qt.point(-75, 5), Qt.point(21, 25), Qt.point(21, 16), Qt.point(21, 18), Qt.point(-30, 15), 0]],];

    this.getArmyName = function(unit)
    {
        var player = unit.getOwner();
        return Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_INFANTRY.armyData);
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
            terrainId === "DESERT_ROCK")
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

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_INFANTRY.getMaxUnitCount());
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_INFANTRY.armyData);
        var riverName = BATTLEANIMATION_INFANTRY.getRiverString(unit);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_INFANTRY.animationData);
        sprite.loadMovingSpriteV2("infantry+" + armyName + riverName + "+walk+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), data[1],
                                  Qt.point(data[0], 0), 600, false,
                                  1, 1);
        if (riverName !== "")
        {
            sprite.loadMovingSprite("infantry+" + armyName + riverName + "+walk", false, sprite.getMaxUnitCount(), Qt.point(-75, 5),
                                    Qt.point(65, 0), 600, false,
                                    1, 1);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("infantry_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count)
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_INFANTRY.armyData);
        var riverName = BATTLEANIMATION_INFANTRY.getRiverString(unit);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_INFANTRY.animationData);
        var offset = Qt.point(data[0] + data[1].x, data[1].y);
        sprite.loadSpriteV2("infantry+" + armyName + riverName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_INFANTRY.getMaxUnitCount(), offset, count);
        if (riverName !== "")
        {
            sprite.loadSprite("infantry+" + armyName + riverName + ending,  false,
                              BATTLEANIMATION_INFANTRY.getMaxUnitCount(), offset, count);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+stop", 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_INFANTRY.getMaxUnitCount());
        var armyName = BATTLEANIMATION_INFANTRY.getArmyName(unit);
        var offset = Qt.point(0, 0);
        var position = BATTLEANIMATION.getRelativePosition(unit, defender);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_INFANTRY.animationData);
        if (position > 0)
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire+up", 3);            
            offset = data[2];
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset);
        }
        else if (position < 0)
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire+down", 3);
            offset = data[3];
            sprite.loadSprite("mg_shot_down",  false, sprite.getMaxUnitCount(), offset);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
            offset = data[4];
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_INFANTRY.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_INFANTRY.getMaxUnitCount());
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_INFANTRY.getMaxUnitCount());
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var terrainId = unit.getTerrain().getTerrainID();
        if (BATTLEANIMATION_INFANTRY.isMountain(terrainId))
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
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_INFANTRY.getMaxUnitCount();
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
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_INFANTRY.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_INFANTRY.animationData);
        var offset = Qt.point(-10, 5);
        var riverName = BATTLEANIMATION_INFANTRY.getRiverString(unit);
        var rotation = data[6];
        var movement = data[5];
        sprite.loadDyingMovingSprite("infantry+" + armyName + riverName + "+mask",
                                     "infantry+" + armyName + "+dying+mask",
                                     GameEnums.Recoloring_Matrix,
                                     offset, movement, rotation, 300);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_INFANTRY = new Constructor();
