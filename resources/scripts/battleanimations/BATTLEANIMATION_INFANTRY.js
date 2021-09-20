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
        sprite.loadMovingSpriteV2("infantry+" + armyName + riverName + "+walk+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), Qt.point(-75, 5),
                                  Qt.point(65, 0), 600, false,
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
        var offset = Qt.point(-10, 5);
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
        if (position > 0)
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire+up", 3);
            if (armyName === "ac")
            {
                offset = Qt.point(15, 19);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(18, 23);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(14, 26);
            }
            else if (armyName === "dm")
            {
                offset = Qt.point(8, 20);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(14, 23);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(16, 19);
            }
            else if (armyName === "os")
            {
                offset = Qt.point(14, 21);
            }
            else if (armyName === "pf")
            {
                offset = Qt.point(13, 19);
            }
            else if (armyName === "ti")
            {
                offset = Qt.point(15, 24);
            }
            else if (armyName === "yc")
            {
                offset = Qt.point(21, 25);
            }
            else if (armyName === "bd")
            {
                offset = Qt.point(19, 27);
            }
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset);
        }
        else if (position < 0)
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire+down", 3);
                        // -10
            if (armyName === "ac")
            {
                offset = Qt.point(15, 14);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(18, 13);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(15, 16);
            }
            else if (armyName === "dm")
            {
                offset = Qt.point(8, 12);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(14, 14);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(16, 14);
            }
            else if (armyName === "os")
            {
                offset = Qt.point(14, 11);
            }
            else if (armyName === "pf")
            {
                offset = Qt.point(13, 14);
            }
            else if (armyName === "ti")
            {
                offset = Qt.point(15, 19);
            }
            else if (armyName === "yc")
            {
                offset = Qt.point(21, 16);
            }
            else if (armyName === "bd")
            {
                offset = Qt.point(21, 16);
            }
            sprite.loadSprite("mg_shot_down",  false, sprite.getMaxUnitCount(), offset);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
            if (armyName === "ac")
            {
                offset = Qt.point(15, 14);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(18, 16);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(15, 19);
            }
            else if (armyName === "dm")
            {
                offset = Qt.point(8, 13);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(14, 15);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(16, 14);
            }
            else if (armyName === "os")
            {
                offset = Qt.point(14, 13);
            }
            else if (armyName === "pf")
            {
                offset = Qt.point(13, 14);
            }
            else if (armyName === "ti")
            {
                offset = Qt.point(15, 19);
            }
            else if (armyName === "yc")
            {
                offset = Qt.point(21, 18);
            }
            else if (armyName === "bd")
            {
                offset = Qt.point(20, 19);
            }
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
        return 800 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_INFANTRY.getMaxUnitCount());
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
        var offset = Qt.point(-10, 5);
        var riverName = BATTLEANIMATION_INFANTRY.getRiverString(unit);
        var rotation = 0;
        var movement = Qt.point(-30, 15);
        if (armyName === "ac" ||
            armyName === "dm" ||
            armyName === "ma" ||
            armyName === "pf" ||
            armyName === "ti")
        {
            rotation = -90;
            movement = Qt.point(-50, 20);
        }
        sprite.loadDyingMovingSprite("infantry+" + armyName + riverName + "+mask",
                                     "infantry+" + armyName + "+dying+mask",
                                     GameEnums.Recoloring_Matrix,
                                     offset, movement, rotation, 300);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_INFANTRY = new Constructor();
